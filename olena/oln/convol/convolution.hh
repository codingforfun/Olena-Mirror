// Copyright (C) 2001, 2003, 2004  EPITA Research and Development Laboratory
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


#ifndef OLENA_CONVOL_CONVOLUTION_HH__
# define OLENA_CONVOL_CONVOLUTION_HH__

# include <oln/basics.hh>
# include <oln/basics2d.hh>
# include <ntg/all.hh>
# include <mlc/cmp.hh>
# include <mlc/array/all.hh>

namespace oln {

  /*!
  ** \brief convolution namespace.
  **
  ** Set of algorithms related to convolution.
  */
  namespace convol {

    /*!
    ** \brief oln::convol::slow namespace.
    **
    ** The algorithms you can find here are slow.  This mean there are
    ** only basic applications of the definitions.
    */
    namespace slow {

      /*!
      ** \brief Perform a convolution of an image and a window.
      **
      ** \param DestValue Data type of the output image you want.
      ** \param I Exact type of the input image.
      ** \param Win Exact type of the window.
      **
      ** \arg input The image to process.
      ** \arg win The window to convolve with.
      **
      ** \todo FIXME: we must always specify DestValue.
      */
      template<class DestValue, class I, class Win>
      typename mute<I, DestValue>::ret
      convolve(const abstract::image < I >& input,
	       const abstract::w_window< Win >& win)
      {
	mlc::eq<I::dim, Win::dim>::ensure();

	typename mute<I, DestValue>::ret output(input.size());
	input.border_adapt_copy(win.delta());
	oln_iter_type(I) p_im(input);
	for_all(p_im)
	  {
	    DestValue sum = ntg_zero_val(DestValue);
	    for (unsigned i = 0; i < win.card(); ++i)
	      sum += static_cast<DestValue> (win.w(i)) *
		     static_cast<DestValue> (input[p_im - win.dp(i)]);
	    output[p_im] = sum;
	  }

	return output;
      }

      /*!
      ** \brief Perform a convolution of an image and a window.
      **
      ** \param DestValue Data type of the output image you want.
      ** \param I Exact type of the input image.
      ** \param Info Informations about the array.
      ** \param Win Data type of the array.
      **
      ** \arg input The image to process.
      ** \arg arr The array to convolve with.
      **
      ** \todo FIXME: don't use array1d, ..., arraynd.
      */
      template<class DestValue, class I, class Info, class Win>
      typename mute<I, DestValue>::ret
      convolve(const abstract::image < I >& input,
	       const mlc::array2d<Info, Win >& arr)
      {
	return convolve<DestValue>(input, static_cast< w_window2d<Win> >(arr));
	// FIXME: Should be abstract::w_window<T_arr>.  Adjust #include once done.
      }

    } // end namespace slow

  } // end namespace convol

} // end namespace oln

#endif // OLENA_CONVOL_CONVOLUTION_HH__
