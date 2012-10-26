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
#include <mln/data/compare.hh>
#include <mln/accu/math/sum.hh>
#include <mln/fun/vv2v/sum.hh>
#include <mln/world/kn/fill_1_from_aux_2_faces.hh>
#include <mln/world/kn/border/duplicate_2_faces.hh>

int main()
{
  using namespace mln;

  int auxvals[5][5] = {
    {0, 0, 0, 0, 0 },
    {0, 1, 0, 4, 0 },
    {0, 0, 0, 0, 0 },
    {0, 4, 0, 1, 0 },
    {0, 0, 0, 0, 0 }
  };
  image2d<int> aux = make::image(auxvals, point2d(-1, -1));
  world::kn::border::duplicate_2_faces(aux);

  int vals[5][5] = {
    {1, 0, 1, 0, 1 },
    {0, 3, 0, 3, 0 },
    {1, 0, 1, 0, 1 },
    {0, 3, 0, 3, 0 },
    {1, 0, 1, 0, 1 }
  };
  image2d<int> imakn = make::image(vals, point2d(-1, -1));

  // Overload with accumulator
  {
    int refvals[5][5] = {
      {1, 1, 1, 4, 1},
      {1, 3, 5, 3, 4},
      {1, 5, 1, 5, 1},
      {4, 3, 5, 3, 1},
      {1, 4, 1, 1, 1}
    };
    image2d<int> ref = make::image(refvals, point2d(-1, -1));

    accu::math::sum<int> accu;
    world::kn::fill_1_from_aux_2_faces(imakn, aux, accu);
    mln_assertion(ref == imakn);
  }

  // Overload with function
  {
    int refvals[5][5] = {
      {1, 2, 1, 8, 1},
      {2, 3, 5, 3, 8},
      {1, 5, 1, 5, 1},
      {8, 3, 5, 3, 2},
      {1, 8, 1, 2, 1}
    };
    image2d<int> ref = make::image(refvals, point2d(-1, -1));

    fun::vv2v::sum<int> f;
    world::kn::fill_1_from_aux_2_faces(imakn, aux, f);

    mln_assertion(ref == imakn);
  }

}
