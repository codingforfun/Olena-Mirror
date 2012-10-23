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

#include <mln/math/median.hh>
#include <mln/value/intsub.hh>

template <typename T>
void do_it(std::vector<T>& vec, float res)
{
  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      for (int k = 0; k < 3; ++k)
      {
	mln_assertion(mln::math::median(vec[0], vec[1], vec[2], vec[3]) == res);
	std::swap(vec[k], vec[k + 1]);
      }
    }
  }
}

int main()
{
  using namespace mln;

  {
    mln_assertion(math::median(2,3) == 2.5);
  }

  {
    value::intsub<2> v1(1.5);
    value::intsub<2> v2(2);
    mln_assertion(math::median(v1, v2) == 1.75);
  }

  {
    std::vector<int> vec(4);
    vec[0] = 1;
    vec[1] = 2;
    vec[2] = 3;
    vec[3] = 4;

    do_it(vec, 2.5);
  }

  {
    std::vector<int> vec(4);
    vec[0] = 0;
    vec[1] = 0;
    vec[2] = 6;
    vec[3] = 24;

    do_it(vec, 3);
  }


  {
    std::vector<value::intsub<2> > vec(4);
    vec[0] = 2;
    vec[1] = 1.5;
    vec[2] = 3;
    vec[3] = 5;

    do_it(vec, 2.5);
  }

}
