// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef NTG_CORE_INTERNAL_GLOBAL_OPS_HH
# define NTG_CORE_INTERNAL_GLOBAL_OPS_HH

/*
  This header defines global operators (arithmetic, logical, and
  comparison). They are examined when at least one ntg type is
  involved. They are required to have a precise dispatch, you can
  check the documentation for more details.
*/

# include <mlc/contract.hh>
# include <mlc/is_a.hh>

# include <ntg/real/builtin_float.hh>
# include <ntg/real/optraits_builtin_int.hh>
# include <ntg/core/internal/global_ops_traits.hh>
# include <ntg/core/macros.hh>
# include <ntg/core/value.hh>
# include <ntg/utils/debug.hh>

// macros defs
# include <ntg/core/internal/global_ops_defs.hh>

// min and max
# include <algorithm>

namespace ntg
{

  /*
    Global operators are defined in the internal namespace to avoid
    any possible clash, even when "using namespace ntg" directive is
    used.
  */

  namespace internal
  {

    /*------------------------.
    | Arithmetic assignements |
    `------------------------*/

    GLOBAL_ASSIGN_OP(operator+=, plus_equal)
    GLOBAL_ASSIGN_OP_BUILTIN_INT(operator+=, plus_equal)
    GLOBAL_ASSIGN_OP_BUILTIN_FLOAT(operator+=, plus_equal)

    GLOBAL_ASSIGN_OP(operator-=, minus_equal)
    GLOBAL_ASSIGN_OP_BUILTIN_INT(operator-=, minus_equal)
    GLOBAL_ASSIGN_OP_BUILTIN_FLOAT(operator-=, minus_equal)

    GLOBAL_ASSIGN_OP(operator*=, times_equal)
    GLOBAL_ASSIGN_OP_BUILTIN_INT(operator*=, times_equal)
    GLOBAL_ASSIGN_OP_BUILTIN_FLOAT(operator*=, times_equal)

    GLOBAL_ASSIGN_OP(operator/=, div_equal)
    GLOBAL_ASSIGN_OP_BUILTIN_INT(operator/=, div_equal)
    GLOBAL_ASSIGN_OP_BUILTIN_FLOAT(operator/=, div_equal)

    GLOBAL_ASSIGN_OP(operator%=, mod_equal)
    GLOBAL_ASSIGN_OP_BUILTIN_INT(operator%=, mod_equal)

    /*---------------------.
    | Logical assignements |
    `---------------------*/

    // FIXME: implement |=, &= and ^= in optraits<bool>.

    GLOBAL_ASSIGN_OP(operator|=, logical_or_equal)
    //    GLOBAL_ASSIGN_OP_BUILTIN_BOOL(operator|=, logical_or_equal)

    GLOBAL_ASSIGN_OP(operator&=, logical_and_equal)
    //    GLOBAL_ASSIGN_OP_BUILTIN_BOOL(operator&=, logical_and_equal)

    GLOBAL_ASSIGN_OP(operator^=, logical_xor_equal)
    //    GLOBAL_ASSIGN_OP_BUILTIN_BOOL(operator^=, logical_xor_equal)

    /*----------------------.
    | Arithmetic operations |
    `----------------------*/

    GLOBAL_ARITH_OP(operator+, plus)
    GLOBAL_ARITH_OP(operator-, minus)
    GLOBAL_ARITH_OP(operator*, times)
    GLOBAL_ARITH_OP(operator/, div)
    GLOBAL_ARITH_OP(operator%, mod)

    /*------------------.
    | Logical operators |
    `------------------*/

    GLOBAL_LOGICAL_OP(operator|, logical_or)
    GLOBAL_LOGICAL_OP(operator&, logical_and)
    GLOBAL_LOGICAL_OP(operator^, logical_xor)

    /*---------------------.
    | Comparison operators |
    `---------------------*/

    GLOBAL_CMP_OP(operator==, cmp_eq)

    template <class T1, class T2> inline
    bool
    operator!=(const T1& lhs, const T2& rhs)
    { return !(lhs == rhs); }

    // >, >= and <= operators are defined using <. They are not
    // implicitely defined so we have to do so by hand.

    GLOBAL_CMP_OP(operator<, cmp_lt)

    template <class T1, class T2> inline
    bool
    operator>(const T1& lhs, const T2& rhs)
    { return rhs < lhs; }

    template <class T1, class T2> inline
    bool
    operator>=(const T1& lhs, const T2& rhs)
    { return !(lhs < rhs); }

    template <class T1, class T2> inline
    bool
    operator<=(const T1& lhs, const T2& rhs)
    { return !(rhs < lhs); }

    /*-----------------------------.
    | Arithmetical unary operators |
    `-----------------------------*/

    // plus

    template <class T> inline
    const T&
    operator+(const T& val)
    {
      return val;
    }

    template<class T> inline
    const T&
    operator++(T& val)
    {
      val += ntg_unit_val(T);
      return val;
    }

    template<class T> inline
    T
    operator++(T& val, int)
    {
      T tmp(val);
      val += ntg_unit_val(T);
      return tmp;
    }

    // minus

    template <class T> inline
    const ntg_signed_type(T)
    operator-(const T& val)
    {
      typedef ntg_signed_type(T) signed_type;
      return static_cast<signed_type>(ntg_zero_val(T)) - val;
    }

    template<class T> inline
    const T& operator--(T& val)
    {
      val -= ntg_unit_val(T);
      return val;
    }

    template<class T> inline
    T operator--(T& val, int)
    {
      T tmp(val);
      val -= ntg_unit_val(T);
      return tmp;
    }

    /*------------------.
    | Min/Max operators |
    `------------------*/

    /*
      These operators are slighly different from standard library ones
      since they can operate on values of 2 different types.
    */

    template <class T1, class T2> inline
    ntg_return_type(min, T1, T2)
    min (const T1& lhs, const T2& rhs)
    {
      typedef ntg_return_type(max, T1, T2) result_type;
      return (lhs < rhs) ? result_type(lhs) : result_type(rhs);
    }

    template <class T1, class T2> inline
    ntg_return_type(max, T1, T2)
    max (const T1& lhs, const T2& rhs)
    {
      typedef ntg_return_type(max, T1, T2) result_type;
      return (lhs > rhs) ? result_type(lhs) : result_type(rhs);
    }

  } // end of internal.

  // FIXME: those are needed to handle max and min between two builtin
  // types. Maybe std::min and std::max should be extended to work
  // with builtin types?
  using internal::min;
  using internal::max;

} // end of ntg.

#endif // ndef NTG_CORE_INTERNAL_GLOBAL_OPS_HH
