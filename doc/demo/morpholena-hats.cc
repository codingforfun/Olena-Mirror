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
#include <oln/morpho/opening.hh>
#include <oln/morpho/top_hat.hh>

using namespace oln;
using namespace ntg;

void morpholena_opening_closing()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");

  save(morpho::opening(lena,  win_c8p()), "lena-ope.pgm");
  save(morpho::closing(lena,  win_c8p()), "lena-clo.pgm");
  save(morpho::fast::opening(lena,  win_c8p()), "lena-ope-f.pgm");
  save(morpho::fast::closing(lena,  win_c8p()), "lena-clo-f.pgm");

  window2d mywin;
  mywin
    .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
    .add(-2,-1).add(-2,0).add(-2,1)
    .add(-1,0);
  save(morpho::closing(lena, mywin), "lena-myw.pgm");

  window2d face_se = load(IMGDIR "/face_se.pbm");
  save(morpho::fast::opening(lena,  face_se), "lena-ope-f-face.pgm");

}

void morpholena_hats()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");

  save(morpho::white_top_hat(lena, win_c8p()), "lena-wth.pgm");
  save(morpho::black_top_hat(lena, win_c8p()), "lena-bth.pgm");
  save(morpho::self_complementary_top_hat(lena, win_c8p()),
       "lena-sth.pgm");
  save(morpho::top_hat_contrast_op(convert::bound<int_u8>(),
				   lena, win_c8p()), "lena-thc.pgm");
}

void morpholena_fast_hats()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");

  save(morpho::fast::white_top_hat(lena, win_c8p()), "lena-wth-f.pgm");
  save(morpho::fast::black_top_hat(lena, win_c8p()), "lena-bth-f.pgm");
  save(morpho::fast::self_complementary_top_hat(lena, win_c8p()),
       "lena-sth-f.pgm");
  save(morpho::fast::top_hat_contrast_op(convert::bound<int_u8>(),
					 lena, win_c8p()), "lena-thc-f.pgm");
}
