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

#ifndef OLENA_VALUE_OPTRAITS_SCALAR_DEFS_HH
# define OLENA_VALUE_OPTRAITS_SCALAR_DEFS_HH

//
// macros used in optraits_scalar definition
//
/////////////////////////////////////////////

# define ASSIGN_SCALAR_OPERATOR(Name, Op)						\
  template <class T1, class T2>	inline							\
  static T1& Name(T1& lhs, const T2& rhs)						\
  {											\
    is_a(optraits<T1>, oln::optraits_scalar)::ensure();					\
    is_a(optraits<T2>, oln::optraits_scalar)::ensure();					\
											\
    return Name##_impl<T1,T2>(lhs, rhs);						\
  }											\
											\
  template <class T1, class T2>	inline							\
  static T1& Name##_impl(rec_scalar<T1>& lhs, const rec_scalar<T2>& rhs)		\
  {											\
    typedef typename typetraits<T1>::behaviour_type behaviour_type;			\
    lhs.self() = behaviour_type::check_##Name(lhs.self().value(), rhs.self().value());	\
    return lhs.self();									\
  }											\
											\
  template <class T1, class T2>	inline							\
  static T1& Name##_impl(rec_scalar<T1>& lhs, const type::any_const_class<T2> rhs)	\
  {											\
    typedef typename typetraits<T1>::behaviour_type behaviour_type;			\
    lhs.self() = behaviour_type::check_##Name(lhs.self().value(), rhs.self());		\
    return lhs.self();									\
  }											\
											\
  template <class T1, class T2>	inline							\
  static T1& Name##_impl(type::any_class<T1> lhs, const rec_scalar<T2>& rhs)		\
  {											\
    typedef typename typetraits<T1>::behaviour_type behaviour_type;			\
    lhs.self() = behaviour_type::check_##Name(lhs.self(), rhs.self().value());		\
    return lhs.self();									\
  }

# define ARITH_SCALAR_OPERATOR(Name, Op)					\
  template <class T1, class T2>	inline 						\
  static typename								\
  internal::deduce_from_traits<internal::operator_##Name##_traits, T1, T2>::ret	\
  Name(const T1& lhs, const T2& rhs)						\
  {										\
    is_a(optraits<T1>, oln::optraits_scalar)::ensure();				\
    is_a(optraits<T2>, oln::optraits_scalar)::ensure();				\
										\
    typedef typename								\
      internal::deduce_from_traits<internal::operator_##Name##_traits,		\
      T1, T2>::ret return_type;							\
    return_type result(lhs);							\
    result Op rhs;								\
    return result;								\
  }


# define CMP_SCALAR_OPERATOR(Name, Op)							\
  template <class T1, class T2>	inline							\
  static bool Name (const T1& lhs, const T2& rhs)					\
  {											\
    is_a(optraits<T1>, oln::optraits_scalar)::ensure();					\
    is_a(optraits<T2>, oln::optraits_scalar)::ensure();					\
											\
    typedef typename									\
      internal::deduce_from_traits<internal::operator_cmp_traits,			\
      T1, T2>::ret tmp_type;								\
											\
    return Name##_impl<tmp_type>(static_cast<tmp_type>(lhs),				\
				 static_cast<tmp_type>(rhs));				\
  }											\
											\
  template <class T> inline 								\
  static bool Name##_impl(const rec_scalar<T>& lhs, const rec_scalar<T>& rhs)		\
  { return lhs.self().value() Op rhs.self().value(); }					\
											\
  template <class T> inline 								\
  static bool										\
  Name##_impl(const type::any_const_class<T> lhs, const type::any_const_class<T> rhs)	\
  { return lhs.self() Op rhs.self(); }



//
//  Macros for optraits_int
//
////////////////////////////


# define ASSIGN_INT_OPERATOR(Name, Op)						\
  template <class T1, class T2>	inline						\
  static T1& Name(T1& lhs, const T2& rhs)					\
  {										\
    is_a(optraits<T1>, oln::optraits_int)::ensure();				\
    is_a(optraits<T2>, oln::optraits_int)::ensure();				\
										\
    return Name##_impl<T1,T2>(lhs, rhs);					\
  }										\
										\
  template <class T1, class T2>	inline						\
  static T1& Name##_impl(rec_int<T1>& lhs, const rec_int<T2>& rhs)		\
  {										\
    lhs.self() = lhs.self().value() Op rhs.self().value();			\
    return lhs.self();								\
  }										\
										\
  template <class T1, class T2>	inline						\
  static T1& Name##_impl(rec_int<T1>& lhs, const type::any_const_class<T2>& rhs) \
  {										\
    lhs.self() = lhs.self().value() Op rhs.self();				\
    return lhs.self();								\
  }										\
										\
  template <class T1, class T2>	inline						\
  static T1& Name##_impl(type::any_class<T1>& lhs, const rec_int<T2>& rhs)	\
  {										\
    lhs.self() = lhs.self() Op rhs.self().value();				\
    return lhs.self();								\
  }    

# define ARITH_INT_OPERATOR(Name, Op)						\
  template <class T1, class T2>	inline						\
  static typename								\
  internal::deduce_from_traits<internal::operator_##Name##_traits, T1, T2>::ret	\
  Name(const T1& lhs, const T2& rhs)						\
  {										\
    is_a(optraits<T1>, oln::optraits_int)::ensure();				\
    is_a(optraits<T2>, oln::optraits_int)::ensure();				\
										\
    typedef typename								\
      internal::deduce_from_traits<internal::operator_##Name##_traits,		\
      T1, T2>::ret return_type;							\
    return_type result(lhs);							\
    result Op rhs;								\
    return result;								\
  }



#endif // ndef OLENA_VALUE_OPTRAITS_SCALAR_DEFS_HH
