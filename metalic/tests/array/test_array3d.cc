// Copyright (C) 2003  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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


#include <mlc/array/all.hh>
#include <iostream>
using std::cout;
using std::endl;

#include "check.hh"

using namespace mlc;

#define OK_OR_FAIL				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	fail = true;				\
      }

bool
check()
{
  bool fail = false;

  array3d< array3d_info< 2, 2, 3, 1>, int > foo = (ints_3d =  1, x(2), 3, lbrk,
						   4, 5, 6, pbrk,

						   7, 8, 9,
						   10, 11, 12, end);

  cout << endl << "Source:\n\n" << foo << endl;

  try {
    cout << "After a transposition:\n\n" << foo.transpose() << endl;
  }
  catch (not_implemented_yet) {
    cout << endl;
  }

  cout << "After a central symmetry:\n\n" << -foo << endl;

  if (1)
    OK_OR_FAIL;

  return fail;
}
