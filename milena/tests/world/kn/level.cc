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

#include <mln/core/image/image2d.hh>
#include <mln/world/kn/level.hh>
#include <mln/data/compare.hh>
#include <mln/make/image2d.hh>
#include <mln/value/interval.hh>
#include <mln/value/intsub.hh>


int main ()
{
  using namespace mln;
  using namespace mln::value;
  using namespace mln::world::kn;

  bool vref_lt[] = {
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
  };
  image2d<bool> ref_lt = make::image2d(vref_lt);

  bool vref_le[] = {
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
  };
  image2d<bool> ref_le = make::image2d(vref_le);

  bool vref_gt[] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1
  };
  image2d<bool> ref_gt = make::image2d(vref_gt);

  bool vref_ge[] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1
  };
  image2d<bool> ref_ge = make::image2d(vref_ge);


  int i = 0;
  image2d<value::interval<int> > ima(5,5);
  mln_piter_(image2d<value::interval<int> >) p(ima.domain());
  for_all(p)
    ima(p) = ++i;

  // Comparison with images
  {
    image2d<bool> imab_lt = (ima < level(15));
    mln_assertion(imab_lt == ref_lt);

    image2d<bool> imab_le = (ima <= level(15));
    mln_assertion(imab_le == ref_le);

    image2d<bool> imab_gt = (ima > level(15));
    mln_assertion(imab_gt == ref_gt);

    image2d<bool> imab_ge = (ima >= level(15));
    mln_assertion(imab_ge == ref_ge);
  }

  // Comparison with intervals
  {

    interval<intsub<2> > i(3, 10);

    mln_assertion(i < level(11));
    mln_assertion(i <= level(3));
    mln_assertion(i > level(2));
    mln_assertion(i >= level(10));
  }

  {
    // i is degenerated.
    interval<intsub<2> > i(3, 3);
    mln_assertion(i == level(3));
    mln_assertion(i < level(4));
    mln_assertion(i <= level(3));
    mln_assertion(i > level(2));
    mln_assertion(i >= level(2));
  }
}
