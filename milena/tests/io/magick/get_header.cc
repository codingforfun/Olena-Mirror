// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#include <iostream>
#include <mln/io/magick/get_header.hh>
#include "tests/data.hh"

int main()
{
  using namespace mln;

  // PBM.
  {
    io::magick::magick_header
      header = io::magick::get_header(MLN_IMG_DIR "/tiny.pbm");
    mln_assertion(header.width == 18);
    mln_assertion(header.height == 22);
    mln_assertion(header.depth == 1);
    mln_assertion(header.image_type == 1);
  }

  // PGM.
  {
    io::magick::magick_header
      header = io::magick::get_header(MLN_IMG_DIR "/tiny.pgm");
    mln_assertion(header.width == 16);
    mln_assertion(header.height == 16);
    mln_assertion(header.depth == 8);
    mln_assertion(header.image_type == 2);
  }

  // PPM.
  {
    io::magick::magick_header
      header = io::magick::get_header(MLN_IMG_DIR "/tiny.ppm");
    mln_assertion(header.width == 16);
    mln_assertion(header.height == 16);
    mln_assertion(header.depth == 8);
    mln_assertion(header.image_type == 6);
  }

}
