// Copyright (C) 2004  EPITA Research and Development Laboratory
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


#ifndef SLOW_GAUSSIAN_HH
# define SLOW_GAUSSIAN_HH
# include <oln/core/image.hh>
# include <ntg/float.hh>
# include <oln/convol/slow_convolution.hh>
# include <oln/core/behavior.hh>

namespace oln {
  namespace convol {
    namespace slow {

      /*! Gaussian kernel.
      **
      ** In the specialization, the function kernel must be defined.
      */
      template <int Dim>
      struct gaussian_kernel
      {
      };

      /*! Slow Gaussian filter.
      **
      ** \arg in input image.
      ** \arg sigma sigma.
      ** \arg radius_factor the kernel of the convolution is done with a
      ** kernel null at a distance of sigma * radius_factor.
      ** \arg behavior Object to know how to work on borders. By default
      ** mirror is used.
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/convol/slow_gaussian.hh>
      ** #include <ntg/int.hh>
      **
      ** using namespace oln;
      ** using namespace ntg;
      **
      ** int main()
      ** {
      **   image2d<int_u8> lena(IMG_IN "lena.pgm");
      **
      **   float_d sigma = 2.5;
      **   lena = convol::slow::gaussian(lena, sigma, 3);
      **   save(lena, IMG_OUT "oln_convol_slow_gaussian.pgm");
      ** }
      ** \endcode
      ** \image html lena_pgm.png
      ** \image latex lena_pgm.png
      ** =>
      ** \image html oln_convol_slow_gaussian.pgm
      ** \image latex oln_convol_slow_gaussian.pgm
      */

      template <class I, class BE>
      oln_concrete_type(I)
	gaussian(const oln::abstract::non_vectorial_image<I> & in,
		 const ntg::float_d sigma,
		 const ntg::float_d radius_factor,
		 const oln::abstract::behavior<BE> &behavior)
      {
	const typename gaussian_kernel<I::dim>::ret k
	  = gaussian_kernel<I::dim>::kernel(sigma, radius_factor);

	// Compute Delta.
	// \todo FIXME: should be in the image hierarchy.
	coord delta = 0;
	for (unsigned i = 0; i < I::dim; i++)
	  if (in.size().nth(i) > delta)
	    delta = in.size().nth(i);
	behavior.adapt_border(in, delta);

	typename mute<I, ntg::float_d>::ret im =
	  oln::convol::slow::convolve<ntg::float_d> (in, k);
	oln_concrete_type(I) out(in.size());
	oln_iter_type(I) it(out);
	for_all(it)
	  out[it] = oln_value_type(I)(im[it]);
	return out;
      }

      //! Gaussian filter, with default borders.
      template <class I>
      oln_concrete_type(I)
	gaussian(const oln::abstract::non_vectorial_image<I> & in,
		 const ntg::float_d sigma,
		 const ntg::float_d radius_factor)
      {
	return gaussian(in, sigma, radius_factor, mirror_behavior<>());
      }
    }//End namespace slow
  }// End namespace convol
}// End namespace oln

#include "slow_gaussian.hxx"
#endif // end SLOW_GAUSSIAN_HH
