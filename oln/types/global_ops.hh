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

#ifndef OLENA_VALUE_GLOBAL_OPS_HH
# define OLENA_VALUE_GLOBAL_OPS_HH

# include <oln/core/contract.hh>
# include <oln/meta/type.hh>

# include <oln/types/to_oln.hh>
# include <oln/types/rec_value.hh>
# include <oln/types/global_ops_traits.hh>
# include <oln/types/optraits.hh>

// macros defs
# include <oln/types/global_ops_defs.hh>

//
//  Global ops
//  These operators cannot be called if one value_type is not involved, 
//  as there are uncapsulated in a namespace
//
///////////////////////////////////////////////////////////////////////

namespace oln
{

  //
  // Arithmetic assignements
  //
  ///////////////////////////


  GLOBAL_ASSIGN_OP(operator+=, plus_equal);
  GLOBAL_ASSIGN_OP(operator-=, minus_equal);
  GLOBAL_ASSIGN_OP(operator*=, times_equal);
  GLOBAL_ASSIGN_OP(operator/=, div_equal);
  GLOBAL_ASSIGN_OP(operator%=, mod_equal);


  //
  //  Logical assignements
  //
  /////////////////////////

  GLOBAL_ASSIGN_OP(operator|=, logical_or_equal);
  GLOBAL_ASSIGN_OP(operator&=, logical_and_equal);
  GLOBAL_ASSIGN_OP(operator^=, logical_xor_equal);


  //
  //  Arithmetic operations
  //
  //////////////////////////

//   GLOBAL_ARITH_OP(operator+, plus);
//   GLOBAL_ARITH_OP(operator-, minus);
//   GLOBAL_ARITH_OP(operator*, times);
//   GLOBAL_ARITH_OP(operator/, div);
//   GLOBAL_ARITH_OP(operator%, mod);

  
  //
  //  Logical operators
  //
  ///////////////////////

  GLOBAL_LOGICAL_OP(operator|, logical_or);
  GLOBAL_LOGICAL_OP(operator&, logical_and);
  GLOBAL_LOGICAL_OP(operator^, logical_xor);


  //
  //  Comparison operators
  //
  /////////////////////////

  // eq

  GLOBAL_CMP_OP(operator==, cmp_eq);

  // neq

  template <class T1, class T2>
  bool operator!=(const T1& lhs, const T2& rhs)
  { return !(lhs == rhs); }


  // cmp_lt

  GLOBAL_CMP_OP(operator<, cmp_lt);
  

  // cmp_gt

  template <class T1, class T2>
  bool operator>(const T1& lhs, const T2& rhs)
  { return rhs < lhs; }  

  
  // cmp_ge

  template <class T1, class T2>
  bool operator>=(const T1& lhs, const T2& rhs)
  { return !(lhs < rhs); }

  
  // cmp_le
  
  template <class T1, class T2>
  bool operator<=(const T1& lhs, const T2& rhs)
  { return !(rhs < lhs); }


  //
  //  Arithmetical unary operators
  //
  /////////////////////////////////

  // plus
  
  template <class T> inline
  const T& operator+(const T& val)
  { 
    return val;
  }

  template<class T> inline
  const T& operator++(T& val)
  {
    val += optraits<T>::unit();
    return rhs;
  }

  template<class T> inline
  T operator++(T& val, int)
  {
    T tmp(val);
    val += optraits<T>::unit();
    return tmp;
  }

  // minus

  template <class T> inline
  const T operator-(const T& val)
  { 
    return optraits<T>::zero() - val; 
  }

  template<class T> inline
  const T& operator--(T& val)
  {
    val -= optraits<T>::unit();
    return rhs;
  }

  template<class T> inline
  T operator--(T& val, int)
  {
    T tmp(val);
    val -= optraits<T>::unit();
    return tmp;
  }
    
} // end of namespace oln

#endif // ndef OLENA_VALUE_GLOBAL_OPS_HH
