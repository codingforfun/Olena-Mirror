// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CONVOL_FAST_GAUSSIAN_FILTER_HH__
# define OLENA_CONVOL_FAST_GAUSSIAN_FILTER_HH__

# include <oln/basics.hh>
# include <oln/convert/basics.hh>
# include <ntg/float.hh>

//
// Gaussian filter implementation from
// "Recursively implementing the gaussian and its derivatives"
// Deriche 93 INRIA REPORT
//

namespace oln {
  namespace convol {
    namespace fast {

      // FIXME: add tests!

      template <class C, class B, class I>
      typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
      gaussian(const convert::abstract::conversion<C, B>& input_conv,
	       const abstract::image<I>& in, ntg::float_s sigma);

      template <class C, class B, class I>
      typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
      gaussian_derivative(const convert::abstract::conversion<C, B>& input_conv,
			  const abstract::image<I>& in, ntg::float_s sigma);
      template <class C, class B, class I>
      typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
      gaussian_second_derivative(const convert::abstract::conversion<C, B>& input_conv,
				 const abstract::image<I>& in,
				 ntg::float_s sigma);

      /* Same functions, with a default conversion.  */

      template <class I> inline
     oln_concrete_type(I)
      gaussian(const abstract::image<I>& in, ntg::float_s sigma)
      { return gaussian(convert::force<oln_value_type(I)>(), in, sigma); }

      template <class I> inline
     oln_concrete_type(I)
      gaussian_derivative(const abstract::image<I>& in, ntg::float_s sigma)
      { return gaussian_derivative(convert::force<oln_value_type(I)>(), in, sigma); }

      template <class I> inline
     oln_concrete_type(I)
      gaussian_second_derivative(const abstract::image<I>& in,
				 ntg::float_s sigma)
      { return gaussian_second_derivative(convert::force<oln_value_type(I)>(),
					  in, sigma); }
    }
  }
}

# include <oln/convol/fast_gaussian.hxx>

#endif // OLENA_CONVOL_FAST_GAUSSIAN_FILTER_HH__
