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


/*=processing beucher_gradient
 * ns: morpho, morpho::fast
 * what: Morphological Beucher Gradient.
 * arg: const conversion<C>&, c, IN, conversion object
 * arg: const image<I>&, input, IN, input image
 * arg: const struct_elt<E>&, se, IN, structural element
 * ret: mute<I, typename convoutput<C, Value(I)>::ret>::ret
 * doc:
 *   Compute the arithmetic difference between the diltation and
 * the erosion of @var{input}  using @var{se} as structural element. Soille, p67.
 * see: morpho::erosion
 * see: morpho::dilation
 * see: morpho::external_gradient
 * see: morpho::internal_gradient
 * ex:
 * $ image2d<int_u8> im = load("lena256.pgm");
 * $ save(morpho::beucher_gradient(im, win_c8p()), "out.pgm");
 * exi: lena256.pgm
 * exo: out.pgm
=*/
template<class C, class I, class E>
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
beucher_gradient(const conversion<C>& c,
		 const image<I>& input, const struct_elt<E>& se)
{
  return arith::minus(c, dilation(input, se), erosion(input, se));
}


/*=processingoverload beucher_gradient
 * ns: morpho, morpho::fast
 * what: Morphological Beucher Gradient.
 * arg: const image<I>&, input, IN, input image
 * arg: const struct_elt<E>&, se, IN, structural element
 * ret: Concrete(I)
=*/
template<class I, class E>
Concrete(I) beucher_gradient(const image<I>& input,
			     const struct_elt<E>& se)
{
  return beucher_gradient(convert::force<Value(I)>(), input, se);
}


/*=processing internal_gradient
 * ns: morpho, morpho::fast
 * what: Morphological Internal Gradient.
 * arg: const conversion<C>&, c, IN, conversion object
 * arg: const image<I>&, input, IN, input image
 * arg: const struct_elt<E>&, se, IN, structural element
 * ret: mute<I, typename convoutput<C, Value(I)>::ret>::ret
 * doc:
 *   Compute the arithmetic difference between the original image @var{input} and
 * the erosion of @var{input} using @var{se} as structural element. Soille, p67.
 * see: morpho::beucher_gradient
 * see: morpho::external_gradient
 * see: morpho::erosion
 * ex:
 * $ image2d<int_u8> im = load("lena256.pgm");
 * $ save(morpho::internal_gradient(im, win_c8p()), "out.pgm");
 * exi: lena256.pgm
 * exo: out.pgm
=*/
template<class C, class I, class E>
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
internal_gradient(const conversion<C>& c,
		  const image<I>& input, const struct_elt<E>& se)
{
  return arith::minus(c, input, erosion(input, se));
}

/*=processingoverload internal_gradient
 * ns: morpho, morpho::fast
 * what: Morphological Internal Gradient.
 * arg: const image<I>&, input, IN, input image
 * arg: const struct_elt<E>&, se, IN, structural element
 * ret: Concrete(I)
=*/
template<class I, class E>
Concrete(I) internal_gradient(const image<I>& input, const
			      struct_elt<E>& se)
{
  return internal_gradient(convert::force<Value(I)>(), input, se);
}

/*=processing external_gradient
 * ns: morpho, morpho::fast
 * what: Morphological External Gradient.
 * arg: const conversion<C>&, c, IN, conversion object
 * arg: const image<I>&, input, IN, input image
 * arg: const struct_elt<E>&, se, IN, structural element
 * ret: mute<I, typename convoutput<C, Value(I)>::ret>::ret
 * doc:
 *   Compute the arithmetic difference between  and the dilatation of
 * @var{input} using @var{se} as structural element, and the original image
 * @var{input}. Soille, p67.
 * see: morpho::beucher_gradient
 * see: morpho::internal_gradient
 * see: morpho::dilation
 * ex:
 * $ image2d<int_u8> im = load("lena256.pgm");
 * $ save(morpho::external_gradient(im, win_c8p()), "out.pgm");
 * exi: lena256.pgm
 * exo: out.pgm
=*/
template<class C, class I, class E>
typename mute<I, typename convoutput<C, Value(I)>::ret>::ret
external_gradient(const conversion<C>& c,
		  const image<I>& input, const struct_elt<E>& se)
{
  return arith::minus(c, dilation(input, se), input);
}


/*=processingoverload external_gradient
 * ns: morpho, morpho::fast
 * what: Morphological External Gradient.
 * arg: const image<I>&, input, IN, input image
 * arg: const struct_elt<E>&, se, IN, structural element
 * ret: Concrete(I)
=*/
template<class I, class E>
Concrete(I) external_gradient(const image<I>& input,
			      const struct_elt<E>& se)
{
  return external_gradient(convert::force<Value(I)>(), input, se);
}
