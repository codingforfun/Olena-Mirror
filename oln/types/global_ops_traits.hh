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

#ifndef OLENA_VALUE_GLOBAL_OPS_TRAITS_HH
# define OLENA_VALUE_GLOBAL_OPS_TRAITS_HH

# include <oln/meta/basics.hh>

# include <oln/types/to_oln.hh>
# include <oln/types/typetraits.hh>

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

namespace oln
{
  namespace internal
  {
    class undefined_traits {};

    template <class T>
    class meta_undefined_traits 
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
    

    // plus
    
    template <class T, class U>
    struct operator_plus_traits
    { 
      enum { commutative = false };
      typedef undefined_traits ret; 
      typedef undefined_traits impl; 
    };  

    // minus
    
    template <class T, class U>
    struct operator_minus_traits
    { 
      enum { commutative = false };
      typedef undefined_traits ret; 
      typedef undefined_traits impl; 
    };  

    // times
    
    template <class T, class U>
    struct operator_times_traits
    { 
      enum { commutative = false };
      typedef undefined_traits ret; 
      typedef undefined_traits impl; 
    };  

    // div
    
    template <class T, class U>
    struct operator_div_traits
    { 
      enum { commutative = false };
      typedef undefined_traits ret; 
      typedef undefined_traits impl; 
    };  

    // mod
    
    template <class T, class U>
    struct operator_mod_traits
    { 
      enum { commutative = false };
      typedef undefined_traits ret; 
      typedef undefined_traits impl; 
    };  

    // logical ops
    
    template <class T, class U>
    struct operator_logical_traits
    { 
      enum { commutative = false };
      typedef undefined_traits ret; 
      typedef undefined_traits impl; 
    };  

    // comparison
    
    template <class T, class U>
    struct operator_cmp_traits
    { 
      enum { commutative = false };
      typedef undefined_traits ret;
      typedef undefined_traits impl; 
    };
    


    //
    //  Traits deducing algorithm
    //
    //  1) Check if traits<T1, T2> is ok
    //  2) Check if traits<T2, T1> ok AND traits<T2, T1>::commutative is true
    //  3) Check if traits<to_oln<T1>, to_oln<T2> > ok
    //  4) Check if traits<to_oln<T2>, to_oln<T1> > ok AND commutative is true
    //
    //////////////////////////////////////////////////////////////////////////

    //
    //  Dev note : 
    //  When using g++ >= 3.1, using deduce_from_traits<operator_xxx_traits, T, U>
    //  may fail at compile time, using 
    //  deduce_from_traits<oln::internal::operator_xxx_traits, T, U> should resolve
    //  the problem.
    //
  
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
   

    template <template <class, class> class traits, class T1, class T2>
    struct deduce_from_traits
    {
      typedef typename typetraits<T1>::op_traits T1_op;
      typedef typename typetraits<T2>::op_traits T2_op;

      typedef typename internal::to_oln<T1_op>::ret T1_oln;
      typedef typename internal::to_oln<T2_op>::ret T2_oln;

      typedef typename
      meta::if_<is_defined<typename traits<T1_op, T2_op>::ret>::ret,
		get_order<T1_op, T2_op>, typename
		meta::if_<is_defined<typename traits<T2_op, T1_op>::ret>::ret && traits<T2_op, T1_op>::commutative,
			  get_order_inv<T1_op, T2_op>, typename
			  meta::if_<is_defined<typename traits<T1_oln, T2_oln>::ret>::ret,
				    get_order<T1_oln, T2_oln>, typename
				    meta::if_<is_defined<typename traits<T2_oln, T1_oln>::ret>::ret && traits<T2_oln, T1_oln>::commutative,
					      get_order_inv<T1_oln, T2_oln>,
					      meta_undefined_traits<undefined_traits>
					     >::ret_t
                                   >::ret_t
                         >::ret_t
               >::ret_t deduced_type;
				    
      typedef typename deduced_type::lhs_type lhs_type;
      typedef typename deduced_type::rhs_type rhs_type;
      typedef typename deduced_type::traits_lhs_type traits_lhs_type;
      typedef typename deduced_type::traits_rhs_type traits_rhs_type;

      typedef typename traits<traits_lhs_type, traits_rhs_type>::ret ret;
      typedef typename traits<traits_lhs_type, traits_rhs_type>::impl impl;
    };

  } // end of namespace internal  
} // end of namespace oln

#endif // ndef OLENA_VALUE_GLOBAL_OPS_TRAITS_HH
