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
#include <oln/morpho/erosion.hh>
#include <oln/morpho/dilation.hh>
#include <oln/morpho/laplacian.hh>

using namespace oln;
using namespace ntg;

void morpholena_base()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");

  save(morpho::erosion(lena,  win_c8p()), "lena-ero.pgm");
  save(morpho::dilation(lena, win_c8p()), "lena-dil.pgm");
  save(morpho::laplacian(convert::force<int_u8>(), lena, win_c8p()),
       "lena-lap.pgm");
  save(morpho::fast::erosion(lena,  win_c8p()), "lena-ero-f.pgm");
  save(morpho::fast::dilation(lena, win_c8p()), "lena-dil-f.pgm");
  save(morpho::fast::laplacian(convert::force<int_u8>(), lena, win_c8p()),
       "lena-lap-f.pgm");
}
