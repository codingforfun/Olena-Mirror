// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef NTG_GLOBAL_OPS_TRAITS_HH
# define NTG_GLOBAL_OPS_TRAITS_HH

# include <mlc/bool.hh>

# include <ntg/core/type_traits.hh>

//
//  Operators traits : give return type, etc ..
// 
//  An operator traits must define : 
//   - ret  ==> return type of the operator
//   - impl ==> implementation type, the type from which 
//              optraits<type>::operator is implemented
//
//  <WARNING>
//  Do not use operator_xxx_traits directly, use deduce_from_traits
//
////////////////////////////////////////////////////////////////////

namespace ntg
{

  namespace internal
  {

    struct undefined_traits {};

    template <class T>
    struct meta_undefined_traits 
    {
      typedef undefined_traits ret;
      typedef undefined_traits lhs_type;
      typedef undefined_traits rhs_type;
      typedef undefined_traits traits_lhs_type;
      typedef undefined_traits traits_rhs_type;
    };

    //
    //  Operators traits declaration
    //  DO NOT USE DIRECTLY, USE DEDUCE_FROM_TRAITS
    //
    ////////////////////////////////////////////////
    

    template <class Op, class T, class U>
    struct operator_traits
    {
      enum { commutative = false };
      typedef undefined_traits ret; 
      typedef undefined_traits impl; 
    };

    // plus    
    struct operator_plus {};

    // minus    
    struct operator_minus {};  

    // times    
    struct operator_times {};

    // div
    struct operator_div {};

    // mod
    struct operator_mod {};
    
    // logical ops
    struct operator_logical {};

    // comparison
    struct operator_cmp {};

    // min
    struct operator_min {};

    // max
    struct operator_max {};

    //
    //  Traits deduction algorithm:
    //
    //  1) Convert T1 and T2 to ntg types.
    //  2) Check if traits<T1, T2> is ok.
    //  3) Else, check if traits<T2, T1> ok AND traits<T2, T1>::commutative 
    //     is true.
    //
    //////////////////////////////////////////////////////////////////////////

    template <class T>
    struct is_defined { enum { ret = true }; };
    template <>
    struct is_defined<undefined_traits> { enum { ret = false }; };
    
    //
    //  Struct get_order and get_order_inv
    //
    //  traits_xhs_type : types for traits use
    //  xhs_type : needed to say if we need to cast into oln_type
  
    template <class T1, class T2>
    struct get_order
    {
      typedef T1 traits_lhs_type;
      typedef T2 traits_rhs_type;

      typedef T1 lhs_type;
      typedef T2 rhs_type;
    };

    template <class T1, class T2>
    struct get_order_inv
    {
      typedef T2 traits_lhs_type;
      typedef T1 traits_rhs_type;

      typedef T1 lhs_type;
      typedef T2 rhs_type;
    };
   
    template <class Op, class T, class U>
    struct deduce_from_traits
    {
      typedef ntg_type(T) T1;
      typedef ntg_type(U) T2;

      typedef typename operator_traits<Op, T1, T2>::ret traits;
      typedef typename operator_traits<Op, T2, T1>::ret rev_traits;

      typedef typename
      mlc::if_<is_defined<traits>::ret,
		get_order<T1, T2>, typename
		mlc::if_<operator_traits<Op, T2, T1>::commutative && is_defined<rev_traits>::ret,
			 get_order_inv<T1, T2>,
			 meta_undefined_traits<undefined_traits>
                        >::ret_t
              >::ret_t deduced_type;
				    
      typedef typename deduced_type::lhs_type lhs_type;
      typedef typename deduced_type::rhs_type rhs_type;
      typedef typename deduced_type::traits_lhs_type traits_lhs_type;
      typedef typename deduced_type::traits_rhs_type traits_rhs_type;

      typedef typename operator_traits<Op, traits_lhs_type, traits_rhs_type>::ret ret;
      typedef typename operator_traits<Op, traits_lhs_type, traits_rhs_type>::impl impl;
      typedef operator_traits<Op, traits_lhs_type, traits_rhs_type> deduced_traits;
    };

  } // end of internal.

} // end of ntg.

#endif // ndef NTG_GLOBAL_OPS_TRAITS_HH
