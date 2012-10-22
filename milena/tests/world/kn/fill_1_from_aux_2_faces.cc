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
#include <mln/make/box2d.hh>
#include <mln/data/compare.hh>
#include <mln/accu/math/sum.hh>
#include <mln/world/kn/fill_1_from_aux_2_faces.hh>
#include <mln/border/fill.hh>


namespace mln
{

  struct sum_t : Function_vv2v<sum_t>
  {
    typedef int result;

    int operator()(const int& v1, const int& v2) const
    {
      return v1 + v2;
    }

  };

}



int main()
{
  using namespace mln;

  int refvals[5][5] = {
    {1, 1, 1, 4, 1},
    {1, 3, 5, 3, 4},
    {1, 5, 1, 5, 1},
    {4, 3, 5, 3, 1},
    {1, 4, 1, 1, 1}
  };
  image2d<int> ref = make::image(refvals, point2d(-1, -1));

  int auxvals[5][5] = {
    {0, 0, 0, 0, 0 },
    {0, 1, 0, 4, 0 },
    {0, 0, 0, 0, 0 },
    {0, 4, 0, 1, 0 },
    {0, 0, 0, 0, 1 }
  };
  image2d<int> aux = make::image(auxvals, point2d(-1, -1));


  int vals[5][5] = {
    {1, 0, 1, 0, 1 },
    {0, 3, 0, 3, 0 },
    {1, 0, 1, 0, 1 },
    {0, 3, 0, 3, 0 },
    {1, 0, 1, 0, 1 }
  };
  image2d<int> imakn = make::image(vals, point2d(-1, -1));

  /// Make sure the border is set to 0 to get deterministic results.
  border::fill(imakn, 0);


  // Overload with accumulator
  {
    accu::math::sum<int> accu;
    world::kn::fill_1_from_aux_2_faces(imakn, aux, accu);
    mln_assertion(ref == imakn);
  }

  // Overload with function
  {
    sum_t f;
    world::kn::fill_1_from_aux_2_faces(imakn, aux, f);
    mln_assertion(ref == imakn);
  }

}
