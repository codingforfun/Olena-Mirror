
#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/debug/println.hh>
#include <mln/data/stretch.hh>
#include <mln/extension/fill.hh>
#include <mln/extension/adjust.hh>

#include <mln/util/timer.hh>

#include "../utils/dh_queue.hh"
#include "../utils/frontiere_vf.hh"
#include "../utils/immerse_vf.hh"




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

      // root
      {
	unsigned p = S[0];
	out.element(p) = f.element(p);
      }

      for (int i = 1; i < S.size(); ++i)
	{
	  unsigned p = S[i], q = parent.element(p);
	  // there is no another root:
	  mln_invariant(q != p);

	  if (f.element(q) != f.element(p)) // p is a node
	    {
	      // area is growing:
	      mln_invariant(area.element(q) > area.element(p));
	      
	      if (area.element(p) >= lambda)
		out.element(p) = f.element(p);
	      else
		out.element(p) = out.element(q);
	    }
	  else // same as its node
	    {
	      // q is a node:
	      mln_invariant(parent.element(q) == q || f.element(parent.element(q)) != f.element(q));

	      out.element(p) = out.element(q);
	    }
	}

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
    image2d<T>
    compute_autodual_tree(image2d<T>& f, const N& nbh, unsigned lambda)
    {
      typedef utils::range<T> R;
      typedef unsigned P;

      extension::adjust(f, nbh);
      const box2d b = f.domain();

      util::timer t, tt;
      tt.start();


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
      T l_start = utils::frontiere_median(f, start_up);
      extension::fill(f, l_start); // Required to avoid small border extrema.
      std::cout << "l_start = " << l_start << std::endl;

      // K

      image2d<R> K = utils::immerse_vf__dyn(f, nbh);
      image2d<T> K_(b);
# ifndef NDEBUG
      data::fill(K_, 0);
# endif
      
      util::array<int> dp = offsets_wrt(f, nbh);
      const unsigned n_nbhs = dp.nelements();

      // q

      utils::dh_queue<P,T> q;
      q.init(l_start, start_up);
      utils::push_frontiere(K, q, deja_vu);

      t.stop();
      std::cout << "  initialization: " << t << "s" << std::endl;



# ifndef NDEBUG
      debug::println("K", K);
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
	  K_.element(p) = q.level();

# ifndef NDEBUG
	  debug::println(K_);
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
//       debug::println("K", K);
//       debug::println("K_", K_);
      io::pgm::save(K_, "temp_K_.pgm");
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

//       image2d<unsigned> out = unwrap_tree(parent, S, K, l_start);
//       io::pgm::save(data::stretch(value::int_u8(), out),
// 		    "temp_out.pgm");

//       io::pgm::save(data::stretch(value::int_u8(),
// 				  depict_area(parent, S, K, area)),
// 		    "temp_a.pgm");

//       io::pgm::save(area_filter(parent, S, f, area, lambda),
// 		    "temp_gf.pgm");

      image2d<T> g = area_filter(parent, S, K_, area, lambda);
      return g;
    }



  } // end of namespace mln::k4

} // end of namespace mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda output.pgm" << std::endl
	    << "   2-faces only" << std::endl
	    << "   without rnk" << std::endl
	    << "   K is [min(d(p,ni)),max(d(p,ni))] on c4 !" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  if (argc != 4)
    usage(argv);

  using namespace mln;
  using namespace mln::k4;

  typedef value::int_u8 T;
  typedef utils::range<T> R;

  image2d<T> f, g;
  io::pgm::load(f, argv[1]);
  unsigned lambda = std::atoi(argv[2]);

  g = compute_autodual_tree(f, c4(), lambda);

  io::pgm::save(g, argv[3]);
}
