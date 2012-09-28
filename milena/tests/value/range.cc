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

#include <mln/value/unsignedh.hh>
#include <mln/value/range.hh>

static const char *ref[] = { "1", "1.5", "2", "2.5", "3", "3.5", "4", "4.5", "5" };

int main()
{
  using namespace mln::value;

  typedef range<unsignedh> R;

  {
    R r(0, 0);
    assert(r.is_degenerated());
    assert(r.length() == 0);
    assert(r.nelements() == 1);
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
    assert(r.nelements() == 9);

    int ref_i = 0;
    for (unsignedh v = r.first(); v != r.last(); v.goto_succ())
      mln_assertion(static_cast<std::string>(v) == ref[ref_i++]);
  }

}
