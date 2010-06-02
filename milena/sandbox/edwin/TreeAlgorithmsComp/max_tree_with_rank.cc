#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/int_u16.hh>

#include <mln/io/pgm/load.hh>

#include <mln/data/sort_offsets.hh>
#include <mln/extension/adjust.hh>
#include <mln/extension/fill.hh>

#include "bench.hh"

#ifndef Q
# define Q int_u8
#endif

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



  template <typename I>
  void
  sort_decreasing(const I& input, std::vector<unsigned>& vec)
  {
    const unsigned n = mln_card(mln_value(I));

    // h
    std::vector<unsigned> h(n, 0);
    mln_fwd_pixter(const I) pxl(input);
    for_all(pxl)
      ++h[pxl.val()];
    
    // preparing output data
    std::vector<unsigned> loc(n);
    loc[n-1] = 0;
    for (int i = n - 2; i >= 0; --i)
      loc[i] = loc[i+1] + h[i+1];

    // computing output data
    for_all(pxl)
      vec[loc[pxl.val()]++] = pxl.offset();
  }



  template <typename I, typename N>
  mln_ch_value(I, unsigned)
    compute_max_tree(const I& f, const N& nbh,
		     bool run_tree_canonization = true)
  {
    const unsigned npixels = f.domain().nsites();

    extension::adjust(f, nbh);
    extension::fill(f, 255);

    std::vector<unsigned> s;
    s.reserve(npixels);
    sort_decreasing(f, s);


    util::array<int> dp = offsets_wrt(f, nbh);
    const unsigned n_nbhs = dp.nelements();

    mln_ch_value(I, unsigned) parent, zpar, rnk, zpar_to_par;

    // initialization.
    initialize(parent, f);
    initialize(rnk, f);
    initialize(zpar, f);
    initialize(zpar_to_par, f);
    data::fill(rnk, 0);
    data::fill(zpar, 0);

    // Tree construction.
    for (unsigned i = 0; i < npixels; ++i)
      {
	unsigned p = s[i];

	parent.element(p) = p;
 	zpar.element(p) = p;
	zpar_to_par.element(p) = p;
 	//last.element(p) = i;

	for (unsigned j = 0; j < n_nbhs; ++j)
	  {
	    unsigned n = p + dp[j];

	    if (zpar.element(n) == 0) // not deja-vu
	      continue;

	    unsigned r = zfind_root(zpar, n);
	    unsigned q = zfind_root(zpar, p);
	    if (r != q)
	      {
		unsigned r_ = zpar_to_par.element(r);
		parent.element(r_) = p;

		// Parenthood goes towards root, e.g., to lower levels:
		// mln_invariant(f.element(p) <= f.element(r));
		if (rnk.element(q) < rnk.element(r)) {
		  zpar.element(q) = r;
		  zpar_to_par.element(r) = p;
		} else if (rnk.element(q) > rnk.element(r)) {
		  zpar.element(r) = q;
		  zpar_to_par.element(q) = p;
		} else {
		  zpar.element(r) = q;
		  zpar_to_par.element(q) = p;
		  ++rnk.element(q);
		}

		// // Link.
		// if (rnk.element(r_) <= rnk.element(p_))
		//   {
		//     zpar.element(r_) = p_;
		//     ++rnk.element(p_);
		//     if (last.element(r_) > last.element(p_))
		//       last.element(p_) = last.element(r_);
		//   }
		// else
		//   {
		//     zpar.element(p_) = r_;
		//     ++rnk.element(r_);
		//     if (last.element(p_) > last.element(r_))
		//       last.element(r_) = last.element(p_);
		//   }
	      }

	  } // j

      } // i


    if (run_tree_canonization)
      // Tree canonization.
      for (int i = npixels - 1; i >= 0; --i)
	{
	  unsigned
	    p = s[i],
	    q = parent.element(p);
	  if (f.element(parent.element(q)) == f.element(q))
	    parent.element(p) = parent.element(q);
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
  std::cerr << "usage: " << argv[0] << " input.pgm nBench" << std::endl;
  std::cerr << "  max-tree with full optimizations" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  if (argc != 3)
    usage(argv);

  using namespace mln;
  using value::int_u8;
  using value::int_u12;
  using value::int_u16;
  typedef Q V;

  neighb2d nbh = c4();

  image2d<V> f;
  io::pgm::load(f, argv[1]);

  image2d<unsigned> parent = compute_max_tree(f, nbh);
  std::cout << "nnodes = " << nnodes(f, parent) << std::endl;

  int nb_bench = atoi(argv[2]);
  START_BENCH(nb_bench);
  parent = compute_max_tree(f, nbh);
  END_BENCH("Max Tree with Rank: ");
}
