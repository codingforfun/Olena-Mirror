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

#ifndef NTG_CORE_INTERNAL_GLOBAL_OPS_TRAITS_HH
# define NTG_CORE_INTERNAL_GLOBAL_OPS_TRAITS_HH

/*
  Tools to determine return type of operators given the parameters
  types. For each type, one has to specialize the traits
  operator_xxx, and then use deduce_from_traits<operator_xxx, T1, T2>
  as return type for operatorxxx(T1, T2) functions.
*/

# include <ntg/core/macros.hh>
# include <ntg/core/type_traits.hh>

# include <mlc/bool.hh>

namespace ntg {

  namespace internal {

    /*-----------.
    | Meta tools |
    `-----------*/

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

    template <class T>
    struct is_defined { enum { ret = true }; };
    template <>
    struct is_defined<undefined_traits> { enum { ret = false }; };

    /*
      get_order, get_order_inv

      These structs are used to inverse input types (if the operator
      is commutative) and to specify whether input types should be
      converted into another type (eg. the corresponding ntg_type).
    */

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

    /*---------------------------.
    | Operator traits definition |
    `---------------------------*/
    //! Give return type for operators, depending on the input types.
    /*!
       operator_traits traits should not be used directly. Instead one
       should use deduce_from_traits, see comments below for more
       details.

       These traits defines 3 properties:

       * commutative (enum): Tells whether the operator is commutative
         or not.

       * ret (typedef): Specifies the return type.

       * impl (typedef): Specifies the type which implement the
         operator, that is, the type T such as optraits<T>::operatorX
         is the good implementation.

	 To specify the concerned operator, one empty class represent
	 each operator. For example, to specify the traits associated
	 to the + operator with T1 and T2 as arguments:

	 template <class T1, class T2>
	 struct operator_traits<operator_plus, T1, T2>
	 {
	   ...
	 }
    */

    template <class Op, class T, class U>
    struct operator_traits
    {
      enum { commutative = false };
      typedef undefined_traits ret;
      typedef undefined_traits impl;
    };

    struct operator_plus {};
    struct operator_minus {};
    struct operator_times {};
    struct operator_div {};
    struct operator_mod {};
    struct operator_logical {};
    struct operator_cmp {};
    struct operator_min {};
    struct operator_max {};

    /*-----------------------------------.
    | deduce_from_traits<operator, T, U> |
    `-----------------------------------*/
    //! Find the good operator_traits, following a simple algorithm.
    /*!
      deduce_from_traits should generally be used instead of
      operator_traits. Indeed, it has a handy algorithm to find return
      types:

      1) Convert T1 and T2 to ntg types.
      2) Check if traits<T1, T2> is defined.
      3) Else, check if traits<T2, T1> is defined
         AND traits<T2, T1>::commutative is true.

      deduce_from_traits defines several types:

      - lhs_type and rhs_type: The types into which the first and
        second paramaters should be converted before called the
        implementation.

      - ret: The return type.

      - impl: The implementation type.

      - deduced_traits: A pointer to the good operator_traits<>
        specialization. This can be useful sometimes.
    */

    template <class Op, class T, class U>
    struct deduce_from_traits
    {
    private:
      typedef ntg_type(T) T1;
      typedef ntg_type(U) T2;

      typedef typename operator_traits<Op, T1, T2>::ret traits;
      typedef typename operator_traits<Op, T2, T1>::ret rev_traits;

      enum { can_invert = (operator_traits<Op, T2, T1>::commutative
			   && is_defined<rev_traits>::ret) };

      typedef typename
      mlc::if_<is_defined<traits>::ret,
		get_order<T1, T2>, typename
		mlc::if_<can_invert,
			 get_order_inv<T1, T2>,
			 meta_undefined_traits<undefined_traits>
                        >::ret
              >::ret deduced_type;

      typedef typename deduced_type::traits_lhs_type traits_lhs_type;
      typedef typename deduced_type::traits_rhs_type traits_rhs_type;

    public:
      typedef operator_traits<Op,
			      traits_lhs_type,
			      traits_rhs_type> deduced_traits;

      typedef typename deduced_type::lhs_type lhs_type;
      typedef typename deduced_type::rhs_type rhs_type;
      typedef typename deduced_traits::ret ret;
      typedef typename deduced_traits::impl impl;
    };

  } // end of internal.

} // end of ntg.

#endif // !NTG_CORE_INTERNAL_GLOBAL_OPS_TRAITS_HH
