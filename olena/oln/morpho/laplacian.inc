// -*- c++ -*-
// Copyright (C) 2001, 2004  EPITA Research and Development Laboratory
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
** \brief Compute the laplacian of an image.
**
** Compute the laplacian of input using se as structural element.
**
** \arg c Conversion object.
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/laplacian.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   oln::save(oln::morpho::fast::laplacian(oln::convert::bound<ntg::int_u8>(),
**                                          im1,
**                                          oln::win_c8p()),
**                                          IMG_OUT "oln_morpho_fast_laplacian.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256.png
** \image latex lena256.png
** =>
** \image html oln_morpho_fast_laplacian.png
** \image latex oln_morpho_fast_laplacian.png
*/
template<class C, class B, class I, class E>
typename mute<I, typename convoutput<C, B,oln_value_type(I)>::ret>::ret
laplacian(const convert::abstract::conversion<C, B>& c,
	  const abstract::non_vectorial_image<I>& input,
	  const abstract::struct_elt<E>& se)
{
  return arith::minus(c,
		      arith::minus(dilation(input, se), input),
		      arith::minus(input, erosion(input, se)));
}

/*!
** \brief Compute the laplacian of an image.
**
** Compute the laplacian of input using se as structural element.
**
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/laplacian.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   oln::save(oln::morpho::fast::laplacian(oln::convert::bound<ntg::int_u8>(), im1, oln::win_c8p()),
**                                          IMG_OUT "oln_morpho_fast_laplacian_overload.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256.png
** \image latex lena256.png
** =>
** \image html oln_morpho_fast_laplacian_overload.png
** \image latex oln_morpho_fast_laplacian_overload.png
**
** \todo FIXME: Call the good laplacian, and correct slarger_t type.
*/
template<class I, class E>
typename mute<I,oln_value_type(I)::slarger_t>::ret
laplacian(const abstract::non_vectorial_image<I>& input,
	  const abstract::struct_elt<E>& se)
{
  return laplacian(convert::force<oln_value_type(I)::slarger_t>(), input, se);
}
