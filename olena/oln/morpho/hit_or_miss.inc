// -*- c++ -*-
// Copyright (C) 2001  EPITA Research and Development Laboratory
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

// extended to every data types ;
// note that results depend on the type of image data when it is not bin.
// you MUST be aware of it.

/*=processing hit_or_miss
 * ns: morpho, morpho::fast
 * what: Hit_or_Miss Transform.
 * arg: const convert::abstract::conversion<C, B>&, c, IN, conversion object
 * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se1, IN, structural element
 * arg: const abstract::struct_elt<E>&, se2, IN, structural element
 * ret: typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
 * doc:
 * Compute the hit_or_miss transform of \var{input} by the composite structural
 * element (\var{se1}, \var{se2}). Soille p.131.
 *
 * By definition \var{se1} and \var{se2} must have the same origin, and need to
 * be disjoint. This algorithm has been extended to every data types
 * (althought it is not increasing).  Beware the result depends upon the
 * image data type if it is not \code{bin}.
 * ex:
 * $ image2d<ntg::bin> im = load("object.pbm");
 * $ window2d mywin;
 * $ mywin
 * $  .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
 * $  .add(-2,-1).add(-2,0).add(-2,1)
 * $  .add(-1,0);
 * $ window2d mywin2 = - mywin;
 * $ save(morpho::fast::hit_or_miss(convert::bound<int_u8>(),
 * $                                im, mywin, mywin2), "out.pgm");
 * exh: convert/bound.hh
 * exi: object.pbm
 * exo: out.pgm
=*/
template<class C, class B, class I, class E1, class E2>
typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
hit_or_miss(const convert::abstract::conversion<C, B>& c,
	    const abstract::non_vectorial_image<I>& input,
	    const abstract::struct_elt<E1>& se1,
	    const abstract::struct_elt<E2>& se2)
{
  mlc::eq<I::dim, E1::dim>::ensure();
  mlc::eq<E1::dim, E2::dim>::ensure();

  // FIXME: call inter on w_windows destroys weights
  precondition(inter(to_exact(se1), to_exact(se2)).card() == 0);
  // FIXME: max(coord, coord) does not exist.
  input.border_adapt_copy((se1.delta() > se2.delta()) ?
			  se1.delta() : se2.delta());

  return arith::min(c,
		    erosion(input, se1),
		    erosion(level::invert(input), se2));
}

/*=processingoverload hit_or_miss
 * ns: morpho, morpho::fast
 * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se1, IN, structural element
 * arg: const abstract::struct_elt<E>&, se2, IN, structural element
 * ret:oln_concrete_type(I)
=*/
template<class I, class E1, class E2>
oln_concrete_type(I) hit_or_miss(const abstract::non_vectorial_image<I>& input,
			const abstract::struct_elt<E1>& se1,
			const abstract::struct_elt<E2>& se2)
{
  return hit_or_miss(convert::force<oln_value_type(I)>(), input, se1, se2);
}


//
//  hit-or-miss opening
//
///////////////////////////////////

/*=processing hit_or_miss_opening
 * ns: morpho, morpho::fast
 * what: Hit_or_Miss opening.
 * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se1, IN, structural element
 * arg: const abstract::struct_elt<E>&, se2, IN, structural element
 * ret:oln_concrete_type(I)
 * doc:
 *   Compute the hit_or_miss opening of \var{input} by the composite structural
 * element (\var{se1}, \var{se2}). Soille p.134.
 *
 * By definition \var{se1} and \var{se2} must have the same origin, and need to
 * be disjoint. This algorithm has been extended to every data types
 * (althought it is not increasing).  Beware the result depends upon the
 * image data type if it is not \code{bin}.
 * see: morpho::hit_or_miss
 * see: morpho::hit_or_miss_closing
 * see: morpho::hit_or_miss_closing_bg
 * see: morpho::hit_or_miss_opening_bg
 * ex:
 * $ image2d<ntg::bin> im = load("object.pbm");
 *  window2d mywin;
 *  mywin
 *   .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
 *   .add(-2,-1).add(-2,0).add(-2,1)
 *   .add(-1,0);
 * window2d mywin2 = - mywin;
 * $ save(morpho::hit_or_miss_opening(im, mywin, mywin2), "out.pbm");
 * exi: object.pbm
 * exo: out.pbm
=*/
template<class I, class E1, class E2>
oln_concrete_type(I) hit_or_miss_opening(const abstract::non_vectorial_image<I>& input,
				const abstract::struct_elt<E1>& se1,
				const abstract::struct_elt<E2>& se2)
{
  // FIXME: call inter on w_windows destroys weights
  precondition(inter(to_exact(se1), to_exact(se2)).card() == 0);
  return dilation(hit_or_miss(input, se1, se2), -se1);
}

/*=processing hit_or_miss_opening_bg
 * ns: morpho, morpho::fast
 * what: Hit_or_Miss opening of background.
 * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se1, IN, structural element
 * arg: const abstract::struct_elt<E>&, se2, IN, structural element
 * ret:oln_concrete_type(I)
 * doc:
 *  Compute the hit_or_miss opening of the background of
 * \var{input} by the composite structural
 * element (\var{se1}, \var{se2}). Soille p.135.
 *
 * By definition \var{se1} and \var{se2} must have the same origin, and need to
 * be disjoint. This algorithm has been extended to every data types
 * (althought it is not increasing).  Beware the result depends upon the
 * image data type if it is not \code{bin}.
 * see: morpho::hit_or_miss
 * see: morpho::hit_or_miss_closing
 * see: morpho::hit_or_miss_closing_bg
 * see: morpho::hit_or_miss_opening
 * ex:
 * $ image2d<ntg::bin> im = load("object.pbm");
 *  window2d mywin;
 *  mywin
 *   .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
 *   .add(-2,-1).add(-2,0).add(-2,1)
 *   .add(-1,0);
 * window2d mywin2 = - mywin;
 * $ save(morpho::hit_or_miss_opening_bg(im, mywin, mywin2), "out.pbm");
 * exi: object.pbm
 * exo: out.pbm
=*/
template<class I, class E1, class E2>
oln_concrete_type(I) hit_or_miss_opening_bg(const abstract::non_vectorial_image<I>& input,
				   const abstract::struct_elt<E1>& se1,
				   const abstract::struct_elt<E2>& se2)
{
  return hit_or_miss_opening(input, se2, se1);
}


//
//  hit-or-miss closing
//
///////////////////////////////////

/*=processing hit_or_miss_closing
 * ns: morpho, morpho::fast
 * what: Hit_or_Miss closing.
 * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se1, IN, structural element
 * arg: const abstract::struct_elt<E>&, se2, IN, structural element
 * ret:oln_concrete_type(I)
 * doc:
 *   Compute the hit_or_miss closing of \var{input} by the composite structural
 * element (\var{se1}, \var{se2}). This is the dual transformation of hit-or-miss opening
 *  with respect to
 * set complementation. Soille p.135.
 *
 * By definition \var{se1} and \var{se2} must have the same origin, and need to
 * be disjoint. This algorithm has been extended to every data types
 * (althought it is not increasing).  Beware the result depends upon the
 * image data type if it is not \code{bin}.
 * see: morpho::hit_or_miss
 * see: morpho::hit_or_miss_closing_bg
 * see: morpho::hit_or_miss_opening
 * see: morpho::hit_or_miss_opening_bg
 * ex:
 * $ image2d<ntg::bin> im = load("object.pbm");
 *  window2d mywin;
 *  mywin
 *   .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
 *   .add(-2,-1).add(-2,0).add(-2,1)
 *   .add(-1,0);
 * window2d mywin2 = - mywin;
 * $ save(morpho::hit_or_miss_closing(im, mywin, mywin2), "out.pbm");
 * exi: object.pbm
 * exo: out.pbm
=*/
template<class I, class E1, class E2>
oln_concrete_type(I) hit_or_miss_closing(const abstract::non_vectorial_image<I>& input,
				const abstract::struct_elt<E1>& se1,
				const abstract::struct_elt<E2>& se2)
{
  // FIXME: call inter on struct elts, even if w_windows do not provide it
  precondition(inter(to_exact(se1), to_exact(se2)).card() == 0);
  return level::invert(hit_or_miss_opening(level::invert(input),
					   se1, se2));
}

/*=processing hit_or_miss_closing_bg
 * ns: morpho, morpho::fast
 * what: Hit_or_Miss closing of background.
 * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se1, IN, structural element
 * arg: const abstract::struct_elt<E>&, se2, IN, structural element
 * ret:oln_concrete_type(I)
 * doc:
 * Compute the hit_or_miss closing of the background of \var{input} by the composite structural
 * element (\var{se1}, \var{se2}). This is the dual transformation of hit-or-miss opening
 *  with respect to
 * set complementation. Soille p.135.
 *
 * By definition \var{se1} and \var{se2} must have the same origin, and need to
 * be disjoint. This algorithm has been extended to every data types
 * (althought it is not increasing).  Beware the result depends upon the
 * image data type if it is not \code{bin}.
 * see: morpho::hit_or_miss
 * see: morpho::hit_or_miss_closing
 * see: morpho::hit_or_miss_opening
 * see: morpho::hit_or_miss_opening_bg
 * ex:
 * $ image2d<ntg::bin> im = load("object.pbm");
 *  window2d mywin;
 *  mywin
 *   .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
 *   .add(-2,-1).add(-2,0).add(-2,1)
 *   .add(-1,0);
 * window2d mywin2 = - mywin;
 * $ save(morpho::hit_or_miss_closing_bg(im, mywin, mywin2), "out.pbm");
 * exi: object.pbm
 * exo: out.pbm
=*/
template<class I, class E1, class E2>
oln_concrete_type(I) hit_or_miss_closing_bg(const abstract::non_vectorial_image<I>& input,
				   const abstract::struct_elt<E1>& se1,
				   const abstract::struct_elt<E2>& se2)
{
  return hit_or_miss_closing(input, se2, se1);
}
