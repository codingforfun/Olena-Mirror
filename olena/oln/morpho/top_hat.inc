// -*- c++ -*-
// Copyright (C) 2002, 2004  EPITA Research and Development Laboratory
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


/*!
** \brief Compute the white top hat of an image.
**
** Compute   white  top   hat  of   input  using   se   as  structuring
** element. Soille p.105.
**
** \arg c Conversion object.
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/top_hat.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   oln::save(oln::morpho::fast::white_top_hat(oln::convert::bound<ntg::int_u8>(),
**                                             im1,
**                                             oln::win_c8p()),
**             IMG_OUT "oln_morpho_fast_white_top_hat.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256.png
** \image latex lena256.png
** =>
** \image html oln_morpho_fast_white_top_hat.png
** \image latex oln_morpho_fast_white_top_hat.png
*/
template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
white_top_hat(const convert::abstract::conversion<C, B>& c,
	      const abstract::non_vectorial_image<I>& input,
	      const abstract::struct_elt<E>& se)
{
  return arith::minus(c, input, opening(input, se));
}

/*!
** \brief Compute the white top hat of an image.
**
** \param I Exact type of the image.
** \param E Exact type of the structuring element.
**
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/top_hat.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   oln::save(oln::morpho::fast::white_top_hat(im1,
**                                              oln::win_c8p()),
**             IMG_OUT "oln_morpho_fast_white_top_hat_overload.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256.png
** \image latex lena256.png
** =>
** \image html oln_morpho_fast_white_top_hat_overload.png
** \image latex oln_morpho_fast_white_top_hat_overload.png
*/
template<class I, class E>
oln_concrete_type(I)
  white_top_hat(const abstract::non_vectorial_image<I>& input,
		const abstract::struct_elt<E>& se)
{
  return white_top_hat(convert::force<oln_value_type(I)>(), input, se);
}


// black top-hat

/*!
** \brief Compute the black top hat of an image.
**
** Compute   black  top   hat  of   input  using   se   as  structuring
** element. Soille p.105.
**
** \arg c Conversion object.
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/top_hat.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   oln::save(oln::morpho::fast::black_top_hat(oln::convert::bound<ntg::int_u8>(),
**                                             im1,
**                                             oln::win_c8p()),
**             IMG_OUT "oln_morpho_fast_black_top_hat.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256.png
** \image latex lena256.png
** =>
** \image html oln_morpho_fast_black_top_hat.png
** \image latex oln_morpho_fast_black_top_hat.png
*/
template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
black_top_hat(const convert::abstract::conversion<C, B>& c,
	      const abstract::non_vectorial_image<I>& input,
	      const abstract::struct_elt<E>& se)
{
  return arith::minus(c, closing(input, se), input);
}


/*!
** \brief Compute the black top hat of an image.
**
** \param I Exact type of the image.
** \param E Exact type of the structuring element.
**
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/top_hat.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   oln::save(oln::morpho::fast::black_top_hat(im1,
**                                              oln::win_c8p()),
**             IMG_OUT "oln_morpho_fast_black_top_hat_overload.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256.png
** \image latex lena256.png
** =>
** \image html oln_morpho_fast_black_top_hat_overload.png
** \image latex oln_morpho_fast_black_top_hat_overload.png
*/
template<class I, class E>
oln_concrete_type(I)
  black_top_hat(const abstract::non_vectorial_image<I>& input,
		const abstract::struct_elt<E>& se)
{
  return black_top_hat(convert::force<oln_value_type(I)>(), input, se);
}


// self-complementary top-hat
//  = white_top_hat + black_top_hat
//  = (input - opening) + (closing - input)
//  = closing - opening


/*!
** \brief Compute the self complementary top hat of an image.
**
** Compute self complementary top hat of input using se as structuring
** element. Soille p.106.
**
** \arg c Conversion object.
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/top_hat.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   oln::save(oln::morpho::fast::self_complementary_top_hat(oln::convert::bound<ntg::int_u8>(),
**                                                           im1,
**                                                           oln::win_c8p()),
**             IMG_OUT "oln_morpho_fast_self_complementary_top_hat.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256.png
** \image latex lena256.png
** =>
** \image html oln_morpho_fast_self_complementary_top_hat.png
** \image latex oln_morpho_fast_self_complementary_top_hat.png
*/
template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
self_complementary_top_hat(const convert::abstract::conversion<C, B>& c,
			   const abstract::non_vectorial_image<I>& input,
			   const abstract::struct_elt<E>& se)
{
  return arith::minus(c, closing(input, se), opening(input, se));
}

/*!
** \brief Compute the self complementary top hat of an image.
**
**
** \arg c Conversion object.
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/top_hat.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   oln::save(oln::morpho::fast::self_complementary_top_hat(im1,
**                                                           oln::win_c8p()),
**             IMG_OUT "oln_morpho_fast_self_complementary_top_hat_overload.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256.png
** \image latex lena256.png
** =>
** \image html oln_morpho_fast_self_complementary_top_hat_overload.png
** \image latex oln_morpho_fast_self_complementary_top_hat_overload.png
*/
template<class I, class E>
oln_concrete_type(I)
self_complementary_top_hat(const abstract::non_vectorial_image<I>& input,
			   const abstract::struct_elt<E>& se)
{
  return self_complementary_top_hat(convert::force<oln_value_type(I)>(), input, se);
}


// top-hat contrast operator


/*!
** \brief Top hat contrast operator.
**
** Enhance  contrast   input  by  adding  the  white   top  hat,  then
** subtracting the black top hat to input. Top hats are computed using
** se as structuring element. Soille p.109.
**
** \arg c Conversion object.
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/top_hat.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   oln::save(oln::morpho::fast::top_hat_contrast_op(oln::convert::bound<ntg::int_u8>(),
**                                                           im1,
**                                                           oln::win_c8p()),
**             IMG_OUT "oln_morpho_fast_top_hat_contrast_op.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256.png
** \image latex lena256.png
** =>
** \image html oln_morpho_fast_top_hat_contrast_op.png
** \image latex oln_morpho_fast_top_hat_contrast_op.png
*/
template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
top_hat_contrast_op(const convert::abstract::conversion<C, B>& c,
		    const abstract::non_vectorial_image<I>& input,
		    const abstract::struct_elt<E>& se)
{
  return arith::plus(c,
		     input,
		     arith::minus(white_top_hat(input, se),
				  black_top_hat(input, se)));
}


/*!
** \brief Top hat contrast operator.
**
** Enhance  contrast   input  by  adding  the  white   top  hat,  then
** subtracting the black top hat to input. Top hats are computed using
** se as structuring element. Soille p.109.
**
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/top_hat.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   //oln::save(oln::morpho::fast::top_hat_contrast_op(im1,
**   //                                                 oln::win_c8p()),
**   //          IMG_OUT "oln_morpho_fast_top_hat_contrast_op_overload.pbm");
**   oln::save(im1, IMG_OUT "oln_morpho_fast_top_hat_contrast_op_overload.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256.png
** \image latex lena256.png
** =>
** \image html oln_morpho_fast_top_hat_contrast_op_overload.png
** \image latex oln_morpho_fast_top_hat_contrast_op_overload.png
**
** \todo FIXME: Seems not to work with convert::force converter.
*/
template<class I, class E>
oln_concrete_type(I)
top_hat_contrast_op(const abstract::non_vectorial_image<I>& input,
		    const abstract::struct_elt<E>& se)
{
  return top_hat_contrast_op(convert::force<oln_value_type(I)>(), input, se);
}
