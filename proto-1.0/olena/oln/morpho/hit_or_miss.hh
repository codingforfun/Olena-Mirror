// Copyright (C) 2001, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_HIT_OR_MISS_HH
# define OLENA_MORPHO_HIT_OR_MISS_HH

# include <mlc/cmp.hh>

# include <oln/utils/record.hh>
# include <oln/morpho/erosion.hh>
# include <oln/morpho/dilation.hh>
# include <oln/level/arith.hh>
# include <oln/level/invert.hh>

// FIXME: More documentation (in particular, code snippets).  See Olena
// 0.10's doc.

namespace oln {

  namespace morpho {

    namespace impl {

      /// Hit-or-miss transform using regular windows, checking that their
      /// intersection is empty.
      template<class I, class G, class W1, class W2>
      oln_type_of(I, concrete)
      hit_or_miss_(const abstract::image<I>& input,
		   const abstract::regular_window<G, W1>& win1,
		   const abstract::regular_window<G, W2>& win2)
      {
	precondition(inter(win1.exact(), win2.exact()).card() == 0);
	entering("->generic_on_regular_window");
	registering(input, "input");

	oln_type_of(I, concrete) output(input.size(), "output");
	output = level::min(erosion(input, win1),
			    erosion(level::invert(input), win2));

	exiting("->generic_on_regular_window");
	return output;
      }

      /// Hit-or-miss transform using non-regular windows.
      template<class I, class W1, class W2>
      oln_type_of(I, concrete)
      hit_or_miss_(const abstract::image<I>& input,
		   const abstract::window<W1>& win1,
		   const abstract::window<W2>& win2)
      {
	entering("->generic_on_non_regular_window");
	registering(input, "input");

	oln_type_of(I, concrete) output(input.size(), "output");
	output = level::min(erosion(input, win1),
			    erosion(level::invert(input), win2));

	exiting("->generic_on_non_regular_window");
	return output;
      }

    } // end of namespace oln::morpho::impl


    /*! \brief Preform a hit-or-miss transform (generic facade)

	REF: Soille, 2nd ed., p.140.

        \arg \c I  exact type of the input image.
	\arg \c W1 exact type of the first structuring element.
	\arg \c W2 exact type of the second structuring element.

	\param input image to process.
	\param win1  first structuring element.
	\param win2  second structuring element.  */
    template<class I, class W1, class W2>
    oln_type_of(I, concrete)
    hit_or_miss(const abstract::image<I>& input,
		const abstract::window<W1>& win1,
		const abstract::window<W2>& win2)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W1, grid)>::ensure();
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W2, grid)>::ensure();
      entering("morpho::hit_or_miss");

      oln_type_of(I, concrete) output("output");
      output = impl::hit_or_miss_(input, win1.exact(), win2.exact());

      exiting("morpho::hit_or_miss");
      return output;
    }


    /*! \brief Perform an hit-or-miss opening.

        Compute the hit-or-miss opening of \a input by the composite
	structuring element (win1, win2).
    
	REF: Soille, 2nd ed., p.149.

	By definition \a win1 and \a win2 must have the same origin,
	and need to be disjoint.  This algorithm has been extended to
	every data types (althought it is not increasing).  Beware the
	result depends upon the image data type if it is not binary.

	\arg \c I  Exact type of the input image.
	\arg \c W1 Exact type of the first structuring element.
	\arg \c W2 Exact type of the second structuring element.

	\param input image to process.
	\param win1  first structuring element.
	\param win2  second structuring element.  */
    template<class I, class W1, class W2>
    oln_type_of(I, concrete)
    hit_or_miss_opening(const abstract::image<I>& input,
			const abstract::window<W1>& win1,
			const abstract::window<W2>& win2)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W1, grid)>::ensure();
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W2, grid)>::ensure();
      entering("morpho::hit_or_miss_opening");

      oln_type_of(I, concrete) output("output");
      output = dilation(hit_or_miss(input, win1, win2), -win1);

      exiting("morpho::hit_or_miss_opening");
      return output;
    }

    /*! \brief Perform an hit-or-miss opening of background.

        Compute the hit-or-miss opening of the background of \a input by
	the composite structuring element (win1, win2).

	REF: Soille, 2nd ed., p.149.

	By definition win1 and win2 must have the same origin, and
	need to be disjoint.  This algorithm has been extended to
	every data types (although it is not increasing).  Beware the
	result depends upon the image data type if it is not bin.
 
	\arg \c I  exact type of the input image.
	\arg \c W1 exact type of the first structuring element.
	\arg \c W2 exact type of the second structuring element.
 
	\param input image to process.
	\param win1  first structuring element.
	\param win2  second structuring element.  */
    template<class I, class W1, class W2>
    oln_type_of(I, concrete)
    hit_or_miss_opening_bg(const abstract::image<I>& input,
			   const abstract::window<W1>& win1,
			   const abstract::window<W2>& win2)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W1, grid)>::ensure();
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W2, grid)>::ensure();
      entering("morpho::hit_or_miss_opening_bg");

      oln_type_of(I, concrete) output("output");
      output = dilation(hit_or_miss(input, win1, win2), -win2);

      exiting("morpho::hit_or_miss_opening_bg");
      return output;
    }

    /*! \brief Perform an hit-or-miss closing.

        Compute the hit-or-miss closing of \a input by the composite
	structuring element (win1, win2).  This is the dual
	transformation of hit-or-miss opening with respect to set
	complementation.
    
	REF: Soille, 2nd ed., p.149.

	By definition \a win1 and \a win2 must have the same origin,
	and need to be disjoint.  This algorithm has been extended to
	every data types (althought it is not increasing).  Beware the
	result depends upon the image data type if it is not binary.

	\arg \c I  Exact type of the input image.
	\arg \c W1 Exact type of the first structuring element.
	\arg \c W2 Exact type of the second structuring element.

	\param input image to process.
	\param win1  first structuring element.
	\param win2  second structuring element.  */
    template<class I, class W1, class W2>
    oln_type_of(I, concrete)
    hit_or_miss_closing(const abstract::image<I>& input,
			const abstract::window<W1>& win1,
			const abstract::window<W2>& win2)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W1, grid)>::ensure();
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W2, grid)>::ensure();
      entering("morpho::hit_or_miss_closing");

      oln_type_of(I, concrete) output("output");
      // FIXME: Is this correct?  Soille doesn't detail the definition
      // of the hit-or-miss closing in his book.
      output =
	level::invert(hit_or_miss_opening(level::invert(input), win1, win2));

      exiting("morpho::hit_or_miss_closing");
      return output;
    }

    /*! \brief Perform an hit-or-miss closing of the background.

        Compute the hit-or-miss closing of the background of \a input
	by the composite structuring element (win1, win2).  This is
	the dual transformation of hit-or-miss opening of the
	background with respect to set complementation.

	REF: Soille, 2nd ed., p.149.

	By definition \a win1 and \a win2 must have the same origin,
	and need to be disjoint.  This algorithm has been extended to
	every data types (althought it is not increasing).  Beware the
	result depends upon the image data type if it is not binary.

	\arg \c I  Exact type of the input image.
	\arg \c W1 Exact type of the first structuring element.
	\arg \c W2 Exact type of the second structuring element.

	\param input image to process.
	\param win1  first structuring element.
	\param win2  second structuring element.  */
    template<class I, class W1, class W2>
    oln_type_of(I, concrete)
    hit_or_miss_closing_bg(const abstract::image<I>& input,
			   const abstract::window<W1>& win1,
			   const abstract::window<W2>& win2)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W1, grid)>::ensure();
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W2, grid)>::ensure();
      entering("morpho::hit_or_miss_closing_bg");

      oln_type_of(I, concrete) output("output");
      // FIXME: Is this correct?  Soille doesn't detail the definition
      // of the hit-or-miss closing of the background in his book.
      output =
	level::invert(hit_or_miss_opening_bg(level::invert(input),
					     win1, win2));

      exiting("morpho::hit_or_miss_closing_bg");
      return output;
    }

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_HIT_OR_MISS_HH
