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

#ifndef NTG_OPTRAITS_REAL_DEFS_HH
# define NTG_OPTRAITS_REAL_DEFS_HH

//
// macros used in optraits_real definition
//
/////////////////////////////////////////////

# define ASSIGN_SCALAR_OPERATOR(Name)					\
  template <class T1, class T2> inline					\
  static T1& Name##_equal(T1& lhs, const T2& rhs)			\
  {									\
    ntg_is_a(T1, ntg::real)::ensure();					\
    ntg_is_a(T2, ntg::real)::ensure();					\
									\
    return Name##_equal_impl<T1,T2>(lhs, rhs);				\
  }									\
									\
  template <class T1, class T2> inline					\
  static T1&								\
  Name##_equal_impl(ntg::real_value<T1>& lhs,				\
	            const ntg::real_value<T2>& rhs)			\
  {									\
    typedef typename typetraits<T1>::behavior_type behavior_type;	\
    lhs.exact() = behavior_type::check_##Name(lhs.exact().val(),	\
					      rhs.exact().val());	\
    return lhs.exact();							\
  }									\
									\
  template <class T1, class T2> inline					\
  static T1&								\
  Name##_equal_impl(ntg::real_value<T1>& lhs,				\
	            const ntg::any_const_class<T2> rhs)			\
  {									\
    typedef typename typetraits<T1>::behavior_type behavior_type;	\
    lhs.exact() = behavior_type::check_##Name(lhs.exact().val(),	\
					      rhs.exact());		\
    return lhs.exact();							\
  }									\
									\
  template <class T1, class T2> inline					\
  static T1&								\
  Name##_equal_impl(ntg::any_class<T1> lhs,				\
	            const ntg::real_value<T2>& rhs)			\
  {									\
    typedef typename typetraits<T1>::behavior_type behavior_type;	\
    lhs.exact() = behavior_type::check_##Name(lhs.exact(),		\
					      rhs.exact().val());	\
    return lhs.exact();							\
  }

# define ARITH_SCALAR_OPERATOR(Name)					\
  template <class T1, class T2> inline					\
  static ntg_return_type(Name, T1, T2)					\
  Name(const T1& lhs, const T2& rhs)					\
  {									\
    ntg_is_a(T1, ntg::real)::ensure();					\
    ntg_is_a(T2, ntg::real)::ensure();					\
									\
    return Name##_impl<T1,T2>(lhs, rhs);				\
  }									\
									\
  template <class T1, class T2> inline					\
  static ntg_return_type(Name, T1, T2)					\
  Name##_impl(const ntg::real_value<T1>& lhs,				\
	      const ntg::real_value<T2>& rhs)				\
  {									\
    typedef ntg_return_type(Name, T1, T2) return_type;			\
    typedef typename							\
      typetraits<E>::abstract_behavior_type::template get<return_type>	\
        behavior_type;							\
    return_type tmp;							\
    tmp = behavior_type::check_##Name(lhs.exact().val(),		\
				      rhs.exact().val());		\
    return tmp;								\
  }									\
									\
  template <class T1, class T2> inline					\
  static ntg_return_type(Name, T1, T2)					\
  Name##_impl(const ntg::real_value<T1>& lhs,				\
	      const ntg::any_const_class<T2>& rhs)			\
  {									\
    typedef ntg_return_type(Name, T1, T2) return_type;			\
    typedef typename							\
      typetraits<E>::abstract_behavior_type::template get<return_type>	\
        behavior_type;							\
    return_type tmp;							\
    tmp = behavior_type::check_##Name(lhs.exact().val(),		\
				      rhs.exact());			\
    return tmp;								\
  }									\
									\
  template <class T1, class T2> inline					\
  static ntg_return_type(Name, T1, T2)					\
  Name##_impl(const ntg::any_const_class<T1>& lhs,			\
	      const ntg::real_value<T2>& rhs)				\
  {									\
    typedef ntg_return_type(Name, T1, T2) return_type;			\
    typedef typename							\
      typetraits<E>::abstract_behavior_type::template get<return_type>	\
        behavior_type;							\
    return_type tmp;							\
    tmp = behavior_type::check_##Name(lhs.exact(),			\
				      rhs.exact().val());		\
    return tmp;								\
  }

# define CMP_SCALAR_OPERATOR(Name, Op)				\
  template <class T1, class T2> inline				\
  static bool Name (const T1& lhs, const T2& rhs)		\
  {								\
    ntg_is_a(T1, ntg::real)::ensure();				\
    ntg_is_a(T2, ntg::real)::ensure();				\
								\
    typedef ntg_return_type(cmp, T1, T2) tmp_type;		\
								\
    return Name##_impl<tmp_type>(static_cast<tmp_type>(lhs),	\
				 static_cast<tmp_type>(rhs));	\
  }								\
								\
  template <class T> inline					\
  static bool 							\
  Name##_impl(const ntg::real_value<T>& lhs,			\
              const ntg::real_value<T>& rhs)			\
  { return lhs.exact().val() Op rhs.exact().val(); }		\
								\
  template <class T> inline					\
  static bool							\
  Name##_impl(const ntg::any_const_class<T> lhs,		\
	      const ntg::any_const_class<T> rhs)		\
  { return lhs.exact() Op rhs.exact(); }


//
//  Macros for optraits_int
//
////////////////////////////

# define ASSIGN_INT_OPERATOR(Name, Op)					\
  template <class T1, class T2> inline					\
  static T1& Name(T1& lhs, const T2& rhs)				\
  {									\
    ntg_is_a(T1, ntg::integer)::ensure();				\
    ntg_is_a(T2, ntg::integer)::ensure();				\
									\
    return Name##_impl<T1,T2>(lhs, rhs);				\
  }									\
									\
  template <class T1, class T2> inline					\
  static T1& Name##_impl(int_value<T1>& lhs, const int_value<T2>& rhs)	\
  {									\
    lhs.exact() = lhs.exact().val() Op rhs.exact().val();		\
    return lhs.exact();							\
  }									\
									\
  template <class T1, class T2> inline					\
  static T1& Name##_impl(int_value<T1>& lhs, 				\
			 const ntg::any_const_class<T2>& rhs)		\
  {									\
    lhs.exact() = lhs.exact().val() Op rhs.exact();			\
    return lhs.exact();							\
  }									\
									\
  template <class T1, class T2> inline					\
  static T1& Name##_impl(ntg::any_class<T1>& lhs, 			\
			 const int_value<T2>& rhs)			\
  {									\
    lhs.exact() = lhs.exact() Op rhs.exact().val();			\
    return lhs.exact();							\
  }

# define ARITH_INT_OPERATOR(Name, Op)			\
  template <class T1, class T2> inline			\
  static ntg_return_type(Name, T1, T2)			\
  Name(const T1& lhs, const T2& rhs)			\
  {							\
    ntg_is_a(T1, integer)::ensure();			\
    ntg_is_a(T2, integer)::ensure();			\
							\
    typedef ntg_return_type(Name, T1, T2) return_type;	\
    return_type result(lhs);				\
    result Op rhs;					\
    return result;					\
  }

#endif // ndef NTG_OPTRAITS_REAL_DEFS_HH
