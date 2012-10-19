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
#include <mln/debug/iota.hh>
#include <mln/util/level.hh>
#include <mln/data/compare.hh>
#include <mln/make/image2d.hh>

int main ()
{
  using namespace mln;

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


  image2d<unsigned> ima(5,5);
  debug::iota(ima);

  image2d<bool> imab_lt = (ima < util::level(15));
  mln_assertion(imab_lt == ref_lt);

  image2d<bool> imab_le = (ima <= util::level(15));
  mln_assertion(imab_le == ref_le);

  image2d<bool> imab_gt = (ima > util::level(15));
  mln_assertion(imab_gt == ref_gt);

  image2d<bool> imab_ge = (ima >= util::level(15));
  mln_assertion(imab_ge == ref_ge);
}
