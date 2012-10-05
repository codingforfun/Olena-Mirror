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

#include <mln/detection/corner/achard.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/essential.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/data/convert.hh>
#include <mln/literal/colors.hh>

#include "tests/data.hh"

using namespace mln;

int main()
{
  image2d<value::int_u8> input;
  image2d<value::rgb8> output;

  p_array<point2d> corners;

  io::pgm::load(input, MLN_TESTS_IMG_DIR "/test_corner_detection.pgm");
  corners = mln::detection::corner::achard(input);
  
  output = mln::data::convert(value::rgb8(), input);

  data::fill( (output | corners).rw(), literal::red);
  io::ppm::save(output, "achard-result.ppm");

  {
    mln_assertion(corners.nsites() == 1);
    mln_assertion(corners[0] == point2d(40,65));
  }
}
