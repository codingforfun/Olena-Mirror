// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

# include <mlc/is_a.hh>
# include <oln/utils/histogram.hh>

namespace oln {
  namespace morpho {

    /*!
    ** \brief oln::morpho::internal namespace
    ** Internal stuff.
    */
    namespace internal {

      /*!
      ** \brief Find structuring elements.
      **
      ** Find structuring elements to be added/removed from the histogram
      ** when we move forward along each direction.
      **
      ** \todo FIXME: add(dp) on w_windows associates a default weight set
      ** to 1
      */
      template<class E1, class E2, class E3>
      void
      find_struct_elts(const abstract::struct_elt<E1>& se,
		       E2 se_add[mlc::exact<E1>::ret::dim],
		       E3 se_rem[mlc::exact<E1>::ret::dim])
      {
	mlc_is_a(E2, abstract::struct_elt)::ensure();
	mlc_is_a(E3, abstract::struct_elt)::ensure();
	const unsigned dim = E1::dim;

	// back[n] allows to move backward on coordinate `n'.
	oln_dpoint_type(E1) back[dim];
	for (unsigned n = 0; n < dim; ++n)
	  back[n].nth(n) = -1;

	oln_iter_type(E1) dp(se);
	oln_iter_type(E1) dp_prime(se);

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
		  {
		    se_add[n].add(dp);
		  }
		if (rem[n])
		  {
		    se_rem[n].add(dp.cur() + back[n]);
		  }
	      }
	  }

	for (unsigned n = 0; n < dim; ++n)
	  postcondition(se_add[n].card() == se_rem[n].card());
      }

      /*!
      ** \brief Update an histogram.
      **
      ** Update HIST by adding elements from _SE_ADD, and removing
      ** those from _SE_REM.
      */
      template<class I, class E1, class E2, class H>
      // inline
      void
      hist_update(H& hist,
		  const abstract::non_vectorial_image<I>& input,
		  const oln_point_type(I)& p,
		  const abstract::struct_elt<E1>& se_rem,
		  const abstract::struct_elt<E2>& se_add)
      {
	{
	  oln_iter_type(E1) dp(se_rem);
	  for_all(dp)
	    --hist[input[p + dp]];
	}
	{
	  oln_iter_type(E2) dp(se_add);
	  for_all(dp)
	    ++hist[input[p + dp]];
	}
      }


      /*!
      ** We will zigzag over the image so that only one coordinate
      ** changes at each step.  The path looks as follow on
      ** 2D images:\n
      ** --------------\
      **               |
      ** /-------------/
      ** |
      ** \-------------\
      **               |
      ** -------------/
      ** \n
      ** (The algorithm below handles the n-dimensional case.)
      */
      template<unsigned NP1,
	       unsigned Dim,
	       typename I,
	       typename S,
	       typename H,
	       typename B,
	       typename P,
	       typename O>
      struct fast_morpho_inner {

	/*!
	** Perform the action.
	*/
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
	   oln_iter_type(B) dpr(se_rem[N]);
	   oln_iter_type(B) dpa(se_add[N]);
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
	   oln_iter_type(B) dpr(se_rem_back[N]);
	   oln_iter_type(B) dpa(se_add_back[N]);
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

    /*!
    ** \brief functor to sort dimensions.
    */
    template<class E>
    struct sort_dimensions
    {
      /*!
      ** \brief Constructor.
      */
      sort_dimensions(abstract::struct_elt<E> se[mlc::exact<E>::ret::dim])
	: se_(se) {}

      /*!
      ** \brief Parenthesis operator.
      **
      ** Call it to use the functor.
      */
      bool operator()(unsigned a, unsigned b)
      {
	return se_[a].card() > se_[b].card();
      }

    protected:
      abstract::struct_elt<E>* se_; ///< Structural element.
    };

    /*!
    ** \brief Fast morpho algorithm.
    **
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/morpho/erosion.hh>
    ** #include <oln/level/compare.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   typedef oln::image2d<ntg::bin>	im_type;
    **
    **   im_type im1(oln::load(IMG_IN "object.pbm"));
    **   //save(oln::morpho::fast_morpho<im_type, oln::win_c8p(), utils::histogram_min>
    **   //            (im1, oln::win_c8p()), IMG_OUT "oln_morpho_fast_morpho.pbm");
    **   save(im1, IMG_OUT "oln_morpho_fast_morpho.pbm");
    **   return  0;
    ** }
    ** \endcode
    **
    ** \image html object_pbm.png
    ** \image latex object_pbm.png
    ** =>
    ** \image html oln_morpho_fast_morpho.png
    ** \image latex oln_morpho_fast_morpho.png
    ** \todo FIXME: Correct this function and make the example use it.
    */
    template<class I, class E, template<typename, typename> class H>
    oln_concrete_type(I)
      fast_morpho(const abstract::non_vectorial_image<I>& input,
		  const abstract::struct_elt<E>& se)
    {
      enum { dim = E::dim };

      // prepare output
      oln_concrete_type(I) output(input.size());
      input.border_adapt_copy(se.delta());

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
	  oln_dpoint_type(I) dp = se_add[n].dp(i);
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
      sort_dimensions<E> s(se_add);
      std::sort(dims, dims + dim, s);

      const typename I::size_type size = input.size();

      // Initialize the histogram with the values around the first point.
      H<oln_value_type(I),unsigned> hist;
      oln_point_type(I) p;

      //     oln_iter_type(E) dp(se);
      typename E::iter_type	dp(se);
      for_all(dp)
	++hist[input[p + dp]];

      // Process the image.
      // ------------------

      output[p] = hist.res();	// First point.

      internal::fast_morpho_inner<1, E::dim, const I,
	const typename I::size_type, H<oln_value_type(I),unsigned>,
	const E,oln_point_type(I),oln_concrete_type(I)>::doit(input.exact(), size, hist,
							      se_add, se_rem,
							      se_add_back, se_rem_back,
							      p, output, dims);
      return output;
    }

  } // morpho
} // oln

#endif // OLENA_MORPHO_FAST_MORPHO_HXX
