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

#include <mln/accu/stat/median_interval.hh>


int main()
{
  using namespace mln;

  // Empty
  {
    // FIXME: How should it behave ?
    accu::stat::median_interval<1> med(1, 4);
    mln_assertion(!med.is_valid());
  }

  // Single value
  {
    // FIXME: How should it behave ?
    accu::stat::median_interval<1> med(1, 4);
    med.take(1);
    mln_assertion(!med.is_valid());
  }

  // Same value several times (odd count)
  {
    accu::stat::median_interval<1> med(1, 4);
    med.take(1);
    med.take(1);
    med.take(1);
    mln_assertion(med.to_result() == 1);
  }

  // Same value several times (even count)
  {
    accu::stat::median_interval<1> med(1, 4);
    med.take(1);
    med.take(1);
    med.take(1);
    med.take(1);
    mln_assertion(med.to_result() == 1);
  }

  // Same value several times + one different higher value.
  {
    accu::stat::median_interval<1> med(0, 4);
    med.take(0);
    med.take(1);
    med.take(1);
    med.take(2);
    mln_assertion(med.to_result() == 1);
  }


  // Various values
  {
    accu::stat::median_interval<1> med(0, 4);
    med.take(0);
    med.take(1);
    med.take(1);
    med.take(3);
    med.take(3);
    med.take(4);
    mln_assertion(med.to_result() == 2);
  }


  // Various values
  {
    accu::stat::median_interval<1> med(0, 4);
    med.take(0);
    med.take(1);
    med.take(1);
    med.take(2);
    med.take(2);
    med.take(3);
    med.take(3);
    med.take(4);
    mln_assertion(med.to_result() == 2);
  }

  // Same value several times + one different higher value.
  {
    accu::stat::median_interval<1> med(1, 4);
    med.take(1);
    med.take(1);
    med.take(1);
    med.take(2);
    mln_assertion(med.to_result() == 1);
  }

  // Same value several times + one different lower value.
  {
    accu::stat::median_interval<1> med(1, 4);
    med.take(1);
    med.take(2);
    med.take(2);
    med.take(2);
    mln_assertion(med.to_result() == 2);
  }

  // Same value several times + one different higher value with
  // subdivided interval.
  {
    accu::stat::median_interval<2> med(1, 4);
    med.take(1);
    med.take(1);
    med.take(1);
    med.take(2);
    mln_assertion(med.to_result() == 1);
  }

  // Same value several times + one different lower value with
  // subdivided interval.
  {
    accu::stat::median_interval<2> med(1, 4);
    med.take(1);
    med.take(2);
    med.take(2);
    med.take(2);
    mln_assertion(med.to_result() == 2);
  }

  // Odd number of values
  {
    accu::stat::median_interval<1> med(1, 8);
    med.take(1);
    med.take(5);
    med.take(2);
    med.take(8);
    med.take(7);
    mln_assertion(med.to_result() == 5);
  }

  // Even number of values
  {
    accu::stat::median_interval<1> med(1, 8);
    med.take(1);
    med.take(6);
    med.take(2);
    med.take(8);
    med.take(7);
    med.take(2);
    mln_assertion(med.to_result() == 4);
  }

  // Based on a subdivided interval
  {
    accu::stat::median_interval<2> med(3.5, 6.5);

    med.take(4.5);
    med.take(6);
    med.take(4.5);
    med.take(6);

    mln_assertion(med.to_result() == 5.25);
  }

  // Using 0 as value.
  {
    accu::stat::median_interval<2> med(0, 4);

    med.take(0);
    med.take(1);
    med.take(2);
    med.take(3);
    med.take(4);

    mln_assertion(med.to_result() == 2);
  }

  // Using 0 as value.
  {
    accu::stat::median_interval<2> med(0, 5);

    med.take(0);
    med.take(1);
    med.take(2);
    med.take(3);
    med.take(4);
    med.take(5);

    mln_assertion(med.to_result() == 2.5);
  }

  // Integer Interval with negative values.
  {
    accu::stat::median_interval<1> med(-2, 3);

    med.take(-1);
    med.take(-2);
    med.take(2);
    med.take(3);

    mln_assertion(med.to_result() == 0.5);
  }

  // Subdivided Interval with negative values.
  {
    accu::stat::median_interval<2> med(-2, 3);

    med.take(-1);
    med.take(-2);
    med.take(2);
    med.take(3);

    mln_assertion(med.to_result() == 0.5);
  }

  // Subdivided Interval with negative values.
  {
    accu::stat::median_interval<1> med(-2, 3);

    med.take(-1);
    med.take(-2);
    med.take(2);
    med.take(3);
    mln_assertion(med.to_result() == 0.5);
  }

}
