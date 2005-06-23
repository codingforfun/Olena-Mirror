// Copyright (C) 2001, 2002, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_GRADIENT_HH
# define OLENA_MORPHO_GRADIENT_HH

# include <oln/morpho/dilation.hh>
# include <oln/morpho/erosion.hh>
# include <oln/level/arith.hh>
# include <oln/convert/force.hh>
# include <oln/core/ch_value_type.hh>

namespace oln {

  namespace morpho {

    // FIXME: Document!  (Get the documentation from Olena 0.10 and
    // adjust it.)

    template<class C, class B, class I, class W>
    typename ch_value_type<I, typename convoutput<C, B, oln_type_of(I, value)>::ret>::ret
    beucher_gradient(const convert::abstract::conversion<C, B>& c,
		     const abstract::scalar_valued_image<I>& input,
		     const abstract::window<W>& win)
    {
      return level::minus(c, dilation(input, win), erosion(input, win));
    }

    template<class I, class W>
    oln_type_of(I, concrete)
    beucher_gradient(const abstract::scalar_valued_image<I>& input,
		     const abstract::window<W>& win)
    {
      return beucher_gradient(convert::force<oln_type_of(I, value)>(),
			      input, win);
    }


    template<class C, class B, class I, class W>
    typename ch_value_type<I, typename convoutput<C, B, oln_type_of(I, value)>::ret>::ret
    internal_gradient(const convert::abstract::conversion<C, B>& c,
		      const abstract::scalar_valued_image<I>& input,
		      const abstract::window<W>& win)
    {
      return level::minus(c, input, erosion(input, win));
    }

    template<class I, class W>
    oln_type_of(I, concrete)
    internal_gradient(const abstract::scalar_valued_image<I>& input,
		      const abstract::window<W>& win)
    {
      return internal_gradient(convert::force<oln_type_of(I, value)>(),
			       input, win);
    }


    template<class C, class B, class I, class W>
    typename ch_value_type<I, typename convoutput<C, B, oln_type_of(I, value)>::ret>::ret
    external_gradient(const convert::abstract::conversion<C, B>& c,
		      const abstract::scalar_valued_image<I>& input,
		      const abstract::window<W>& win)
    {
      return level::minus(c, dilation(input, win), input);
    }

    template<class I, class W>
    oln_type_of (I, concrete)
    external_gradient(const abstract::scalar_valued_image<I>& input,
		      const abstract::window<W>& win)
    {
      return external_gradient(convert::force<oln_type_of(I, value)>(),
			       input, win);
    }

  } // end of namespace oln::morpho

} // end of namespace oln

#endif // OLENA_MORPHO_GRADIENT_HH
