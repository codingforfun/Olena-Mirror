// -*- c++ -*-
// Copyright 2001  EPITA Research and Development Laboratory
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

// Beucher gradient

template<class C, class I, class E> inline
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
beucher_gradient(const conversion<C>& c,
		 const image<I>& input, const struct_elt<E>& se)
{
  return arith::minus(c, dilation(input, se), erosion(input, se));
}

template<class I, class E> inline
Concrete(I) beucher_gradient(const image<I>& input,
			     const struct_elt<E>& se)
{
  return beucher_gradient(convert::force<Value(I)>(), input, se);
}

// half-gradient by erosion ---or internal gradient

template<class C, class I, class E> inline
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
internal_gradient(const conversion<C>& c,
		  const image<I>& input, const struct_elt<E>& se)
{
  return arith::minus(c, input, erosion(input, se));
}

template<class I, class E> inline
Concrete(I) internal_gradient(const image<I>& input, const
			      struct_elt<E>& se)
{
  return internal_gradient(convert::force<Value(I)>(), input, se);
}

// half-gradient by dilation ---or external gradient

template<class C, class I, class E> inline
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
external_gradient(const conversion<C>& c,
		  const image<I>& input, const struct_elt<E>& se)
{
  return arith::minus(c, dilation(input, se), input);
}

template<class I, class E> inline
Concrete(I) external_gradient(const image<I>& input,
			      const struct_elt<E>& se)
{
  return external_gradient(convert::force<Value(I)>(), input, se);
}
