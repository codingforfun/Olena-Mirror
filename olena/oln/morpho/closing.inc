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

/*=processing closing
 * ns: morpho, morpho::fast
 * what: Morphological closing.
 * arg: const abstract::non_vectorial_image<I>&, input, IN, input image
 * arg: const abstract::struct_elt<E>&, se, IN, structuring element
 * ret:oln_concrete_type(I)
 * doc:
 *   Compute the morphological closing of \var{input} using \var{se}
 *   as structuring element.
 * see: morpho::erosion
 * see: morpho::dilation
 * see: morpho::closing
 * ex:
 * $ image2d<ntg::bin> im = load("object.pbm");
 * $ save(morpho::dilation(im, win_c8p()), "out.pbm");
 * exi: object.pbm
 * exo: out.pbm
 =*/
/*!
** \brief Processing closing.
**
** Compute the  morphological closing of input using  se as structuring
** element.
**
** \param I Exact type of the input image.
** \param E Exact type of the structuring element.
**
** \arg input Input image to close.
** \arg se Structuring element to use.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/closing.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::bin>	im_type;
**
**   im_type im1(oln::load(IMG_IN "object.pbm"));
**   save(oln::morpho::closing(im1, oln::win_c8p()),
**        IMG_OUT "oln_morpho_closing.pbm");
**   return  0;
** }
** \endcode
** \image html object_pbm.png
** \image latex object_pbm.png
** =>
** \image html oln_morpho_closing.png
** \image latex oln_morpho_closing.png
*/
template<class I, class E>
oln_concrete_type(I)
  closing(const abstract::non_vectorial_image<I>& input,
	  const abstract::struct_elt<E>& se)
{
  return erosion(dilation(input, se), -(se.exact()));
}
