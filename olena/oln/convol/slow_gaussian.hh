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
# include "convolution.hh"

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

      /*! Gaussian filter.
      **
      ** \todo FIXME: this class has been done on hurry.
      ** \arg in input image.
      ** \arg sigma sigma.
      ** \arg mu point corresponding to the center.
      */
      template <class I>
      oln_concrete_type(I)
      gaussian(const oln::abstract::non_vectorial_image<I> & in,
	       const ntg::float_d sigma,
	       const oln_point_type(I) & mu = oln_point_type(I)())
      {
	static const typename gaussian_kernel<I::dim>::ret k
	  = gaussian_kernel<I::dim>::kernel_norm(sigma, mu);
	in.border_adapt_mirror(k.delta());
	//FIXME: we should use fast convol.
	//FIXME: This is hugely.
	typename mute<I, double>::ret im = oln::convol::slow::convolve<double>
	  (in,
	   gaussian_kernel<I::dim>::kernel(sigma, mu));
	oln_concrete_type(I) out(in.size());
	oln_iter_type(I) it(out);
	for_all(it)
	  out[it] = oln_value_type(I)(im[it]);
	return out;
      }
    }//End namespace slow
  }// End namespace convol
}// End namespace oln

#include "slow_gaussian.hxx"
#endif // end SLOW_GAUSSIAN_HH
