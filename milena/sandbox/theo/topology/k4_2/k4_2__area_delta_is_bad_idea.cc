
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



    // -----------------------------------------   area


    template <typename T, typename A>
    image2d<T>
    filter(const image2d<unsigned>& parent,
	   const std::vector<unsigned>& S,
	   const image2d<T>& f,
	   const image2d<A>& a,
	   A lambda)
    {
      image2d<T> g(f.domain());

      // root
      {
	unsigned p = S[0];
	g.element(p) = f.element(p);
      }

      for (int i = 1; i < S.size(); ++i)
	{
	  unsigned p = S[i], q = parent.element(p);
	  // there is no another root:
	  mln_invariant(q != p);

	  if (f.element(q) != f.element(p)) // p is a node
	    {
	      // a is growing:
	      mln_invariant(a.element(q) > a.element(p));
	      
	      if (a.element(p) >= lambda)
		g.element(p) = f.element(p);
	      else
		g.element(p) = g.element(q);
	    }
	  else // same as its node
	    {
	      // q is a node:
	      mln_invariant(parent.element(q) == q || f.element(parent.element(q)) != f.element(q));

	      g.element(p) = g.element(q);
	    }
	}

      return g;
    }



    template <typename T, typename A>
    image2d<unsigned>
    back_propagate(const image2d<unsigned>& parent,
		   const std::vector<unsigned>& S,
		   const image2d<T>& f,
		   const image2d<A>& a)
    {
      image2d<A> a_(a.domain());

      unsigned p = S[0]; // root
      a_.element(p) = a.element(p);

      // other nodes:
      for (int i = 1; i < S.size(); ++i)
	{
	  unsigned p = S[i], q = parent.element(p);
	  if (f.element(q) != f.element(p))
	    a_.element(p) = a.element(p);
	  else
	    a_.element(p) = a_.element(q);
	}

      return a_;
    }


    template <typename T, typename A>
    void
    back_propagate_inplace(const image2d<unsigned>& parent,
			   const std::vector<unsigned>& S,
			   const image2d<T>& f,
			   image2d<A>& a)
    {
      for (int i = 1; i < S.size(); ++i)
	{
	  unsigned p = S[i], q = parent.element(p);
	  if (f.element(q) == f.element(p)) // p is not a node; q is, so...
	    a.element(p) = a.element(q);    // ...propagate
	}
    }


    template <typename I>
    mln_ch_value(I, value::int_u8)
      apply_log(const I& a)
    {
      mln_ch_value(I, float) log_f;
      initialize(log_f, a);
      mln_piter(I) p(a.domain());
      for_all(p)
	log_f(p) = std::log(1.f + float(a(p)));
      return data::stretch(value::int_u8(), log_f);
    }



    // -----------------------------------------   area_delta


    template <typename I>
    void
    update_plus_delta(const I& f,
		      const image2d<unsigned>& parent,
		      unsigned delta,
		      unsigned n,
		      unsigned& n_prime)
    {
      int f_n = f.element(n);
      mln_precondition(n_prime != 0);
      for (;;)
	{
	  unsigned par_n_prime = parent.element(n_prime);
	  if (std::abs(f_n - int(f.element(par_n_prime)) >= delta))
	    break; // n_prime is OK
	  if (par_n_prime == n_prime)
	    {
	      n_prime = 0;
	      break;
	    }
	  n_prime = par_n_prime;
	}
    }


    template <typename T, typename A>
    image2d<unsigned>
    compute_a_delta_plus(const image2d<unsigned>& parent,
			 const std::vector<unsigned>& S,
			 const image2d<T>& f,
			 const image2d<A>& a,
			 unsigned delta)
    {
      image2d<A> a_delta(a.domain());
      const A sup_a = a.element(S[0]) + 1; // increment root value of a 

      image2d<bool> done;
      initialize(done, f);
      data::fill(done, false);

      unsigned n_at_none = 0;

      for (int i = S.size() - 1; i > 0; --i) // from leaves to about root
	{
	  unsigned p = S[i], q = parent.element(p);

	  if (f.element(q) == f.element(p))
	    // not a node so...
	    continue;

	  if (done.element(p))
	    // already visited so...
	    continue;

	  // starting from p
	  unsigned n = p, n_prime = n;
	  do
	    {
	      if (n_prime != 0)
		update_plus_delta(f, parent, delta, n, n_prime);

	      if (n_prime == 0)
		{
		  ++n_at_none;
		  a_delta.element(n) = sup_a;
		}
	      else
		a_delta.element(n) = a.element(n_prime);
	    
	      done.element(n) = true;
	      n = parent.element(n);
	    }
	  while (! done.element(n));
	}

      back_propagate_inplace(parent, S, f, a_delta);
 
      return a_delta;
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
// 				  back_propagate(parent, S, K, area)),
// 		    "temp_a.pgm");

//       io::pgm::save(filter(parent, S, f, area, lambda),
// 		    "temp_gf.pgm");


      back_propagate_inplace(parent, S, K_, area);
      io::pgm::save(apply_log(area), "temp_log_area.pgm");


      image2d<unsigned> area_delta = compute_a_delta_plus(parent, S, K_, area, 30);
      io::pgm::save(apply_log(area_delta), "temp_log_area_delta.pgm");

      image2d<T> g = filter(parent, S, K_, area, lambda);
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
