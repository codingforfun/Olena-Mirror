// -*- c++ -*-
// Copyright (C) 2002  EPITA Research and Development Laboratory
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



/*=processing white_top_hat
 * ns: morpho, morpho::fast
 * what: White top hat.
 * arg: const convert::abstract::conversion<C, B>&, c, IN, conversion object
 * arg: const abstract::image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se, IN, structural element
 * ret: typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
 * doc:
 *   Compute white top hat of \var{input} using \var{se}
 *   as structural element. Soille p.105.
 * see: morpho::opening
 * ex:
 * $ image2d<int_u8> im = load("lena256.pgm");
 * $ save(morpho::white_top_hat(im, win_c8p()), "out.pgm");
 * exi: lena256.pgm
 * exo: out.pgm
=*/
template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
white_top_hat(const convert::abstract::conversion<C, B>& c,
	      const abstract::image<I>& input,
	      const abstract::struct_elt<E>& se)
{
  return arith::minus(c, input, opening(input, se));
}

/*=processingoverload white_top_hat
 * ns: morpho, morpho::fast
 * what: White top hat.
 * arg: const abstract::image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se, IN, structural element
 * ret: Concrete(I)
=*/
template<class I, class E>
Concrete(I) white_top_hat(const abstract::image<I>& input,
			  const abstract::struct_elt<E>& se)
{
  return white_top_hat(convert::force<Value(I)>(), input, se);
}


// black top-hat

/*=processing black_top_hat
 * ns: morpho, morpho::fast
 * what: Black top hat.
 * arg: const convert::abstract::conversion<C, B>&, c, IN, conversion object
 * arg: const abstract::image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se, IN, structural element
 * ret: typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
 * doc:
 *   Compute black top hat of \var{input} using \var{se}
 *   as structural element. Soille p.105.
 * see: morpho::closing
 * ex:
 * $ image2d<int_u8> im = load("lena256.pgm");
 * $ save(morpho::black_top_hat(im, win_c8p()), "out.pgm");
 * exi: lena256.pgm
 * exo: out.pgm
=*/
template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
black_top_hat(const convert::abstract::conversion<C, B>& c,
	      const abstract::image<I>& input,
	      const abstract::struct_elt<E>& se)
{
  return arith::minus(c, closing(input, se), input);
}


/*=processingoverload balck_top_hat
 * ns: morpho, morpho::fast
 * what: Black top hat.
 * arg: const abstract::image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se, IN, structural element
 * ret: typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
=*/
template<class I, class E>
Concrete(I) black_top_hat(const abstract::image<I>& input,
			  const abstract::struct_elt<E>& se)
{
  return black_top_hat(convert::force<Value(I)>(), input, se);
}


// self-complementary top-hat
//  = white_top_hat + black_top_hat
//  = (input - opening) + (closing - input)
//  = closing - opening

/*=processing self_complementary_top_hat
 * ns: morpho, morpho::fast
 * what: Self complementary top hat.
 * arg: const convert::abstract::conversion<C, B>&, c, IN, conversion object
 * arg: const abstract::image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se, IN, structural element
 * ret: typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
 * doc:
 *   Compute self complementary top hat of \var{input} using \var{se}
 *   as structural element. Soille p.106.
 * see: morpho::closing
 * see: morpho::opening
 * ex:
 * $ image2d<int_u8> im = load("lena256.pgm");
 * $ save(morpho::self_complementary_top_hat(im, win_c8p()), "out.pgm");
 * exi: lena256.pgm
 * exo: out.pgm
=*/

template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
self_complementary_top_hat(const convert::abstract::conversion<C, B>& c,
			   const abstract::image<I>& input,
			   const abstract::struct_elt<E>& se)
{
  return arith::minus(c, closing(input, se), opening(input, se));
}

/*=processingoverload self_complementary_top_hat
 * ns: morpho, morpho::fast
 * what: Self complementary top hat.
 * arg: const abstract::image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se, IN, structural element
 * ret: typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
=*/
template<class I, class E>
Concrete(I) self_complementary_top_hat(const abstract::image<I>& input,
				       const abstract::struct_elt<E>& se)
{
  return self_complementary_top_hat(convert::force<Value(I)>(), input, se);
}


// top-hat contrast operator

/*=processing top_hat_contrast_op
 * ns: morpho, morpho::fast
 * what: Top hat contrastor operator.
 * arg: const convert::abstract::conversion<C, B>&, c, IN, conversion object
 * arg: const abstract::image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se, IN, structural element
 * ret: typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
 * doc:
 *   Enhance contrast \var{input} by adding the white top hat, then
 * substracting the black top hat to \var{input}. Top hats are computed using
 * \var{se} as structural element. Soille p.109.
 * see: morpho::white_top_hat
 * see: morpho::black_top_hat
 * ex:
 * $ image2d<int_u8> im = load("lena256.pgm");
 * $ save(morpho::top_hat_contrast_op(convert::bound<int_u8>(),
 *                                    im, win_c8p()), "out.pgm");
 * exi: lena256.pgm
 * exo: out.pgm
 * exh: convert/bound.hh
=*/

template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
top_hat_contrast_op(const convert::abstract::conversion<C, B>& c,
		    const abstract::image<I>& input,
		    const abstract::struct_elt<E>& se)
{
  return arith::plus(c,
		     input,
		     arith::minus(white_top_hat(input, se),
				  black_top_hat(input, se)));
}

/*=processingoverload top_hat_contrast_op
 * ns: morpho, morpho::fast
 * what: Top hat contrastor operator.
 * arg: const abstract::image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se, IN, structural element
 * ret: typename mute<I, typename convoutput<C, B, Value(I)>::ret>::ret
=*/
template<class I, class E>
Concrete(I) top_hat_contrast_op(const abstract::image<I>& input,
				const abstract::struct_elt<E>& se)
{
  return top_hat_contrast_op(convert::force<Value(I)>(), input, se);
}
