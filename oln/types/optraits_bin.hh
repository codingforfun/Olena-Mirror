// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_OPTRAITS_BIN_HH
# define OLENA_VALUE_OPTRAITS_BIN_HH

# include <oln/types/global_ops.hh>
# include <oln/types/optraits.hh>
# include <oln/types/bin.hh>

# include <string>
# include <sstream>

namespace oln
{
  
  //
  //  optraits for bin
  //
  /////////////////////

  template <>
  struct optraits<bin> : public optraits_enum<bin>
  {
    // logical assignement operators

    static bin& logical_or_equal(bin& lhs, const bin& rhs)
    {
      lhs = lhs.value() | rhs.value();
      return lhs;
    }

    static bin& logical_and_equal(bin& lhs, const bin& rhs)
    {
      lhs = lhs.value() & rhs.value();
      return lhs;
    }

    static bin& logical_xor_equal(bin& lhs, const bin& rhs)
    {
      lhs = lhs.value() ^ rhs.value();
      return lhs;
    }

    // logical binary ops

    static bin logical_or(const bin& lhs, const bin& rhs)
    {
      bin tmp(lhs);
      tmp |= rhs;
      return tmp;
    }

    static bin logical_and(const bin& lhs, const bin& rhs)
    {
      bin tmp(lhs);
      tmp |= rhs;
      return tmp;
    }

    static bin logical_xor(const bin& lhs, const bin& rhs)
    {
      bin tmp(lhs);
      tmp |= rhs;
      return tmp;
    }


    // comparisons

    static bool cmp_lt(const bin& lhs, const bin& rhs)
    {
      return lhs.value() < rhs.value();
    }

    static bool cmp_eq(const bin& lhs, const bin& rhs)
    {
      return lhs.value() == rhs.value();
    }

    // debug
    static std::string name() { return "bin"; }
  };


  namespace internal 
  {

    //
    //
    //  Operators traits
    //
    /////////////////////

    template <class T>
    struct operator_logical_traits<bin, T>
    {
      enum { commutative = true };
      typedef bin ret;
      typedef bin impl;
    };

    template <class T>
    struct operator_cmp_traits<bin, T>
    {
      enum { commutative = true };
      typedef bin impl;
    };

  } // end of namespace internal
    
} // end of namespace oln

#endif // ndef OLENA_VALUE_OPTRAITS_BIN_HH
