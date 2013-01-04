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
#include <mln/math/mean.hh>
#include <mln/value/intsub.hh>
#include <mln/value/inc.hh>
#include <mln/value/dec.hh>
#include <mln/value/succ.hh>
#include <mln/value/prev.hh>
#include <mln/math/mean.hh>
#include <mln/math/median.hh>
#include <mln/math/min.hh>
#include <mln/math/ceil.hh>
#include <mln/math/floor.hh>

int main()
{
  using namespace mln;
  using namespace mln::value;

  // i == 2
  intsub<2> i = 2;
  inc(i);

  // i == 2.5
  mln_assertion(i == half + 2);

  // (2.5 + 0.5) / 2 == 1.5
  mln_assertion(math::mean(i, half) == 1 + half);

  // (2.5 + 2) / 2 = 2.25
  intsub<4> res = math::mean(i, intsub<2>(2));
  mln_assertion(res == 2.25);

  // i == 3
  inc(i);
  mln_assertion(i == intsub<2>(3));

  // i == 2.5
  dec(i);
  mln_assertion(i == intsub<2>(2,2));
  dec(i);

  // i == 2.5
  mln_assertion(succ(i) == 2 + half);

  // i == 1.5
  mln_assertion(prev(i) == 2 - half);

  // i == 6
  i *= 3;
  mln_assertion(i == 6);

  // j = 6
  intsub<4> j = i;
  mln_assertion(j == 6);

  // j = 6.25
  inc(j);
  mln_assertion(j == 6 + quarter);

  // k = 10.25
  intsub<8> k(10.25);
  mln_assertion(k == 10 + quarter);

  // sum(j,k,2) == 18.5
  mln_assertion(j + k + 2 == 18.5);

  // min(i,l) == 2.5
  intsub<2> l(2.5);
  mln_assertion(mln::math::min(i, l) == 2.5);

  // mean(6,2,2.5,3) = 3.375
  mln_assertion(math::mean(i, intsub<2>(2), l, intsub<2>(3)) == 3.375);

  // ceil(2.5)
  mln_assertion(math::ceil(l) == 3);
  mln_assertion(math::floor(l) == 2);

  // median(6,2,2.5,3) = 2.75
  mln_assertion(math::median(i, intsub<2>(2), l, intsub<2>(3)) == 2.75);

  // median(6,2) = 4
  mln_assertion(math::median(i, intsub<2>(2)) == 4);


  // 2.5 * 3
  mln_assertion((l * 3) == 7.5);

  // 4 * 2.5
  mln_assertion((4 * l) == 10);

  // 2.5 * 10.25
  mln_assertion((l * k) == 25.625);

  // Mean with 2 intsub
  mln_assertion(math::mean(intsub<2>(2), intsub<2>(4)) == 3);

  // Mean with 4 intsub
  mln_assertion(math::mean(intsub<2>(2), intsub<2>(4), intsub<2>(6), intsub<2>(8)) == 5);

  // Add 2 intsub
  mln_assertion((intsub<2>(2) + intsub<2>(3)) == 5);
}
