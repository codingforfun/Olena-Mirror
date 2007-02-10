// Copyright (C) 2006  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, Boston, MA 02110-1301, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#include <cstdlib>
#include <functional>

#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/utils/timer.hh>

#include <oln/lrde/ufmt/generic_salembier.hh>
#include <oln/lrde/efigi/io.hh>


void usage(char* argv[])
{
  std::cerr << "generic max-tree computation with salembier's flooding"
	    << " (float version, with a different comparison function)."
	    << std::endl
	    << "usage: " << argv[0] << " input.pfm c" << std::endl;
  exit(1);
}

// A comparison functor on floats, inspired from
// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
class almost_greater : public std::binary_function<float, float, bool>
{
public:
  almost_greater(float max_relative_error = 0.0000001f,
		 float max_absolute_error = 3 * FLT_MIN) :
    max_relative_error_ (max_relative_error),
    max_absolute_error_ (max_absolute_error)
  {
  }

  bool
  operator() (float a, float b)
  {
    // Return false if A and B are close numbers, to ensure that the
    // equivalence of the ``strict'' weak ordering
    // (http://www.sgi.com/tech/stl/StrictWeakOrdering.html) is
    // preserved (i.e. to let STL containers using this function
    // consider A and B as equal when their difference is small).
    if (almost_equal_relative_or_absolute(a, b))
      return false;
    return a > b;
  }

private:
  bool almost_equal_relative_or_absolute(float A, float B)
  {
    if (fabs(A - B) < max_absolute_error_)
      return true;
    float relative_error;
    if (fabs(B) > fabs(A))
      relative_error = fabs((A - B) / B);
    else
      relative_error = fabs((A - B) / A);
    if (relative_error <= max_relative_error_)
      return true;
    return false;
  }

private:
  float max_relative_error_;
  float max_absolute_error_;
};


int main(int argc, char* argv[])
{
  if (argc != 3)
    usage(argv);

  using namespace oln;
  typedef image2d<float> image_t;

  image_t input = efigi::load_pfm(argv[1]);
  assert (input.has_impl());

  int c = atoi(argv[2]);
  if (not (c == 2 or c == 4 or c == 8))
    usage(argv);

  neighborhood2d nbh;
  if (c == 2)
    nbh.add(dpoint2d(0, 1));
  else
    nbh = c == 4 ? neighb_c4() : neighb_c8();

  typedef lrde::ufmt::generic_salembier<image_t, almost_greater> algorithm_t;

  algorithm_t run(input, nbh);
  utils::timer t;
  t.start();
  run.go();
  t.stop();
  std::cout << "n level roots = " << run.n_level_roots() << std::endl
	    << "elapsed time  = " << t.last_time() << std::endl;
}
