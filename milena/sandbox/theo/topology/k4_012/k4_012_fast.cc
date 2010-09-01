
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

#include "../utils/dh_queue.hh"
#include "../utils/frontiere_vf.hh"




namespace mln
{

  namespace k4
  {


    //      0 1 2 
    //
    //  0   a b c
    //  1   d e f


    //      0 1 2 3 4 5 6
    //
    //  0   + - + - + - +
    //  1   | a | b | c |
    //  2   + - + - + - +
    //  3   | d | e | f |
    //  4   + - + - + - +



    template <typename I>
    mln_ch_value(I, utils::range<mln_value(I)>)
      immerse_and_interpolate(const I& f)
    {
      typedef mln_value(I) T;
      typedef utils::range<T> R;
      const int nrows = f.nrows(), ncols = f.ncols();
      image2d<R> K(2 * nrows + 1, 2 * ncols + 1);

      for (int row = 0; row < nrows; ++row)
	{
	  const T* t = & f.at_(row, 0);
	  R* r = & K.at_(2 * row + 1, 1);
	  for (int col = 0; col < ncols; ++col)
	    {
	      (*r++).set(*t);                          // 2-face
	      *r++ = utils::make_range(*t, *(t + 1));  // vertical 1-face
	      ++t;
	    }
	}

      for (int row = 0; row < nrows; ++row)
	{
	  const T* t_up = & f.at_(row, 0);
	  const T* t_do = & f.at_(row + 1, 0);
	  R* r    = & K.at_(2 * row + 2, 1);
	  const R* r_top = & K.at_(2 * row + 1, 2);
	  const R* r_bot = & K.at_(2 * row + 3, 2);
	  for (int col = 0; col < ncols; ++col)
	    {
	      *r++ = utils::make_range(*t_up++, *t_do++);      // horizontal 1-face
	      *r++ = R(std::min(r_top->lower, r_bot->lower),
		       std::max(r_top->upper, r_bot->upper));  // 0-face
	      r_top += 2;
	      r_bot += 2;
	    }
	}

      return K;
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


    template <typename T>
    image2d<T>
    area_filter(const image2d<unsigned>& parent,
		const std::vector<unsigned>& S,
		const image2d<T>& K_,
		const image2d<unsigned>& area,
		unsigned lambda)
    {
      image2d<T> out(K_.domain());
      for (int i = 0; i < S.size(); ++i)
	{
	  unsigned p = S[i], q = parent.element(p);
	  if (q == p || K_.element(q) != K_.element(p))
	    {
	      // node
	      if (area.element(p) >= lambda)
		out.element(p) = K_.element(p);
	      else
		out.element(p) = out.element(q);
	    }
	  else
	    out.element(p) = out.element(q);
	}

      return out;
    }


//     inline
//     bool
//     is_2_face(const image2d<& b, unsigned p)
//     {
//       point2d p_ = 
//     }


    template <typename T>
    image2d<T>
    only_2_faces(const image2d<T>& F)
    {
      image2d<T> out(F.nrows() / 2, F.ncols() / 2);
      mln_piter(box2d) p(out.domain());
      for_all(p)
	out(p) = F.at_(2 * p.row() + 1, 2 * p.col() + 1);
      return out;
    }


    template <typename T>
    image2d<T>
    only_2_faces_max(const image2d<T>& F)
    {
      image2d<T> out(F.nrows() / 2, F.ncols() / 2);
      mln_piter(box2d) p(out.domain());
      for_all(p)
      {
	point2d p_(2 * p.row() + 1, 2 * p.col() + 1), n;
	T max = F(p_);
	n = p_ + dpoint2d(-1,0);
	if (F(n) > max) max = F(n);
	n = p_ + dpoint2d(+1,0);
	if (F(n) > max) max = F(n);
	n = p_ + dpoint2d(0,-1);
	if (F(n) > max) max = F(n);
	n = p_ + dpoint2d(0,+1);
	if (F(n) > max) max = F(n);
	out(p) = max;
      }
      return out;
    }


    // -----------------------------------------   tree computation



    template <typename T, typename N>
    image2d<T>
    compute_autodual_tree(const image2d<T>& f, const N& nbh, unsigned lambda)
    {
      typedef utils::range<T> R;
      typedef unsigned P;
      const unsigned n_nbhs = nbh.size();

      util::timer t, tt;
      tt.start();


      // Initialisation.
      // ---------------------------------------------

      t.start();

      // K and K_

      image2d<R> K = immerse_and_interpolate(f);
      const box2d b = K.domain();
      util::array<int> dp = offsets_wrt(K, nbh);
      image2d<T> K_(b);

      // S

      std::vector<P> S;
      S.reserve(b.nsites());

      // deja-vu is aux data

      image2d<bool> deja_vu(b);
      data::fill(deja_vu, false);
      extension::fill(deja_vu, true);

      // l_start

      bool start_up;
      T l_start = utils::frontiere_median(f, start_up);
      std::cout << "l_start = " << l_start << std::endl;

      // q

      utils::dh_queue<P,T> q;
      q.init(l_start, start_up);
      push_frontiere_at_q_level(q, deja_vu);


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
	  K_.element(p) = q.level();
	    
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
      data::fill(area, 0);

      for (int i = S.size() - 1; i >= 0; --i)
	{
	  p = S[i];

	  parent.element(p) = p;
	  zpar.element(p) = p;
	  last.element(p) = i; // we keep track of the smallest index!

	  {
	    point2d p__ = area.point_at_index(p);
	    if (p__.row() % 2 && p__.col() % 2)
	      area.element(p) = 1; // only for 2-faces :-)
	  }

	  for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      unsigned n = p + dp[j];

	      if (zpar.element(n) == 0) // not deja-vu so skip
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
	  if (K_.element(r) == K_.element(q))
	    parent.element(p) = r;
	}

      t.stop();
      std::cout << "  canonization: " << t << "s" << std::endl;


      //  Log.
      // ---------------------------------------------

      tt.stop();
      std::cout << "total: " << tt << "s" << std::endl;

      print_tree_stats(parent, S, K_);

      image2d<T> g = area_filter(parent, S, K_, area, lambda);
      return g;
    }


  } // end of namespace mln::k4

} // end of namespace mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda output.pgm" << std::endl
	    << "   c4 with 0- and 1-faces" << std::endl
	    << "   WARNING: lambda for both 2-faces and others!!!" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  if (argc != 4)
    usage(argv);

  using namespace mln;
  using namespace mln::k4;

  using value::int_u8;
  typedef utils::range<int_u8> R;

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);
  unsigned lambda = std::atoi(argv[2]);

  image2d<int_u8> g = compute_autodual_tree(f, c4(), lambda);

  io::pgm::save(g, "temp_g.pgm");
  io::pgm::save(only_2_faces(g), argv[3]);

  io::pgm::save(only_2_faces_max(g), "temp_g_max.pgm");
}
