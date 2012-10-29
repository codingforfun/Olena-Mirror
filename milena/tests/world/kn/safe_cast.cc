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

#include <mln/value/int_u8.hh>
#include <mln/value/interval.hh>
#include <mln/value/intsub.hh>
#include <mln/world/kn/safe_cast.hh>

int main()
{
  using namespace mln;
  using namespace mln::value;
  using namespace mln::world;

  // int -> int_u<8>
  {
    int i = 2;
    int_u<8> j = kn::safe_cast_to<int_u<8> >(i);
    mln_assertion(j == 2);
  }

  // int -> intsub<2>
  {
    int i = 2;
    intsub<2> j = kn::safe_cast_to<intsub<2> >(i);
    mln_assertion(j == 2);
  }


  // int_u<8> -> int
  {
    int_u<8> i = 2;
    int j = kn::safe_cast_to<int>(i);
    mln_assertion(j == 2);
  }

  // int_u<8> -> intsub<2>
  {
    int_u<8> i = 2;
    intsub<2> j = kn::safe_cast_to<intsub<2> >(i);
    mln_assertion(j == 2);
  }

  // intsub<2> -> int
  {
    intsub<2> i = 2;
    int j = kn::safe_cast_to<int>(i);
    mln_assertion(j == 2);
  }

  // intsub<2> -> int_u<8>
  {
    intsub<2> i = 2;
    int_u<8> j = kn::safe_cast_to<int_u<8> >(i);
    mln_assertion(j == 2);
  }

  // int -> interval<int>
  {
    int i = 2;
    interval<int> j = kn::safe_cast_to<interval<int> >(i);
    mln_assertion(j.is_degenerated() && j.first() == 2);
  }

  // int_u -> interval<int>
  {
    int_u<8> i = 2;
    interval<int> j = kn::safe_cast_to<interval<int> >(i);
    mln_assertion(j.is_degenerated() && j.first() == 2);
  }

  // int_u -> interval<int>
  {
    intsub<2> i = 2;
    interval<int> j = kn::safe_cast_to<interval<int> >(i);
    mln_assertion(j.is_degenerated() && j.first() == 2);
  }

  // int -> interval<int_u<8> >
  {
    int i = 2;
    interval<int_u<8> > j = kn::safe_cast_to<interval<int_u<8> > >(i);
    mln_assertion(j.is_degenerated() && j.first() == 2);
  }

  // int_u<8> -> interval<int_u<8> >
  {
    int_u<8> i = 2;
    interval<int_u<8> > j = kn::safe_cast_to<interval<int_u<8> > >(i);
    mln_assertion(j.is_degenerated() && j.first() == 2);
  }

  // intsub<2> -> interval<int_u<8> >
  {
    intsub<2> i = 2;
    interval<int_u<8> > j = kn::safe_cast_to<interval<int_u<8> > >(i);
    mln_assertion(j.is_degenerated() && j.first() == 2);
  }


  // int -> interval<intsub<2> >
  {
    int i = 2;
    interval<intsub<2> > j = kn::safe_cast_to<interval<intsub<2> > >(i);
    mln_assertion(j.is_degenerated() && j.first() == 2);
  }

  // int_u<8> -> interval<intsub<2> >
  {
    int_u<8> i = 2;
    interval<intsub<2> > j = kn::safe_cast_to<interval<intsub<2> > >(i);
    mln_assertion(j.is_degenerated() && j.first() == 2);
  }

  // intsub<2> -> interval<intsub<2> >
  {
    intsub<2> i = 2;
    interval<intsub<2> > j = kn::safe_cast_to<interval<intsub<2> > >(i);
    mln_assertion(j.is_degenerated() && j.first() == 2);
  }

  // interval<int> -> int
  {
    interval<int> i(2,2);
    int j = kn::safe_cast_to<int>(i);
    mln_assertion(j == 2);
  }

  // interval<int> -> int_u<8>
  {
    interval<int> i(2,2);
    int_u<8> j = kn::safe_cast_to<int_u<8> >(i);
    mln_assertion(j == 2);
  }

  // interval<int> -> int_sub<2>
  {
    interval<int> i(2,2);
    intsub<2> j = kn::safe_cast_to<intsub<2> >(i);
    mln_assertion(j == 2);
  }

  // interval<int_u<8> > -> int
  {
    interval<int_u<8> > i(2,2);
    int j = kn::safe_cast_to<int>(i);
    mln_assertion(j == 2);
  }

  // interval<int_u<8> > -> int_u<8>
  {
    interval<int_u<8> > i(2,2);
    int_u<8> j = kn::safe_cast_to<int_u<8> >(i);
    mln_assertion(j == 2);
  }

  // interval<int_u<8> > -> int_sub<2>
  {
    interval<int_u<8> > i(2,2);
    intsub<2> j = kn::safe_cast_to<intsub<2> >(i);
    mln_assertion(j == 2);
  }

  // interval<intsub<2> > -> int
  {
    interval<intsub<2> > i(2,2);
    int j = kn::safe_cast_to<int>(i);
    mln_assertion(j == 2);
  }

  // interval<intsub<2> > -> int_u<8>
  {
    interval<intsub<2> > i(2,2);
    int_u<8> j = kn::safe_cast_to<int_u<8> >(i);
    mln_assertion(j == 2);
  }

  // interval<intsub<2> > -> int_sub<2>
  {
    interval<intsub<2> > i(2,2);
    intsub<2> j = kn::safe_cast_to<intsub<2> >(i);
    mln_assertion(j == 2);
  }

}
