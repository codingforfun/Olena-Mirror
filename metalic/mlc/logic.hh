// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_META_LOGIC_HH
# define OLENA_META_LOGIC_HH

# include <mlc/basics.hh>

namespace oln {

  namespace meta {

    // FIXME: I hardly see the purpose of these structs.
    // What's the point of writing
    //   l_not<l_and<A, B>::ret>::ensure()
    // when one can simply do
    //   is_true<!(A && B)>::ensure()
    // ? -- adl.

    template<bool b>
    struct l_not
    {
      enum { ret = ! b };
      static void ensure() { is_true<ret>::ensure(); };
    };

    template<bool b1, bool b2>
    struct l_and
    {
      enum { ret = (b1 & b2) };
      static void ensure() { is_true<ret>::ensure(); };
    };

    template<bool b1, bool b2>
    struct l_or
    {
      enum { ret = (b1 | b2) };
      static void ensure() { is_true<ret>::ensure(); };
    };

    template<bool b1, bool b2>
    struct l_xor
    {
      enum { ret = (b1 ^ b2) };
      static void ensure() { is_true<ret>::ensure(); };
    };

    // FIXME: define l_and_not, etc.?

  } // meta

} // oln

#endif // OLENA_META_LOGIC_HH
