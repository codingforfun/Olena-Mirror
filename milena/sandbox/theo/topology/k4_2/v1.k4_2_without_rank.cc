
#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/debug/println.hh>
#include <mln/data/stretch.hh>
#include <mln/extension/fill.hh>

#include <mln/core/image/dmorph/image_if.hh>

#include <mln/util/timer.hh>

#include "../utils/dh_queue_vf.hh"
#include "../utils/frontiere_vf.hh"




namespace mln
{

  namespace k4
  {


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
	    if (parent.element(p) == p ||
		K_.element(parent.element(p)) != K_.element(p))
	      ++nnodes;
	  }
	std::cout << "nnodes = " << nnodes << std::endl;
      }
    }


    template <typename T>
    image2d<unsigned>
    unwrap_tree(const image2d<unsigned>& parent,
		const std::vector<unsigned>& S,
		const image2d<T>& f,
		T l_start)
    {
      image2d<unsigned> output(f.domain());

      unsigned p = S[0];
      output.element(p) = l_start;

      for (int i = 1; i < S.size(); ++i)
	{
	  p = S[i];
	  unsigned q = parent.element(p);
	  unsigned delta = std::abs(int(f.element(q)) - int(f.element(p)));
	  output.element(p) = output.element(q) + delta;
	}

      return output;
    }



    template <typename T>
    image2d<T>
    area_filter(const image2d<unsigned>& parent,
		const std::vector<unsigned>& S,
		const image2d<T>& f,
		const image2d<unsigned>& area,
		unsigned lambda)
    {
      image2d<T> out(f.domain());
      for (int i = 0; i < S.size(); ++i)
	{
	  unsigned p = S[i], q = parent.element(p);
	  if (q == p || f.element(q) != f.element(p))
	    {
	      if (area.element(p) >= area.element(q))
		std::cout << '-';

	      // node
	      if (area.element(p) >= lambda)
		out.element(p) = f.element(p);
	      else
		out.element(p) = out.element(q);
	    }
	  else
	    out.element(p) = out.element(q);
	}

      std::cout << std::endl;

      return out;
    }



    template <typename T>
    image2d<unsigned>
    depict_area(const image2d<unsigned>& parent,
		const std::vector<unsigned>& S,
		const image2d<T>& f,
		const image2d<unsigned>& area)
    {
      image2d<unsigned> out(f.domain());
      for (int i = 0; i < S.size(); ++i)
	{
	  unsigned p = S[i], q = parent.element(p);
	  if (q == p || f.element(q) != f.element(p))
	    out.element(p) = area.element(p);
	  else
	    out.element(p) = out.element(q);
	}
      return out;
    }


    // -----------------------------------------   tree computation



    template <typename T, typename N>
    void
    compute_autodual_tree(image2d<T>& K, const N& nbh, unsigned lambda)
    {
      typedef unsigned P;
      
      const box2d b = K.domain();

      util::array<int> dp = offsets_wrt(K, nbh);
      const unsigned n_nbhs = dp.nelements();

      util::timer t;


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

      // l_start

      bool start_up;
      T l_start = utils::frontiere_median(K, start_up);
      std::cout << "l_start = " << l_start << std::endl;

      // q

      utils::dh_queue_vf<P,T> q;
      q.init(l_start, start_up);
      utils::push_frontiere(K, q, deja_vu);

      t.stop();
      std::cout << "  initialization: " << t << "s" << std::endl;



# ifndef NDEBUG
      image2d<unsigned> ordering(b);
      unsigned index = 0;
# endif


      //  1st pass: ordering.
      // ---------------------------------------------

      t.start();

      P p;
      while (! q.is_empty())
	{
	  p = q.pop();
	  S.push_back(p);

# ifndef NDEBUG
	  ordering.element(p) = ++index;
#endif

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



# ifndef NDEBUG
      io::pgm::save(data::stretch(value::int_u8(), ordering), "temp_ord.pgm");
#endif


      //  2nd pass: rough tree.
      // ---------------------------------------------

      t.start();

      image2d<P> parent(b);

      // data for compression and union by rank:
      image2d<P> zpar(b);
      data::fill(zpar, 0);

      // attribute
      image2d<unsigned> area(b);

      for (int i = S.size() - 1; i >= 0; --i)
	{
	  p = S[i];

	  parent.element(p) = p;
	  zpar.element(p) = p;
	  area.element(p) = 1;

	  for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      unsigned n = p + dp[j];

	      if (zpar.element(n) == 0)
		continue;

	      P r = zfind_root(zpar, n);
	      if (r != p)
		{
		  parent.element(r) = p;
		  zpar.element(r) = p;
		  area.element(p) += area.element(r);
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

      image2d<unsigned> out = unwrap_tree(parent, S, K, l_start);
      io::pgm::save(data::stretch(value::int_u8(), out),
		    "temp_out.pgm");

      io::pgm::save(data::stretch(value::int_u8(),
				  depict_area(parent, S, K, area)),
		    "temp_a.pgm");

      io::pgm::save(area_filter(parent, S, K, area, lambda),
		    "temp_g.pgm");
    }




  } // end of namespace mln::k4

} // end of namespace mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda" << std::endl;
  std::cerr << "   c4 with 2-faces only!" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  if (argc != 3)
    usage(argv);

  using namespace mln;
  using namespace mln::k4;

  image2d<value::int_u8> f;
  io::pgm::load(f, argv[1]);
  unsigned lambda = std::atoi(argv[2]);

  util::timer t;
  t.start();

  compute_autodual_tree(f, c8(), lambda);

  t.stop();
  std::cout << "tree computation: " << t << "s" << std::endl;
}
