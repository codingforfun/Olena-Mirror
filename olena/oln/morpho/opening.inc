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

/*!
** \brief Perform a morphological opening.
**
**   Compute  the   morphological  opening   of  input  using   se  as
**   structuring element.
**
** \param I Exact type of the input image.
** \param E Exact type of the structuring element.
**
** \arg input Image to process.
** \arg se Structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/opening.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::bin>	im_type;
**
**   im_type	im(oln::load(IMG_IN "object.pbm"));
**
**   oln::save(oln::morpho::opening(im, oln::win_c8p()),
**                                       IMG_OUT "oln_morpho_fast_opening.pbm");
**   return  0;
** }
** \endcode
**
** \image html object.png
** \image latex object.png
** =>
** \image html oln_morpho_fast_opening.png
** \image latex oln_morpho_fast_opening.png
**
** \todo FIXME: Histogram problems (fast version do not compile).
*/
template<class I, class E>
oln_concrete_type(I)
opening(const abstract::non_vectorial_image<I>& input,
	  const abstract::struct_elt<E>& se)
{
  //  mlc::eq< I::dim, E::dim >::ensure();
  return dilation(erosion(input, se), -(se.exact()));
}
