// Copyright (C) 2002, 2003  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#include <oln/basics2d.hh>
#include <oln/level/cc.hh>
#include <iostream>
#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace mlc;
using namespace ntg;

#define OK_OR_FAIL				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	fail = true;				\
      }


bool
check(void)
{
  bool fail = false;

  image2d<bin>	src = load(rdata("object"));

  unsigned int	n_labels;

  std::cout << "----  level::frontp_connected_component<int_u8>  ----" << std::endl;
  image2d<int_u8> connected_comp =
    level::frontp_connected_component<int_u8>(src, neighb_c8(), n_labels);
  save(connected_comp, "cc");
  if (n_labels == 4)
    OK_OR_FAIL;

  std::cout << "----  level::get_n_cc(connected_comp) ----" << std::endl;
  int_u8 n_cc = level::get_n_cc(connected_comp);
  if (n_cc == n_labels - 1)
    OK_OR_FAIL;

  std::cout << "----  level::frontp_connected_component<int_u8>  ----" << std::endl;
  image2d<bin> cc_2 = level::extract_i_cc(connected_comp, 2);
  save(cc_2, "2nd_cc");
  connected_comp = level::frontp_connected_component<int_u8>(cc_2, neighb_c8(), n_labels);
  if (n_labels == 2) //background + object
    OK_OR_FAIL;

  return fail;
}
