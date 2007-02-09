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
#include <oln/morpho/closing.hh>
#include <oln/level/fill.hh>
#include <oln/morpho/watershed.hh>
#include <oln/morpho/gradient.hh>

using namespace oln;
using namespace ntg;

void morpholena_watershed_beucher()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");

  image2d<int_u8> markers(lena.size());
  level::fill(markers, 0);

  for (int i = 0; i < 10; ++i)
    for (int j = 0; j < 10; ++j)
      {
	markers(477 + i, 67 + j) = 72;
	markers(100 + i, 200 + j) = 20;
	markers(50 + i, 50 + j) = 60;
	markers(100 + i, 500 + j) = 200;
	markers(255 + i, 355 + j) = 120;
      }

  save
    (morpho::watershed_seg_or
     (morpho::closing
      (morpho::beucher_gradient(lena, win_c8p()), win_c8p()), markers,
      neighb_c8()),
     "lena-wor.pgm");
}
