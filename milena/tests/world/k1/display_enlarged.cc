// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file

#include <mln/core/image/image2d.hh>
#include <mln/world/k1/display_enlarged.hh>
#include <mln/world/k1/immerse.hh>
#include <mln/make/box2d.hh>
#include <mln/data/compare.hh>

#include <mln/debug/println.hh>

int main()
{
  using namespace mln;

  int vals[5][5] = {
    {1, 2, 1, 2, 1 },
    {2, 3, 2, 3, 2 },
    {1, 2, 1, 2, 1 },
    {2, 3, 2, 3, 2 },
    {1, 2, 1, 2, 1 }
  };

  int vals3[9][9] = {
    {1, 2, 2, 2, 1, 2, 2, 2, 1},
    {2, 3, 3, 3, 2, 3, 3, 3, 2},
    {2, 3, 3, 3, 2, 3, 3, 3, 2},
    {2, 3, 3, 3, 2, 3, 3, 3, 2},
    {1, 2, 2, 2, 1, 2, 2, 2, 1},
    {2, 3, 3, 3, 2, 3, 3, 3, 2},
    {2, 3, 3, 3, 2, 3, 3, 3, 2},
    {2, 3, 3, 3, 2, 3, 3, 3, 2},
    {1, 2, 2, 2, 1, 2, 2, 2, 1}
  };

  int vals5[17][17] = {
    {1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 2},
    {1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1}
  };

  image2d<int> ima = make::image(vals, point2d(-1, -1));

  {
    image2d<int> enlarged = world::k1::display_enlarged(ima, 1);
    mln_assertion(ima == enlarged);
  }

  {
    image2d<int> ref3 = make::image(vals3, point2d(-2, -2));
    image2d<int> enlarged = world::k1::display_enlarged(ima, 3);
    mln_assertion(ref3 == enlarged);
  }

  {
    image2d<int> ref5 = make::image(vals5, point2d(-4, -4));
    image2d<int> enlarged = world::k1::display_enlarged(ima, 5);
    mln_assertion(ref5 == enlarged);
  }
}
