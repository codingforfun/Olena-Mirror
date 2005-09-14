// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef METALIC_FARITH_HH
# define METALIC_FARITH_HH

# include <mlc/afun.hh>
# include <mlc/ftraits.hh>

# include <algorithm>



/// Macro mlc_decl_fun_unary_arith.

# define mlc_decl_fun_unary_arith(OperatorName, OperatorBody)				\
											\
  namespace fun										\
  {											\
    template <typename T> struct OperatorName##_;					\
  }											\
											\
  template <typename T>									\
  struct set_super_type < fun::OperatorName##_<T> >					\
  {											\
    typedef abstract::unary_function< fun::OperatorName##_<T> > ret;			\
  };											\
											\
  template <typename T>									\
  struct set_props < category::fun, fun::OperatorName##_<T> >				\
  {											\
    typedef typename traits_##OperatorName<T>::ret res_type;				\
    typedef T arg_type;									\
  };											\
											\
  namespace fun										\
  {											\
    template <typename T>								\
    struct OperatorName##_ : public mlc::abstract::unary_function< OperatorName##_<T> >	\
    {											\
      typedef OperatorName##_<T> self_type;						\
      mlc_fun_type_of(self_type, res)							\
      impl_unop(const T& arg) const							\
      {											\
	return OperatorBody;								\
      }											\
    };											\
  }											\
											\
  typedef mfun1<fun::OperatorName##_> f_##OperatorName##_type;				\
  static f_##OperatorName##_type f_##OperatorName;					\
											\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



/// Macro mlc_decl_fun_binary_arith.

# define mlc_decl_fun_binary_arith(OperatorName, OperatorBody)					\
												\
  namespace fun											\
  {												\
    template <typename T1, typename T2> struct OperatorName##_;					\
  }												\
												\
  template <typename T1, typename T2>								\
  struct set_super_type < fun::OperatorName##_<T1,T2> >						\
  {												\
    typedef abstract::binary_function< fun::OperatorName##_<T1,T2> > ret;			\
  };												\
												\
  template <typename T1, typename T2>								\
  struct set_props < category::fun, fun::OperatorName##_<T1,T2> >				\
  {												\
    typedef typename traits_##OperatorName<T1,T2>::ret res_type;				\
    typedef T1 arg1_type;									\
    typedef T2 arg2_type;									\
  };												\
												\
  namespace fun											\
  {												\
    template <typename T1, typename T2 = T1>							\
    struct OperatorName##_ : public mlc::abstract::binary_function< OperatorName##_<T1,T2> >	\
    {												\
      typedef OperatorName##_<T1,T2> self_type;							\
      mlc_fun_type_of(self_type, res)								\
      impl_binop(const T1& lhs, const T2& rhs) const						\
      {												\
	return OperatorBody;									\
      }												\
    };												\
  }												\
												\
  typedef mfun2<fun::OperatorName##_> f_##OperatorName##_type;					\
  static f_##OperatorName##_type f_##OperatorName;						\
												\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



namespace mlc
{
  // FIXME: update... sample uses:
  //
  // mlc::fun::and_<bool> f;
  // f(b1, b2);
  //
  // mlc::fun::and_<bool, Boolean> f;
  // f(b1, b2);
  //
  // mlc::f_and f;
  // f(b1, b2);
  // or directly:  mlc::f_and(b1, b2);

  mlc_decl_fun_unary_arith(  inc,    ++arg );
  mlc_decl_fun_unary_arith(  dec,    --arg );
  mlc_decl_fun_unary_arith(  uminus, - arg );

  mlc_decl_fun_binary_arith( plus,  lhs + rhs );
  mlc_decl_fun_binary_arith( minus, lhs - rhs );
  mlc_decl_fun_binary_arith( times, lhs * rhs );
  mlc_decl_fun_binary_arith( div,   lhs / rhs );
  mlc_decl_fun_binary_arith( mod,   lhs % rhs );

  mlc_decl_fun_binary_arith( min,   (std::min<T1>(lhs, rhs)) );
  mlc_decl_fun_binary_arith( max,   (std::max<T1>(lhs, rhs)) );
  mlc_decl_fun_binary_arith( inf,   (std::min<T1>(lhs, rhs)) ); // FIXME: awful!
  mlc_decl_fun_binary_arith( sup,   (std::max<T1>(lhs, rhs)) ); // FIXME: awful!

} // end of namespace mlc



#endif // ! METALIC_FARITH_HH
