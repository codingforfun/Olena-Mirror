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

/*=processing opening
 * ns: morpho, morpho::fast
 * what: Morphological opening.
 * arg: const image<I>&, input, IN, input image
 * arg: const struct_elt<E>&, se, IN, structural element
 * ret: Concrete(I)
 * doc:
 *   Compute the morphological opening of @var{input} using @var{se}
 *   as structural element.
 * see: morpho::erosion
 * see: morpho::dilation
 * see: morpho::closing
 * ex:
 * $ image2d<bin> im = load("object.pbm");
 * $ save(morpho::opening(im, win_c8p()), "out.pbm");
 * exi: object.pbm
 * exo: out.pbm
=*/
template<class I, class E> inline
Concrete(I) opening(const image<I>& input, const struct_elt<E>& se)
{
  return dilation(erosion(input, se), -to_exact(se));
}
