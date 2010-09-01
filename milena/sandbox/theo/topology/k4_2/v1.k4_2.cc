
#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/debug/println.hh>
#include <mln/data/stretch.hh>
#include <mln/extension/fill.hh>

#include <mln/util/timer.hh>

#include "../utils/dh_queue_vf.hh"
#include "../utils/h_median.hh"




namespace mln
{

  namespace k4
  {

    

    template <typename T>
    T
    frontiere_median(const image2d<T>& f)
    {
      utils::h_median<T> m;
      const int
	nrows = f.nrows(),
	nrows_1 = nrows - 1,
	ncols = f.ncols(),
	ncols_1 = ncols - 1;

      // ------
      //
      // ------
      for (int col = 0; col < ncols; ++col)
	{
	  m.take(f.at_(0, col));
	  m.take(f.at_(nrows_1, col));
	}

      // ------
      // |    |
      // ------
      for (int row = 1; row < nrows_1; ++row)
	{
	  m.take(f.at_(row, 0));
	  m.take(f.at_(row, ncols_1));
	}

      return m.value();
    }



    template <typename R, typename T>
    void
    set_frontiere(image2d<R>& K, const T& m)
    {
      const int nrows = K.nrows(), ncols = K.ncols();
      const unsigned dp = ncols + 2 * K.border();
      R* p;

      // top row
      p = & K.at_(0, 0);
      for (int col = 0; col < ncols; ++col, ++p)
	*p = m;

      // bot row
      p = & K.at_(nrows - 1, 0);
      for (int col = 0; col < ncols; ++col, ++p)
	*p = m;

      // left col
      p = & K.at_(0, 0);
      for (int row = 0; row < nrows; ++row, p += dp)
	*p = m;

      // right col
      p = & K.at_(0, ncols - 1);
      for (int row = 0; row < nrows; ++row, p += dp)
	*p = m;
    }




    // -----------------------------------------   tree



    inline
    unsigned
    zfind_root(image2d<unsigned>& zpar, unsigned x)
    {
      if (zpar.element(x) == x)
	return x;
      return zpar.element(x) = zfind_root(zpar, zpar.element(x));
    }


    template <typename T>
    void print_tree_stats(const image2d<unsigned>& parent,
			  const std::vector<unsigned>& S,
			  const image2d<T>& K_)
    {
      unsigned p;

      // About leaves.
      {
	image2d<bool> is_leaf(parent.domain());
	data::fill(is_leaf, true);
	for (int i = 0; i < S.size(); ++i)
	  {
	    p = S[i];
	    is_leaf.element(parent.element(p)) = false;
	  }
	unsigned nleaves = 0;
	for (int i = 0; i < S.size(); ++i)
	  {
	    p = S[i];
	    if (is_leaf.element(p))
	      ++nleaves;
	  }
	std::cout << "nleaves = " << nleaves << std::endl;
      }

      // About nodes.
      {
	unsigned nnodes = 0;
	for (int i = 0; i < S.size(); ++i)
	  {
	    p = S[i];
	    if (parent.element(p) == p || K_.element(parent.element(p)) != K_.element(p))
	      ++nnodes;
	  }
	std::cout << "nnodes = " << nnodes << std::endl;
      }

    }



    // -----------------------------------------   tree computation



    template <typename T, typename N>
    void
    compute_autodual_tree(image2d<T>& K, const N& nbh)
    {
      typedef unsigned P;
      
      const box2d b = K.domain();
      const unsigned n_faces = b.nsites();

      util::array<int> dp = offsets_wrt(K, nbh);
      const unsigned n_nbhs = dp.nelements();

      util::timer t;



      // Reference.
      // ---------------------------------------------

      t.start();

      T l_start = frontiere_median(K);
      set_frontiere(K, l_start);

      t.stop();
      std::cout << "  reference: " << t << "s" << std::endl;



      // Initialisation.
      // ---------------------------------------------

      t.start();

      // S

      std::vector<P> S;
      S.reserve(b.nsites());

      // deja-vu is aux data

      image2d<bool> deja_vu(b);
      data::fill(deja_vu, false);
      extension::fill(deja_vu, true);

      // q

      utils::dh_queue_vf<P,T> q;
      q.init(l_start, false); // so going down.

      {
	const int
	  nrows = K.nrows(),
	  nrows_1 = nrows - 1,
	  ncols = K.ncols(),
	  ncols_1 = ncols - 1;
	unsigned p,
	  dp = K.delta_index(dpoint2d(1, 0));

	// top
	p = K.index_of_point(point2d(0, 0));
	for (int col = 0; col < ncols; ++col, ++p)
	  {
	    q.push(p, K.element(p));
	    deja_vu.element(p) = true;
	  }

	// bot
	p = K.index_of_point(point2d(nrows_1, 0));
	for (int col = 0; col < ncols; ++col, ++p)
	  {
	    q.push(p, K.element(p));
	    deja_vu.element(p) = true;
	  }

	// left
	p = K.index_of_point(point2d(1, 0));
	for (int row = 1; row < nrows_1; ++row, p += dp)
	  {
	    q.push(p, K.element(p));
	    deja_vu.element(p) = true;
	  }

	// right
	p = K.index_of_point(point2d(1, ncols_1));
	for (int row = 1; row < nrows_1; ++row, p += dp)
	  {
	    q.push(p, K.element(p));
	    deja_vu.element(p) = true;
	  }
      }

      t.stop();
      std::cout << "  initialization: " << t << "s" << std::endl;


      //  1st pass: ordering.
      // ---------------------------------------------

      t.start();

      P p;
      while (! q.is_empty())
	{
	  p = q.pop();

	  S.push_back(p);
	    
	  for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      unsigned n = p + dp[j];
	      if (deja_vu.element(n))
		continue;
	      q.push(n, K.element(n));
	      deja_vu.element(n) = true;
	    }
	}

      t.stop();
      std::cout << "  ordering: " << t << "s" << std::endl;


      //  2nd pass: rough tree.
      // ---------------------------------------------

      t.start();

      image2d<P> parent(b);

      // data for compression and union by rank:
      image2d<P> zpar(b);
      data::fill(zpar, 0);
      image2d<unsigned> rnk(b), last(b);
      data::fill(rnk, 0);

      // attribute
      image2d<unsigned> area(b);

      for (int i_face = n_faces - 1; i_face >= 0; --i_face)
	{
	  p = S[i_face];

	  parent.element(p) = p;
	  zpar.element(p) = p;
	  last.element(p) = i_face;
	  area.element(p) = 1;

	  for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      unsigned n = p + dp[j];

	      if (zpar.element(n) == 0)
		continue;

	      P r_ = zfind_root(zpar, n),
		p_ = zfind_root(zpar, p);

	      if (r_ != p_)
		{
		  P r = S[last.element(r_)];
		  parent.element(r) = p;
		  area.element(p) += area.element(r);

		  if (rnk.element(p_) > rnk.element(r_))
		    {
		      zpar.element(r_) = p_;
		      if (last.element(r_) < last.element(p_))
			last.element(p_) = last.element(r_);
		    }
		  else
		    {
		      zpar.element(p_) = r_;
		      if (last.element(p_) < last.element(r_))
			last.element(r_) = last.element(p_);
		      if (rnk.element(p_) == rnk.element(r_))
			++rnk.element(r_);
		    }
		}
	    }
	}

      t.stop();
      std::cout << "  union-find: " << t << "s" << std::endl;



      //  3rd pass: canonized tree.
      // ---------------------------------------------

      t.start();

      for (int i = 0; i < S.size(); ++i)
	{
	  P p = S[i],
	    q = parent.element(p),
	    r = parent.element(q);
	  if (K.element(r) == K.element(q))
	    parent.element(p) = r;
	}

      t.stop();
      std::cout << "  canonization: " << t << "s" << std::endl;


      //  Log.
      // ---------------------------------------------

      print_tree_stats(parent, S, K);
    }




  } // end of namespace mln::k4

} // end of namespace mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm" << std::endl;
  std::cerr << "   c4 with 2-faces only!" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv);

  using namespace mln;
  using namespace mln::k4;

  image2d<value::int_u8> f;
  io::pgm::load(f, argv[1]);

  util::timer t;
  t.start();

  compute_autodual_tree(f, c4());

  t.stop();
  std::cout << "tree computation: " << t << "s" << std::endl;
}
