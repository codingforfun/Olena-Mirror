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

#ifndef OLENA_OP_TRAITS_HH
# define OLENA_OP_TRAITS_HH

# include <oln/value/to_oln.hh>
# include <oln/meta/basics.hh>
# include <oln/math/macros.hh>

namespace oln {
  namespace internal {

    // Undefined type (for static exception recovering).
    class undefined_traits;

    template <class T>
    struct is_defined { enum { result = true }; };
    template <>
    struct is_defined<undefined_traits> { enum { result = false }; };

    // properties of operator
    template <
      template<class,class> class traits,
      class lhs_t,
      class rhs_t>
    struct properties_of_operator;

    // defining default operator
#define DEFINE_OPERATOR_DEFAULT_PROPERTIES(OP, FUN, COMMUTATIVITY)	\
    template <class lhs_t, class rhs_t>					\
    struct operator_##OP##_traits					\
    { typedef undefined_traits ret_t; };				\
    template <class lhs_t, class rhs_t>					\
    struct properties_of_operator<operator_##OP##_traits, lhs_t, rhs_t>	\
    {									\
      enum { commutative = COMMUTATIVITY };				\
      template <class output_t>						\
      static inline							\
      void operate(output_t& l, const rhs_t& r) { FUN; }		\
    };

    // Likewise, with no `operate' method.
#define DEFINE_OPERATOR_DEFAULT_PROPERTIES_NOOP(OP, COMMUTATIVITY)	\
    template <class lhs_t, class rhs_t>					\
    struct operator_##OP##_traits					\
    { typedef undefined_traits ret_t; };				\
    template <class lhs_t, class rhs_t>					\
    struct properties_of_operator<operator_##OP##_traits, lhs_t, rhs_t>	\
    {									\
      enum { commutative = COMMUTATIVITY };				\
    };

    DEFINE_OPERATOR_DEFAULT_PROPERTIES(times, l *= r, true);
    DEFINE_OPERATOR_DEFAULT_PROPERTIES(div,   l /= r, false);
    DEFINE_OPERATOR_DEFAULT_PROPERTIES(plus,  l += r, true);
    DEFINE_OPERATOR_DEFAULT_PROPERTIES(minus, l -= r, false);
    DEFINE_OPERATOR_DEFAULT_PROPERTIES_NOOP(min, true);
    DEFINE_OPERATOR_DEFAULT_PROPERTIES_NOOP(max, true);

    class inverse;
    class normal;

    template <class T, class LHS, class RHS>
    struct get_order
    {
      typedef undefined_traits lhs_t;
      typedef undefined_traits rhs_t;
    };

    template <class LHS, class RHS>
    struct get_order<normal, LHS, RHS>
    {
      typedef LHS lhs_t;
      typedef RHS rhs_t;
    };

    template <class LHS, class RHS>
    struct get_order<inverse, LHS, RHS>
    {
      typedef RHS lhs_t;
      typedef LHS rhs_t;
    };

    template
    <
      class T,
      class ret_t,
      template <class,class> class operator_traits
    >
    struct operate_functor;

    // Two ways to apply an operator
    template
    <
      class ret_t,
      template <class,class> class operator_traits
    >
    struct operate_functor<normal, ret_t, operator_traits>
      {
	template <class LHS, class RHS>
	static inline
	ret_t operate(const LHS& lhs, const RHS& rhs)
	{
	  ret_t output(lhs);
	  properties_of_operator<operator_traits, LHS, RHS>::operate(output, rhs);
	  return output;
	}
      };

    template
    <
      class ret_t,
      template <class,class> class operator_traits
    >
    struct operate_functor<inverse, ret_t, operator_traits>
      {
	template <class LHS, class RHS>
	static inline
	ret_t operate(const LHS& lhs, const RHS& rhs)
	{
	  ret_t output(rhs);
	  properties_of_operator<operator_traits, RHS, LHS>::operate(output, lhs);
	  return output;
	}
      };


    // Here is the algorithm to statically deduced all static informations that are :
    // | ret_t   -> The output type of the traits
    // | operate -> How the operator is appliied
    template <template<class, class> class operator_traits,
	      class LHS_,
	      class RHS_>
    struct deduced_from_traits
    {
      // Work on Olena's type exclusively.
      typedef typename oln::internal::to_oln<RHS_>::ret RHS;
      typedef typename oln::internal::to_oln<LHS_>::ret LHS;

      // What's the way to apply operator ?
      typedef typename
      meta::if_<is_defined<typename operator_traits<LHS, RHS>::ret_t>::result,
		normal,
		typename
		meta::if_<(is_defined<typename operator_traits<RHS, LHS>::ret_t>::result
			   && properties_of_operator<operator_traits, RHS, LHS>::commutative)
			  ,inverse
	                  ,typename
			  meta::if_<(is_defined<typename operator_traits<RHS, LHS>::ret_t>::result &&
				     !properties_of_operator<operator_traits,RHS, LHS>::commutative),
			              normal,
			              undefined_traits
				     >::ret_t
                         >::ret_t
               >::ret_t
      order_t;

      // Finally, what are the lhs's and the rhs's type ?
      // (if the operation is commutative, it could be different from LHS and RHS.)
      typedef typename get_order<order_t, LHS, RHS>::lhs_t lhs_t;
      typedef typename get_order<order_t, LHS, RHS>::rhs_t rhs_t;

      // What's the output of the operator ?
      typedef typename
      meta::if_<is_defined<typename operator_traits<LHS, RHS>::ret_t>::result,
		typename operator_traits<LHS,RHS>::ret_t,
		typename
		meta::if_<is_defined<typename operator_traits<RHS, LHS>::ret_t>::result,
			  typename operator_traits<RHS,LHS>::ret_t,
			  undefined_traits
			  >::ret_t
      >::ret_t
      ret_t;

      static inline
      ret_t
      operate(const LHS& lhs, const RHS& rhs)
      {
	return operate_functor<order_t, ret_t, operator_traits>
	  ::operate(lhs, rhs);
      }
    };
  } // end of internal
} // end of oln

#endif // OLENA_OP_TRAITS_HH
