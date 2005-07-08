// Copyright (C) 2002, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_TOP_HAT_HH
# define OLENA_MORPHO_TOP_HAT_HH

# include <mlc/cmp.hh>

# include <oln/utils/record.hh>
# include <oln/morpho/opening.hh>
# include <oln/morpho/closing.hh>
# include <oln/morpho/temp.hh>
# include <oln/level/arith.hh>

// FIXME: More documentation (in particular, code snippets).  See Olena
// 0.10's doc.

namespace oln {

  namespace morpho {

    /*! \brief Compute the white top-hat of an image (also called
        top-hat by opening).
    
        Compute white top-hat of \a input using \a win as structuring
        element.

	REF: Soille, 2nd ed., p.121.
    
	\arg \c I exact type of the input image.
	\arg \c W exact type of the first structuring element.

	\param input image to process.
	\param win   structuring element.  */
    template<typename I, typename W>
    oln_type_of(I, concrete)
    white_top_hat(const abstract::image<I>& input,
		  const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      entering("morpho::white_top_hat");
      registering(input, "input");
      oln_type_of(I, concrete) open("open"), output("output");

      open = opening(input, win);
      output = force_value_type_to<oln_type_of(I, concrete)>(input - open);

      exiting("morpho::white_top_hat");
      return output;
    }

    /*! \brief Compute the black top-hat of an image (also called
        top-hat by closing).
    
        Compute black top-hat of \a input using \a win as structuring
        element.

	REF: Soille, 2nd ed., p.122.
    
	\arg \c I exact type of the input image.
	\arg \c W exact type of the first structuring element.

	\param input image to process.
	\param win   structuring element.  */
    template<typename I, typename W>
    oln_type_of(I, concrete)
    black_top_hat(const abstract::image<I>& input,
		  const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      entering("morpho::black_top_hat");
      registering(input, "input");
      oln_type_of(I, concrete) close("close"), output("output");

      close = closing(input, win);
      output = force_value_type_to<oln_type_of(I, concrete)>(close - input);

      exiting("morpho::black_top_hat");
      return output;
    }

    /*! \brief Compute the self-complementary top-hat of an image.
    
        Compute self-complementary top-hat of \a input using \a win as
        structuring element.

	REF: Soille, 2nd ed., p.122.
    
	\arg \c I exact type of the input image.
	\arg \c W exact type of the first structuring element.

	\param input image to process.
	\param win   structuring element.  */
    template<typename I, typename W>
    oln_type_of(I, concrete)
    self_complementary_top_hat(const abstract::image<I>& input,
			       const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      entering("morpho::self_complementary_top_hat");
      registering(input, "input");
      oln_type_of(I, concrete) close("close"), open("open"), output("output");

      close = closing(input, win);
      open  = opening(input, win);
      output = force_value_type_to<oln_type_of(I, concrete)>(close - open);

      exiting("morpho::self_complementary_top_hat");
      return output;
    }

    /*! \brief Top-hat contrast operator.
    
        Enhance contrast of \a input by adding the white top-hat, then
	subtracting the black top-hat to \a input.  Top-hats are
	computed using \win as structuring element.

	REF: Soille, 2nd ed., p.126.

	\arg \c I exact type of the input image.
	\arg \c W exact type of the first structuring element.

	\param input image to process.
	\param win   structuring element.  */
    template<typename I, typename W>
    oln_type_of(I, concrete)
    top_hat_contrast_op(const abstract::image<I>& input,
			const abstract::window<W>& win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();

      entering("morpho::top_hat_contrast_op");
      registering(input, "input");
      oln_type_of(I, concrete) wth("wth"), bth("bth"), output("output");

      wth = white_top_hat(input, win);
      bth = black_top_hat(input, win);
      /* FIXME: A more efficient computation might be achieved using
         this formula:
        
           output =
             force_value_type_to<oln_type_of(I, concrete)>
               (3 * input + closing(input, win) - opening(input, win));
        
         but we don't have the product of image with a literal yet in
         Olena.  */
      /* FIXME: `force' is not necessary the preferred behavior.
	 Consider adding another facade to this operator, to let the
	 user choose another behavior (`bound', for example).  See
	 Olena 0.10's oln/morpho/top_hat.inc.  */
      output =
	force_value_type_to<oln_type_of(I, concrete)>(input + wth - bth);

      exiting("morpho::top_hat_contrast_op");
      return output;
    }

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_TOP_HAT_HH
