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
** \brief Thicken an image.
**
** \param I Exact type of the image.
** \param E1 Exact type of the first structuring element.
** \param E2 Exact type of the second structuring element.
**
** \arg input Image to process.
** \arg se1 First structuring element.
** \arg se2 Second structuring element.
**
** \code
** #include <oln/basics2d.hh>
** #include <oln/morpho/thickening.hh>
** #include <oln/level/compare.hh>
** #include <ntg/all.hh>
** int main()
** {
**   typedef oln::image2d<ntg::int_u8>	im_type;
**
**   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
**
**   oln::save(oln::morpho::fast::thickening(im1,
**                                          oln::win_c8p(),
**                                          oln::win_c8p()),
**                                          IMG_OUT "oln_morpho_fast_thickening.pbm");
**   return  0;
** }
** \endcode
**
** \image html lena256_pgm.png
** \image latex lena256_pgm.png
** =>
** \image html oln_morpho_fast_thickening.png
** \image latex oln_morpho_fast_thickening.png
**
*/
template<class I, class E1, class E2>
oln_concrete_type(I)
  thickening(const abstract::non_vectorial_image<I>& input,
	     const abstract::struct_elt<E1>& se1,
	     const abstract::struct_elt<E2>& se2)
{
  mlc::eq<I::dim, E1::dim>::ensure();
  mlc::eq<I::dim, E2::dim>::ensure();

  oln_concrete_type(I) dilated = dilation(input, se1);
  oln_concrete_type(I) eroded =  erosion(input, se2);
  oln_concrete_type(I) output(input.size());
  oln_iter_type(I) p(input);
  for_all(p)
    {
      if ((dilated[p] == input[p]) && (input[p] < eroded[p]))
	output[p] = dilated[p];
      else
	output[p] = input[p];
    }
  return output;
}
