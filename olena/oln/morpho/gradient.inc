// -*- c++ -*-
// Copyright (C) 2001, 2004  EPITA Research and Development Laboratory
//
// This  file is  part of  the Olena  Library.  This  library  is free
// software; you can redistribute it  and/or modify it under the terms
// of the  GNU General  Public License version  2 as published  by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with  this library; see the  file COPYING.  If  not, write to
// the Free Software Foundation, 59  Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a  special exception, you  may use this  file as part of  a free
// software library without restriction.  Specifically, if other files
// instantiate templates  or use macros or inline  functions from this
// file, or  you compile  this file  and link it  with other  files to
// produce  an executable,  this file  does  not by  itself cause  the
// resulting  executable  to be  covered  by  the  GNU General  Public
// License.   This exception  does  not however  invalidate any  other
// reasons why the executable file might be covered by the GNU General
// Public License.

/*!
** \brief Process a morphological beucher gradient.
**
** Compute  the arithmetic  difference between  the diltation  and the
** erosion of input using se as structural element. Soille, p67.
**
** \arg c Conversion functor.
** \arg input Image to process.
** \arg se Structuring element.
*/
template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
beucher_gradient(const convert::abstract::conversion<C, B>& c,
		 const abstract::non_vectorial_image<I>& input,
		 const abstract::struct_elt<E>& se)
{
  return arith::minus(c, dilation(input, se), erosion(input, se));
}

/*!
** \brief Process a morphological beucher gradient.
**
** \param I Exact type of the input image.
** \param E Exact type of the structuring element.
**
** \return The beucher gradient of the input.
**
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/gradient.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
**
**   save(oln::morpho::beucher_gradient(im1, oln::win_c8p()),
**        IMG_OUT "oln_morpho_beucher_gradient.pbm");
**   return  0;
** }
** \encode
**
** \image html lena128_pgm.png
** \image latex lena128_pgm.png
** =>
** \image html oln_morpho_beucher_gradient.png
** \image latex oln_morpho_beucher_gradient.png
**
*/
template<class I, class E>
oln_concrete_type(I)
  beucher_gradient(const abstract::non_vectorial_image<I>& input,
		   const abstract::struct_elt<E>& se)
{
  return beucher_gradient(convert::force<oln_value_type(I)>(), input, se);
}


/*!
** \brief Process a morphological internal gradient.
**
** \return The internal gradient of the input.
**
**
** \arg c Conversion functor.
** \arg input Image to process.
** \arg se Structuring element.
**
** Compute the arithmetic difference between the original image input and
** the erosion of input using se as structural element. Soille, p67.
**
*/
template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
internal_gradient(const convert::abstract::conversion<C, B>& c,
		  const abstract::non_vectorial_image<I>& input,
		  const abstract::struct_elt<E>& se)
{
  return arith::minus(c, input, erosion(input, se));
}

/*!
** \brief Process a morphological internal gradient.
**
** \param I Exact type of the input image.
** \param E Exact type of the structuring element.
**
** \return The internal gradient of the input.
**
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/gradient.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   save(oln::morpho::internal_gradient(im1, oln::win_c8p()),
**        IMG_OUT "oln_morpho_internal_gradient.pbm");
**   return  0;
** }
** \encode
**
** \image html lena256_pgm.png
** \image latex lena256_pgm.png
** =>
** \image html oln_morpho_internal_gradient.png
** \image latex oln_morpho_internal_gradient.png
*/
template<class I, class E>
oln_concrete_type(I)
  internal_gradient(const abstract::non_vectorial_image<I>& input, const
		    abstract::struct_elt<E>& se)
{
  return internal_gradient(convert::force<oln_value_type(I)>(), input, se);
}

/*!
** \brief Process a morphological external gradient.
**
** Compute  the arithmetic  difference between  and the  dilatation of
** input  using  se as  structural  element,  and  the original  image
** input. Soille, p67.
**
** \arg c Conversion functor.
** \arg input Image to process.
** \arg se Structuring element.
*/
template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
external_gradient(const convert::abstract::conversion<C, B>& c,
		  const abstract::non_vectorial_image<I>& input,
		  const abstract::struct_elt<E>& se)
{
  return arith::minus(c, dilation(input, se), input);
}


/*!
** \brief Process morphological external gradient.
**
** \param I Exact type of the input image.
** \param E Exact type of the structuring element.
**
** \return The external gradient of the input.
**
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/gradient.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   save(oln::morpho::internal_gradient(im1, oln::win_c8p()),
**        IMG_OUT "oln_morpho_external_gradient.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256_pgm.png
** \image latex lena256_pgm.png
** =>
** \image html oln_morpho_external_gradient.png
** \image latex oln_morpho_external_gradient.png
*/
template<class I, class E>
oln_concrete_type(I)
  external_gradient(const abstract::non_vectorial_image<I>& input,
		    const abstract::struct_elt<E>& se)
{
  return external_gradient(convert::force<oln_value_type(I)>(), input, se);
}
