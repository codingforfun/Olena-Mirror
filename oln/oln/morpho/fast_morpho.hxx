// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_MORPHO_FAST_MORPHO_HXX
# define OLENA_MORPHO_FAST_MORPHO_HXX

// Implementation of morphological operations from:
// M. Van Droogenbroeck and H. Talbot.
// "Fast computation of morphological operations with arbitrary
// structuring elements". Pattern Recognition Letters,
// 17(14):1451-1460, 1996.

# include <oln/utils/histogram.hh>

namespace oln {
  namespace morpho {

    namespace internal {

      // Find structuring elements to be added/removed from the histogram
      // when we move forward along each direction.
      template<class E1_, class E2_, class E3_>
      void
      find_struct_elts(const struct_elt<E1_>& _se,
		       struct_elt<E2_> _se_add[type::exact<E1_>::ret::dim],
		       struct_elt<E3_> _se_rem[type::exact<E1_>::ret::dim])
      {
	Exact_cref(E1, se);
	Exact_ptr(E2, se_add);
	Exact_ptr(E3, se_rem);
	const unsigned dim = E1::dim;

	// back[n] allows to move backward on coordinate `n'.
	DPoint(E1) back[dim];
	for (unsigned n = 0; n < dim; ++n)
	  back[n].nth(n) = -1;

	Iter(E1) dp(se);
	Iter(E1) dp_prime(se);

	for_all(dp)
	  {
	    bool add[dim];	// whether to add `dp' when moving forward
				// on coordinate `n'.
	    bool rem[dim];	// whether to remove `dp'.
	    for (unsigned n = 0; n < dim; ++n)
	      add[n] = rem[n] = true;

	    for_all(dp_prime)
	      for (unsigned n = 0; n < dim; ++n)
		{
		  if (dp_prime.cur() + back[n] == dp)
		    // DP_PRIME is already in SE: don't add it.
		    add[n] = false;

		  if (dp.cur() + back[n] == dp_prime)
		    // DP is still in SE: don't remove it.
		    rem[n] = false;
		}

	    for (unsigned n = 0; n < dim; ++n)
	      {
		if (add[n])
		  se_add[n].add(dp);
		if (rem[n])
		  se_rem[n].add(dp.cur() + back[n]);
	      }
	  }

	for (unsigned n = 0; n < dim; ++n)
	  postcondition(se_add[n].card() == se_rem[n].card());
      }


      // Update HIST by adding elements from _SE_ADD, and removing
      // those from _SE_REM.
      template<class I_, class E1_, class E2_, class H>
      inline void
      hist_update(H& hist,
		  const image<I_>& _input,
		  const Point(I_)& p,
		  const struct_elt<E1_>& _se_rem,
		  const struct_elt<E2_>& _se_add)
      {
	Exact_cref(I, input);
	{
	  Exact_cref(E1, se_rem);
	  Iter(E1) dp(se_rem);
	  for_all(dp)
	    --hist[input[p + dp]];
	}
	{
	  Exact_cref(E2, se_add);
	  Iter(E2) dp(se_add);
	  for_all(dp)
	    ++hist[input[p + dp]];
	}
      }


      // We will zigzag over the image so that only one coordinate
      // changes at each step.  The path looks as follow on
      // 2D images:
      //   -----------------.
      //   ,----------------'
      //   `----------------.
      //   ,----------------'
      //   `-----------------
      // (The algorithm below handles the n-dimensional case.)

      template<unsigned NP1,
	       unsigned Dim,
	       typename I,
	       typename S,
	       typename H,
	       typename B,
	       typename P,
	       typename O>
      struct fast_morpho_inner {
	static void
	doit(I& input, S& size, H& hist,
	     B* se_add, B* se_rem, B* se_add_back, B* se_rem_back,
	     P& p, O& output, const unsigned* dims)
	{
	  const unsigned N = *dims;

	  fast_morpho_inner<NP1 + 1, Dim,
	    I, S, H, B, P, O>::doit(input, size, hist,
				    se_add, se_rem,
				    se_add_back, se_rem_back, p,
				    output, dims + 1);
	  if (p.nth(N) == 0) {	// Go forward
	    for(++p.nth(N); p.nth(N) < size.nth(N); ++p.nth(N)) {
	      hist_update(hist, input, p, se_rem[N], se_add[N]);
	      output[p] = hist.res();
	      fast_morpho_inner<NP1 + 1, Dim,
		I, S, H, B, P, O>::doit(input, size, hist,
					se_add, se_rem,
					se_add_back,
					se_rem_back,
					p, output, dims + 1);
	    }
	    --p.nth(N);
	  } else {		// Go backward
	    for(--p.nth(N); p.nth(N) >= 0; --p.nth(N)) {
	      hist_update(hist, input, p, se_rem_back[N], se_add_back[N]);
	      output[p] = hist.res();
	      fast_morpho_inner<NP1 + 1, Dim,
		I, S, H, B, P, O>::doit(input, size, hist,
					se_add, se_rem,
					se_add_back,
					se_rem_back,
					p, output, dims + 1);
	    }
	    ++p.nth(N);
	  }
	  return;
	}
      };

      template<unsigned Dim,
	       typename I,
	       typename S,
	       typename H,
	       typename B,
	       typename P,
	       typename O>
      struct fast_morpho_inner<Dim, Dim, I, S, H, B, P, O> {
	static void
	doit(I& input, S& size, H& hist,
	     B* se_add, B* se_rem, B* se_add_back, B* se_rem_back,
	     P& p, O& output, const unsigned* dims)
	{
	  const unsigned N = *dims;

	  if (p.nth(N) == 0) { // Go forward
	    // Don't call hist_update because this would create new `dpr' and
	    // `dpa' iterators for each points.
	    Iter(B) dpr(se_rem[N]);
	    Iter(B) dpa(se_add[N]);
	    for(++p.nth(N);
		p.nth(N) < size.nth(N);
		++p.nth(N)) {
	      for_all(dpr)
		--hist[input[p + dpr]];
	      for_all(dpa)
		++hist[input[p + dpa]];
	      output[p] = hist.res();
	    }
	    --p.nth(N);
	  } else {		// Go backward
	    // Don't call hist_update because this would create new `dpr' and
	    // `dpa' iterators for each points.
	    Iter(B) dpr(se_rem_back[N]);
	    Iter(B) dpa(se_add_back[N]);
	    for(--p.nth(N);
		p.nth(N) >= 0;
		--p.nth(N)) {
	      for_all(dpr)
		--hist[input[p + dpr]];
	      for_all(dpa)
		++hist[input[p + dpa]];
	      output[p] = hist.res();
	    }
	    ++p.nth(N);
	  }
	  return;
	}
      };
    } // internal


    template<class E_>
    struct sort_dimensions
    {
      sort_dimensions(struct_elt<E_> se[type::exact<E_>::ret::dim])
	: _se(se) {}

      bool operator()(unsigned a, unsigned b)
      {
	Exact_ptr(E, se);
	return se[a].card() > se[b].card(); 
      }

    protected:
      struct_elt<E_>* _se;
    };


    template<class I_, class E_, template<typename, typename> class H>
    Concrete(I_)
    fast_morpho(const image<I_>& _input,
		const struct_elt<E_>& _se)
    {
      Exact_cref(I, input);
      Exact_cref(E, se);
      enum { dim = E::dim };

      // prepare output
      Concrete(I) output(input.size());
      border::adapt_copy(input, se.delta());

      // compute delta structuring elements for forward movements
      E se_add[dim];
      E se_rem[dim];
      internal::find_struct_elts(se, se_add, se_rem);

      // compute delta structuring elements for backward movements
      E se_add_back[dim];
      E se_rem_back[dim];
      for (unsigned n = 0; n < dim; ++n)
	for (unsigned i = 0; i < se_add[n].card(); ++i)
	{
	  DPoint(I) dp = se_add[n].dp(i);
	  dp.nth(n) += 1;
	  se_rem_back[n].add(dp);

	  dp = se_rem[n].dp(i);
	  dp.nth(n) += 1;
	  se_add_back[n].add(dp);
	}

      // Order dimensions
      unsigned dims[dim];
      for (unsigned n = 0; n < dim; ++n)
	dims[n] = n;
      sort_dimensions<E_> s(se_add);
      std::sort(dims, dims + dim, s);

      const typename I::image_size size = input.size();

      // Initialize the histogram with the values around the first point.
      H<Value(I),unsigned> hist;
      Point(I) p;

      Iter(E) dp(se);
      for_all(dp)
	++hist[input[p + dp]];

      // Process the image.
      // ------------------

      output[p] = hist.res();	// First point.

      internal::fast_morpho_inner<1, E::dim, const I,
	const typename I::image_size, H<Value(I),unsigned>,
	const E, Point(I), Concrete(I_)>::doit(input, size, hist,					       
					       se_add, se_rem,
					       se_add_back, se_rem_back,
					       p, output, dims);
      return output;
    }

  } // morpho
} // oln

#endif // OLENA_MORPHO_FAST_MORPHO_HXX
