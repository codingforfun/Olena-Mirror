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


#include <iostream>
#include <fstream>

int one_center(std::ostream&, int, int*, int);
int if_center(std::ostream&, int, int*, int);
int all_centers(std::ostream&, int, int*, int);
int no_comma(std::ostream&, int, int*, int);
int no_element(std::ostream&, int, int*, int);
int no_lbreak(std::ostream&, int, int*, int);
int one_lbreak(std::ostream&, int, int*, int);
int all_lbreak(std::ostream&, int, int*, int);
int too_much_lbreak(std::ostream&, int, int*, int);
int twice_lbreak(std::ostream&, int, int*, int);
int no_pbreak(std::ostream&, int, int*, int);
int one_pbreak(std::ostream&, int, int*, int);
int all_pbreak(std::ostream&, int, int*, int);
int normal_end(std::ostream&, int, int*, int);
int no_end(std::ostream&, int, int*, int);
int end_elt(std::ostream&, int, int*, int);
int end_lbrk(std::ostream&, int, int*, int);
int end_center(std::ostream&, int, int*, int);
int end_starter(std::ostream&, int, int*, int);
