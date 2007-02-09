// Copyright (C) 2003  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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


#include <oln/basics2d.hh>
#include <oln/convol/fast_gaussian.hh>

using namespace oln;
using namespace ntg;

int main(void)
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");
  assert(lena.has_impl());

  assert( save(convol::fast::gaussian(lena, float_d(0.50f)),
 	       "lena-gau.pgm") );
  assert( save(convol::fast::gaussian_derivative(lena, float_d(0.50f)),
	       "lena-gd1.pgm") );
  assert( save(convol::fast::gaussian_second_derivative(lena, float_d(0.50f)),
	       "lena-gd2.pgm") );

}
