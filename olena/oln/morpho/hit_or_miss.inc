// -*- c++ -*-
// Copyright (C) 2001, 2003, 2004  EPITA Research and Development Laboratory
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

/*!
** \brief Preform a 'hit or miss' transform.
**
** \arg c Conversion object.
** \arg input Image to process.
** \arg se1 First structuring element.
** \arg se2 Second structuring element.
**
** Compute the hit_or_miss transform of input by the composite structuring
** element (se1, se2). Soille p.131.\n
**
** By definition se1 and se2 must have the same origin, and need to
** be disjoint. This algorithm has been extended to every data types
** (although it is not increasing).  Beware the result depends upon the
** image data type if it is not bin.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/hit_or_miss.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::bin>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "object.pbm"));
**
**   oln::window2d mywin;
**   mywin
**    .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
**    .add(-2,-1).add(-2,0).add(-2,1)
**    .add(-1,0);
**   oln::window2d mywin2 = - mywin;
**
**   oln::save(oln::morpho::hit_or_miss(oln::convert::bound<ntg::int_u8>(), im1, mywin, mywin2),
**                                       IMG_OUT "oln_morpho_fast_hit_or_miss.pbm");
**   return  0;
** }
** \endcode
**
** \image html object_pbm.png
** \image latex object_pbm.png
** =>
** \image html oln_morpho_fast_hit_or_miss.png
** \image latex oln_morpho_fast_hit_or_miss.png
**
** \todo FIXME: Histogram problems (fast version do not work).
*/
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
  precondition(inter(se1.exact(), se2.exact()).card() == 0);
  // FIXME: max(coord, coord) does not exist.
  input.border_adapt_copy((se1.delta() > se2.delta()) ?
			  se1.delta() : se2.delta());

  return arith::min(c,
		    erosion(input, se1),
		    erosion(level::invert(input), se2));
}

/*!
** \brief Preform a 'hit or miss' transform.
**
** \param I Exact type of the input image.
** \param E1 Exact type of the first structuring element.
** \param E2 Exact type of the second structuring element.
**
** \arg input Image to process.
** \arg se1 First structuring element.
** \arg se2 Second structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/hit_or_miss.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::bin>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "object.pbm"));
**
**   oln::window2d mywin;
**   mywin
**    .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
**    .add(-2,-1).add(-2,0).add(-2,1)
**    .add(-1,0);
**   oln::window2d mywin2 = - mywin;
**
**   oln::save(oln::morpho::hit_or_miss(im1, mywin, mywin2),
**                                      IMG_OUT "oln_morpho_fast_hit_or_miss_overload.pbm");
**   return  0;
** }
** \endcode
**
** \image html object_pbm.png
** \image latex object_pbm.png
** =>
** \image html oln_morpho_fast_hit_or_miss_overload.png
** \image latex oln_morpho_fast_hit_or_miss_overload.png
**
** \todo FIXME: Histogram problems (fast version do not work).
*/
template<class I, class E1, class E2>
oln_concrete_type(I)
  hit_or_miss(const abstract::non_vectorial_image<I>& input,
	      const abstract::struct_elt<E1>& se1,
	      const abstract::struct_elt<E2>& se2)
{
  return hit_or_miss(convert::force<oln_value_type(I)>(), input, se1, se2);
}


//
//  hit-or-miss opening
//
///////////////////////////////////

/*!
** \brief Perform an hit or miss opening.
**
** Compute the hit_or_miss opening of input by the composite structuring
** element (se1, se2). Soille p.134.\n
**
** By definition se1 and se2 must have the same origin, and need to
** be disjoint. This algorithm has been extended to every data types
** (althought it is not increasing).  Beware the result depends upon the
** image data type if it is not bin.
**
** \param I Exact type of the input image.
** \param E1 Exact type of the first structuring element.
** \param E2 Exact type of the second structuring element.
**
** \arg input Image to process.
** \arg se1 First structuring element.
** \arg se2 Second structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/hit_or_miss.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::bin>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "object.pbm"));
**
**   oln::window2d mywin;
**   mywin
**    .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
**    .add(-2,-1).add(-2,0).add(-2,1)
**    .add(-1,0);
**   oln::window2d mywin2 = - mywin;
**
**    oln::save(oln::morpho::hit_or_miss_opening(im1, mywin, mywin2),
**                                       IMG_OUT "oln_morpho_fast_hit_or_miss_opening.pbm");
**   return  0;
** }
** \endcode
**
** \image html object_pbm.png
** \image latex object_pbm.png
** =>
** \image html oln_morpho_fast_hit_or_miss_opening.png
** \image latex oln_morpho_fast_hit_or_miss_opening.png
**
** \todo FIXME: Histogram problems (fast version do not work).
*/
template<class I, class E1, class E2>
oln_concrete_type(I)
  hit_or_miss_opening(const abstract::non_vectorial_image<I>& input,
		      const abstract::struct_elt<E1>& se1,
		      const abstract::struct_elt<E2>& se2)
{
  // FIXME: call inter on w_windows destroys weights
  precondition(inter(se1.exact(), se2.exact()).card() == 0);
  return dilation(hit_or_miss(input, se1, se2), -se1);
}

/*!
** \brief Perform an hit or miss opening of background.
**
**  Compute the hit_or_miss opening of  the background of input by the
** composite structuring element (se1, se2). Soille p.135.\n
**
** By definition se1 and se2 must have the same origin, and need to be
** disjoint.  This algorithm  has been  extended to  every  data types
** (although it  is not increasing).   Beware the result  depends upon
** the image data type if it is not bin.
**
** \param I Exact type of the input image.
** \param E1 Exact type of the first structuring element.
** \param E2 Exact type of the second structuring element.
**
** \arg input Image to process.
** \arg se1 First structuring element.
** \arg se2 Second structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/hit_or_miss.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::bin>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "object.pbm"));
**
**   oln::window2d mywin;
**   mywin
**    .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
**    .add(-2,-1).add(-2,0).add(-2,1)
**    .add(-1,0);
**   oln::window2d mywin2 = - mywin;
**
**   oln::save(oln::morpho::hit_or_miss_opening_bg(im1, mywin, mywin2),
**                                       IMG_OUT "oln_morpho_fast_hit_or_miss_opening_bg.pbm");
**   return  0;
** }
** \endcode
**
** \image html object_pbm.png
** \image latex object_pbm.png
** =>
** \image html oln_morpho_fast_hit_or_miss_opening_bg.png
** \image latex oln_morpho_fast_hit_or_miss_opening_bg.png
**
** \todo FIXME: Histogram problems (fast version do not work).
*/
template<class I, class E1, class E2>
oln_concrete_type(I)
  hit_or_miss_opening_bg(const abstract::non_vectorial_image<I>& input,
			 const abstract::struct_elt<E1>& se1,
			 const abstract::struct_elt<E2>& se2)
{
  return hit_or_miss_opening(input, se2, se1);
}


//
//  hit-or-miss closing
//
///////////////////////////////////

/*!
** \brief Perform an hit or miss closing.
**
**   Compute the hit_or_miss closing of input by the composite structuring
** element (se1, se2). This is the dual transformation of hit-or-miss opening
**  with respect to
** set complementation. Soille p.135.\n
**
** By definition se1 and se2 must have the same origin, and need to
** be disjoint. This algorithm has been extended to every data types
** (althought it is not increasing).  Beware the result depends upon the
** image data type if it is not bin.
**
** \param I Exact type of the input image.
** \param E1 Exact type of the first structuring element.
** \param E2 Exact type of the second structuring element.
**
** \arg input Image to process.
** \arg se1 First structuring element.
** \arg se2 Second structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/hit_or_miss.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::bin>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "object.pbm"));
**
**   oln::window2d mywin;
**   mywin
**    .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
**    .add(-2,-1).add(-2,0).add(-2,1)
**    .add(-1,0);
**   oln::window2d mywin2 = - mywin;
**
**   oln::save(oln::morpho::hit_or_miss_closing(im1, mywin, mywin2),
**                                       IMG_OUT "oln_morpho_fast_hit_or_miss_closing.pbm");
**   return  0;
** }
** \endcode
**
** \image html object_pbm.png
** \image latex object_pbm.png
** =>
** \image html oln_morpho_fast_hit_or_miss_closing.png
** \image latex oln_morpho_fast_hit_or_miss_closing.png
**
** \todo FIXME: Histogram problems (fast version do not work).
*/
template<class I, class E1, class E2>
oln_concrete_type(I)
  hit_or_miss_closing(const abstract::non_vectorial_image<I>& input,
		      const abstract::struct_elt<E1>& se1,
		      const abstract::struct_elt<E2>& se2)
{
  // FIXME: call inter on struct elts, even if w_windows do not provide it
  precondition(inter(se1.exact(), se2.exact()).card() == 0);
  return level::invert(hit_or_miss_opening(level::invert(input),
					   se1, se2));
}

/*!
** \brief Perform an hit or miss closing of background.
**
** Compute the hit_or_miss  closing of the background of  input by the
** composite  structuring  element  (se1,   se2).  This  is  the  dual
** transformation   of  hit-or-miss  opening   with  respect   to  set
** complementation. Soille p.135.\n
**
** By definition se1 and se2 must have the same origin, and need to
** be disjoint. This algorithm has been extended to every data types
** (althought it is not increasing).  Beware the result depends upon the
** image data type if it is not bin.
**
** \param I Exact type of the input image.
** \param E1 Exact type of the first structuring element.
** \param E2 Exact type of the second structuring element.
**
** \arg input Image to process.
** \arg se1 First structuring element.
** \arg se2 Second structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/hit_or_miss.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::bin>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "object.pbm"));
**
**   oln::window2d mywin;
**   mywin
**    .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
**    .add(-2,-1).add(-2,0).add(-2,1)
**    .add(-1,0);
**   oln::window2d mywin2 = - mywin;
**
**   oln::save(oln::morpho::hit_or_miss_closing_bg(im1, mywin, mywin2),
**                                       IMG_OUT "oln_morpho_fast_hit_or_miss_closing_bg.pbm");
**   return  0;
** }
** \endcode
**
** \image html object_pbm.png
** \image latex object_pbm.png
** =>
** \image html oln_morpho_fast_hit_or_miss_closing_bg.png
** \image latex oln_morpho_fast_hit_or_miss_closing_bg.png
**
** \todo FIXME: Histogram problems (fast version do not work).
*/
template<class I, class E1, class E2>
oln_concrete_type(I)
hit_or_miss_closing_bg(const abstract::non_vectorial_image<I>& input,
		       const abstract::struct_elt<E1>& se1,
		       const abstract::struct_elt<E2>& se2)
{
  return hit_or_miss_closing(input, se2, se1);
}
