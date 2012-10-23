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

#include <mln/fun/vv2v/mean.hh>
#include <mln/value/intsub.hh>

using namespace mln;

void do_it(float v1, float v2, float res)
{

  {
    fun::vv2v::mean<value::intsub<2> > f;
    mln_assertion(f(v1, v2) == res);
  }

  {
    fun::vv2v::mean<int> f;
    mln_assertion(f(v1, v2) == res);
  }

  {
    fun::vv2v::mean<float> f;
    mln_assertion(f(v1, v2) == res);
  }
}


void do_it2(float v1, float v2, float res)
{
  {
    fun::vv2v::mean<value::intsub<2> > f;
    mln_assertion(f(v1, v2) == res);
  }

  {
    fun::vv2v::mean<float> f;
    mln_assertion(f(v1, v2) == res);
  }
}


int main()
{
  do_it(3, 4, 3.5);
  do_it(0, 4, 2);

  do_it2(0, 4.5, 2.25);
}
