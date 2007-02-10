// Copyright (C) 2001, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_MACROS_HH
# define OLENA_CORE_MACROS_HH

/* This is called for_all, not for_each, otherwise calls to the
   for_each() STL function would fail.  */
# define for_all(p) for((p) = mlc::begin; (p) != mlc::end; ++(p))

/* Same as for_all, but the iterattion has already begun.  We use 'if
   (p == end) {} else', not 'if (p != end)' so that an 'else'
   following the for_all_remaining macro will match the expected if.
   */
# define for_all_remaining(p) \
  if (p == mlc::end) {} else while(++(p), ((p) != mlc::end))

#endif // OLENA_CORE_MACROS_HH
