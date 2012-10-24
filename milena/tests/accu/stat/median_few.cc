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

#include <mln/value/intsub.hh>
#include <mln/accu/stat/median_few.hh>


template <typename T>
void doit()
{
  typedef mln::accu::stat::median_few<T> M;

  {
    M m;
    m.init();
    m.take(6);
    mln_postcondition(m.to_result() == 6);
  }

  {
    M m;
    m.init();
    m.take(6);
    m.take(6);
    mln_postcondition(m.to_result() == 6);
  }

  {
    M m;
    m.init();
    m.take(6);
    m.take(6);
    m.take(0);
    mln_postcondition(m.to_result() == 6);
  }

  {
    M m;
    m.init();
    m.take(12);
    m.take(0);
    m.take(0);
    m.take(24);
    mln_postcondition(m.to_result() == 6);
  }

  {
    M m;
    m.init();
    m.take(1);
    m.take(0);
    mln_postcondition(m.to_result() == 0.5);
  }
}


int main()
{
  using namespace mln;

  doit<int>();
  doit< value::intsub<2> >();
}