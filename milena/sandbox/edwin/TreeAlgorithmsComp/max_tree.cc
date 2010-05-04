#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>

#include <mln/data/sort_offsets.hh>
#include <mln/extension/adjust.hh>
#include <mln/extension/fill.hh>
#include <mln/debug/println.hh>
#include <mln/util/timer.hh>

static mln::util::timer tm;
static const int ITERATION = 4;


#define START_BENCH()				\
  tm.restart();					\
  for (int i = 0; i < ITERATION; ++i) {

#define END_BENCH()						\
  }								\
    std::cout << ((float)(tm.stop() * 1000.0) / ITERATION) << std::endl;


namespace mln
{


  template <typename Z>
  inline
  unsigned zfind_root(Z& zpar, unsigned x)
  {
    mlc_equal(mln_value(Z), unsigned)::check();
    if (zpar.element(x) == x)
      return x;
    else
      return zpar.element(x) = zfind_root(zpar, zpar.element(x));
  }



//   template <typename I>
//   void
//   radix_decreasing_sort(const I& f, util::array<unsigned>& loc, util::array<unsigned>& s)
//   {
//     typedef mln_value(I) T;
//     const unsigned n = unsigned(mln_max(T)) + 1;

//     // h
//     util::array<unsigned> h(n, 0);
//     mln_fwd_pixter(const I) pxl(f);
//     for_all(pxl)
//       ++h[pxl.val()];

//     // preparing output data
//     util::array<unsigned> loc(n);
//     loc[n-1] = 0;
//     for (int i = n - 2; i >= 0; --i)
//       loc[i] = loc[i+1] + h[i+1];
//     // computing output data
//     s.resize(f.domain().nsites());
//     for_all(pxl)
//       s[ loc[pxl.val()]++ ] = pxl.offset();
//   }



  template <typename I>
  void
  radix_decreasing_sort(const I& f, util::array< util::array<unsigned> >& s)
  {
    typedef mln_value(I) T;
    const unsigned n = unsigned(mln_max(T)) + 1;
    s.resize(n);

    // h
    util::array<unsigned> h(n, 0);
    mln_fwd_pixter(const I) pxl(f);
    for_all(pxl)
      ++h[pxl.val()];

    for (unsigned v = 0; v < n; ++v)
      s[v].reserve(h[v]);

    // computing output data
    for_all(pxl)
      s[pxl.val()].append(pxl.offset());
  }



  template <typename I, typename N>
  mln_ch_value(I, unsigned)
    compute_max_tree(const I& f, const N& nbh,
		     bool run_compression_finalization = true,
		     bool run_tree_canonization        = true)
  {
    typedef mln_value(I) T;
    const unsigned n_values = unsigned(mln_max(T)) + 1;

    extension::adjust(f, nbh);
    extension::fill(f, mln_max(T));

    util::array< util::array<unsigned> > s;
    radix_decreasing_sort(f, s); // so a max-tree

    util::array<int> dp = offsets_wrt(f, nbh);
    const unsigned n_nbhs = dp.nelements();

    mln_ch_value(I, unsigned) parent, zpar;

    // initialization.
    initialize(parent, f);
    initialize(zpar, f);
    data::fill(zpar, 0);

    // Tree construction.
    for (int v = n_values - 1; v >= 0; --v)
      {

	const util::array<unsigned>& s_v = s[v];
	unsigned n_p = s_v.nelements();
	for (unsigned i = 0; i < n_p; ++i)
	  {
	    unsigned p = s_v[i];
	    
	    parent.element(p) = p;
	    zpar.element(p) = p;
	    
	    for (unsigned j = 0; j < n_nbhs; ++j)
	      {
		unsigned n = p + dp[j];
		
		if (zpar.element(n) == 0) // not deja-vu
		  continue;
		
		unsigned r = zfind_root(zpar, n);
		if (r != p)
		  {
		    parent.element(r) = p;
		    zpar.element(r) = p;
		  }
	      }
	  }

	if (run_compression_finalization)
	  // Finalizing compression per gray-level.
	  for (int i = n_p - 1; i >= 0; --i)
	    {
	      unsigned p = s_v[i];
	      zpar.element(p) = zpar.element(zpar.element(p));
	    }

      }


    if (run_tree_canonization)
      // Tree canonization.
      for (int v = 0; v < n_values; ++v)
	{
	  const util::array<unsigned>& s_v = s[v];
	  unsigned n_p = s_v.nelements();
	  for (int i = n_p - 1; i >= 0; --i)
	    {
	      unsigned
		p = s_v[i],
		q = parent.element(p);
	      if (f.element(parent.element(q)) == f.element(q))
		parent.element(p) = parent.element(q);
	    }
	}

    return parent;
  }


  template <typename I, typename J>
  unsigned nnodes(const I& f, const J& parent)
  {
    mlc_equal(mln_value(J), unsigned)::check();
    unsigned n = 0;
    mln_piter(I) p(parent.domain());
    for_all(p)
    {
      unsigned o = parent.index_of_point(p);
      if (parent(p) == o || f.element(parent(p)) != f(p))
	++n;
    }
    return n;
  }


} // mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm" << std::endl;
  std::cerr << "  max-tree with full optimizations" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv);

  using namespace mln;

  neighb2d nbh = c4();

  image2d<value::int_u8> f;
  io::pgm::load(f, argv[1]);

  // go go go
  image2d<unsigned> parent = compute_max_tree(f, nbh);

  tm.start();

  START_BENCH();
  compute_max_tree(f, nbh);
  END_BENCH();

  //  debug::println(parent);
  std::cout << "nnodes = " << nnodes(f, parent) << std::endl;
}
