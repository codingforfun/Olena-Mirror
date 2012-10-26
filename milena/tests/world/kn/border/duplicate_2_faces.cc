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
#include <mln/border/fill.hh>
#include <mln/world/kn/border/duplicate_2_faces.hh>


int main()
{
  using namespace mln;

  border::thickness = 1;

  int refvals[][7] = {
    {2, 9, 2, 9, 3, 9, 3},
    {9, 0, 1, 0, 1, 0, 9},
    {2, 1, 2, 1, 3, 1, 3},
    {9, 0, 1, 0, 1, 0, 9},
    {4, 1, 4, 1, 5, 1, 5},
    {9, 0, 1, 0, 1, 0, 9},
    {4, 9, 4, 9, 5, 9, 5}
  };
  image2d<int> ref = make::image(refvals, point2d(-2,-2));


  int vals[][5] = {
    {0, 1, 0, 1, 0},
    {1, 2, 1, 3, 1},
    {0, 1, 0, 1, 0},
    {1, 4, 1, 5, 1},
    {0, 1, 0, 1, 0}
  };
  image2d<int> ima = make::image(vals, point2d(-1,-1));

  border::fill(ima, 9);
  world::kn::border::duplicate_2_faces(ima);

  mln_piter(image2d<int>) p(ref.domain());
  for_all(p)
    mln_assertion(ima(p) == ref(p));
 }
