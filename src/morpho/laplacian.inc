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

/*=processing laplacian
 * ns: morpho, morpho::fast
 * what: Laplacian.
 * arg: const conversion<C>&, c, IN, conversion object
 * arg: const image<I>&, input, IN, input image
 * arg: const struct_elt<E>&, se, IN, structural element
 * ret: typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
 * doc:
 *   Compute the laplacian of @var{input} using @var{se}
 *   as structural element.
 * see: morpho::dilation
 * see: morpho::erosion
 * ex:
 * $ image2d<int_u8> im = load("lena256.pgm");
 * $ save(morpho::laplacian(convert::bound<int_u8>(), im, win_c8p()), "out.pgm");
 * exi: lena256.pgm
 * exo: out.pgm
 * exh: convert/bound.hh
=*/
template<class C, class I, class E> inline
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
laplacian(const conversion<C>& c,
	  const image<I>& input, const struct_elt<E>& se)
{
  return arith::minus(c,
		      arith::minus(dilation(input, se), input),
		      arith::minus(input, erosion(input, se)));
}

/*=processingoverload laplacian
 * ns: morpho, morpho::fast
 * what: Laplacian.
 * arg: const image<I>&, input, IN, input image
 * arg: const struct_elt<E>&, se, IN, structural element
 * ret: typename mute<I, Value(I)::slarger_t>::ret
=*/
template<class I, class E> inline
typename mute<I, Value(I)::slarger_t>::ret
laplacian(const image<I>& input, const struct_elt<E>& se)
{
  return laplacian(convert::force<Value(I)::slarger_t>(), input, se);
}
