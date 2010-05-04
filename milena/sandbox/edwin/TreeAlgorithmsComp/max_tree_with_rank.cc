#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/dump/save.hh>

#include <mln/data/sort_offsets.hh>
#include <mln/extension/adjust.hh>
#include <mln/extension/fill.hh>

#include <mln/util/timer.hh>
#include <mln/debug/println.hh>



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
    const unsigned n = 256;

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
		     bool run_tree_canonization = false)
  {
    const unsigned npixels = f.domain().nsites();

    extension::adjust(f, nbh);
    extension::fill(f, 255);

    std::vector<unsigned> s;
    s.reserve(npixels);
    sort_decreasing(f, s);


    util::array<int> dp = offsets_wrt(f, nbh);
    const unsigned n_nbhs = dp.nelements();

    mln_ch_value(I, unsigned) parent, zpar, rnk, last;

    // initialization.
    initialize(parent, f);
    initialize(rnk, f);
    data::fill(rnk, 0);
    initialize(zpar, f);
    data::fill(zpar, 0);
    initialize(last, f);


    // Tree construction.
    for (unsigned i = 0; i < npixels; ++i)
      {
	unsigned p = s[i];
	    
	parent.element(p) = p;
 	zpar.element(p) = p;
 	last.element(p) = i;

	for (unsigned j = 0; j < n_nbhs; ++j)
	  {
	    unsigned n = p + dp[j];
		
	    if (zpar.element(n) == 0) // not deja-vu
	      continue;
		
	    unsigned r_ = zfind_root(zpar, n);
	    unsigned p_ = zfind_root(zpar, p);
	    if (r_ != p_)
	      {
		unsigned r = s[last.element(r_)];
		parent.element(r) = p;

		// Parenthood goes towards root, e.g., to lower levels:
		mln_invariant(f.element(p) <= f.element(r));

		// Link.
		if (rnk.element(r_) <= rnk.element(p_))
		  {
		    zpar.element(r_) = p_;
		    ++rnk.element(p_);
		    if (last.element(r_) > last.element(p_))
		      last.element(p_) = last.element(r_);
		  }
		else
		  {
		    zpar.element(p_) = r_;
		    ++rnk.element(r_);
		    if (last.element(p_) > last.element(r_))
		      last.element(r_) = last.element(p_);
		  }
	      }

	  } // j

      } // i


//     if (run_tree_canonization)
//       // Tree canonization.
//       for (int i = npixels - 1; i >= 0; --i)
// 	{
// 	  unsigned
// 	    p = s[i],
// 	    q = parent.element(p);
// 	  if (f.element(parent.element(q)) == f.element(q))
// 	    parent.element(p) = parent.element(q);
// 	}

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

  util::timer t;
  t.start();

  // go go go
  image2d<unsigned> parent = compute_max_tree(f, nbh);

  float ts = t.stop();
  std::cout << ts * 1000 << std::endl;

  std::cout << "nnodes = " << nnodes(f, parent) << std::endl;
}
