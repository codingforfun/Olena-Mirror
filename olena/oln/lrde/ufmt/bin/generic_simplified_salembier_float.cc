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

#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/utils/timer.hh>

#include <oln/lrde/ufmt/generic_simplified_salembier.hh>
#include <oln/lrde/efigi/io.hh>


void usage(char* argv[])
{
  std::cerr
    << "generic simplified max-tree computation with Salembier's flooding"
    << " (float version)." << std::endl
    << "usage: " << argv[0] << " input.pfm c" << std::endl;
  exit(1);
}


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

  typedef lrde::ufmt::generic_simplified_salembier<image_t> algorithm_t;

  algorithm_t run(input, nbh);
  utils::timer t;
  t.start();
  run.go();
  t.stop();
  std::cout << "n level roots = " << run.n_level_roots() << std::endl
	    << "elapsed time  = " << t.last_time() << std::endl;
}
