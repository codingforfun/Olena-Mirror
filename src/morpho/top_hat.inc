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


template<class C, class I, class E> inline
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
white_top_hat(const conversion<C>& c,
	      const image<I>& input, const struct_elt<E>& se)
{
  return arith::minus(c, input, opening(input, se));
}

template<class I, class E> inline
Concrete(I) white_top_hat(const image<I>& input, const struct_elt<E>& se)
{
  return white_top_hat(convert::force<Value(I)>(), input, se);
}


// black top-hat

template<class C, class I, class E> inline
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
black_top_hat(const conversion<C>& c,
	      const image<I>& input, const struct_elt<E>& se)
{
  return arith::minus(c, closing(input, se), input);
}

template<class I, class E> inline
Concrete(I) black_top_hat(const image<I>& input, const struct_elt<E>& se)
{
  return black_top_hat(convert::force<Value(I)>(), input, se);
}


// self-complementary top-hat
//  = white_top_hat + black_top_hat
//  = (input - opening) + (closing - input)
//  = closing - opening
template<class C, class I, class E> inline
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
self_complementary_top_hat(const conversion<C>& c,
			   const image<I>& input, const struct_elt<E>& se)
{
  return arith::minus(c, closing(input, se), opening(input, se));
}

template<class I, class E> inline
Concrete(I) self_complementary_top_hat(const image<I>& input,
				       const struct_elt<E>& se)
{
  return self_complementary_top_hat(convert::force<Value(I)>(), input, se);
}


// top-hat contrast operator
template<class C, class I, class E> inline
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
top_hat_contrast_op(const conversion<C>& c,
		    const image<I>& input, const struct_elt<E>& se)
{
  // FIXME: The original used a conversion for the inner minus
  // too.  I think it's wrong.  Else the output type needs to be
  // adapted to reflect the composition of the conversions. -- adl
  return arith::plus(c,
		     input,
		     arith::minus(white_top_hat(input, se),
				  black_top_hat(input, se)));
}

template<class I, class E> inline
Concrete(I) top_hat_contrast_op(const image<I>& input,
				const struct_elt<E>& se)
{
  return top_hat_contrast_op(convert::force<Value(I)>(), input, se);
}
