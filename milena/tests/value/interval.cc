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

#include <cassert>
#include <vector>
#include <algorithm>

#include <mln/value/intsub.hh>
#include <mln/value/interval.hh>
#include <mln/value/inc.hh>

# include <mln/math/min.hh>
# include <mln/math/max.hh>

int main()
{
  using namespace mln::value;
  using namespace mln;

  typedef interval<intsub<2> > R;

  {
    R r(0, 0);
    assert(r.is_degenerated());
    assert(r.length() == 0);
    assert(r.nvalues() == 1);
  }

  {
    assert(are_adjacent(R(0,1), R(1+half,2)));
  }

  {
    std::vector<R> v;
    v.push_back(R(0,0));
    std::sort(v.begin(), v.end(), std::less<R>());
  }

  {
    R r(1, 5);
    assert(!r.is_degenerated());
    assert(r.length() == 4);
    assert(r.nvalues() == 9);

    int ref_i = 2;
    for (intsub<2> v = r.first(); v <= r.last(); inc(v))
      mln_assertion(v.to_enc() == ref_i++);
  }

  {
    // are_adjacent / span / do_intersect
    R r1(3, 4);
    R r2(4.5, 6);
    mln_assertion(r1 != r2);
    mln_assertion(are_adjacent(r1, r2));
    mln_assertion(span(r1, r2) == R(3,6));
    mln_assertion(!do_intersect(r1, r2));

    // Interset / Inter
    R r3(1, 3.5);
    mln_assertion(do_intersect(r1, r3));
    mln_assertion(inter(r1, r3) == R(3, 3.5));

    // Min / Max
    mln_assertion(math::min(r1, r3) == R(1, 3.5));
    mln_assertion(math::max(r1, r3) == R(3, 4));
  }

  // Access from/to indexes.
  {
    R r1(7.5, 11.5);
    mln_assertion(r1.index_of(7.5) == 0);
    mln_assertion(r1.index_of(9.5) == 4);
    mln_assertion(r1.index_of(11.5) == 8);
    mln_assertion(r1.ith_element(0) == 7.5);
    mln_assertion(r1.ith_element(4) == 9.5);
    mln_assertion(r1.ith_element(8) == 11.5);
  }

}
