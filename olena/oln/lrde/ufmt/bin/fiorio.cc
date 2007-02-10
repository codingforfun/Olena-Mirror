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
#include <ntg/color/rgb.hh>

#include <oln/basics2d.hh>
#include <oln/level/fill.hh>

// Max-tree computation base on Fiorio's and Gustedt's algorithm.
#if defined(FIORIO_VERSION) && FIORIO_VERSION == 3
# include <oln/lrde/ufmt/fiorio-3.hh>
#else
# if defined(FIORIO_VERSION) && FIORIO_VERSION == 2
#  include <oln/lrde/ufmt/fiorio-2.hh>
# else
#  include <oln/lrde/ufmt/fiorio-1.hh>
# endif
#endif


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm"
	    << std::endl
	    << "Max-tree computation with Fiorio's and Gustedt's algorithm."
	    << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv);

  typedef oln::image2d<ntg::int_u8> image_type;
  image_type input = oln::io::load(argv[1]);

  typedef oln::lrde::ufmt::fiorio<image_type> algorithm_type;
  algorithm_type run(input);
  std::cout << "npoint                        = " << run.ima().npoints()
	    << std::endl;
  run.go();
  std::cout << "n level roots (first method)  = " << run.n_level_roots1()
	    << std::endl
	    << "n level roots (second method) = " << run.n_level_roots2()
	    << std::endl;
}
