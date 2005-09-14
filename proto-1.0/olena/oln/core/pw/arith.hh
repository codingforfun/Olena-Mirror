// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_PW_ARITH_HH
# define OLENA_CORE_PW_ARITH_HH

# include <mlc/cmp.hh>
# include <mlc/ftraits.hh>
# include <mlc/farith.hh>

# include <oln/core/abstract/image_typeness.hh>
# include <oln/core/gen/cst.hh>

# include <oln/core/pw/image.hh>
# include <oln/core/pw/unary_op.hh>
# include <oln/core/pw/binary_op.hh>
# include <oln/core/pw/literal.hh>

# include <oln/ops/overload.hh>




// overload ids:
// 1  <=>  pwf      OP pwf
// 2  <=>  pwf      OP builtin
// 3  <=>  builtin  OP pwf
// 4  <=>  pwf      OP constant
// 5  <=>  constant OP pwf 

// 6  <=>  image OP image  (in oln/core/ops/arith.hh)
// ...


/*

// FIXME: these built-in types are not handled in operators:

												\
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, float);		\
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, double);		\
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, char);			\
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, signed int);		\

  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, long double);
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, unsigned char);
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, signed char);
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, unsigned short);
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, signed short);
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, unsigned int);
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, unsigned long);
  oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, signed long);

*/



# define oln_pw_decl_binary_arith_op_with_builtin(OperatorName, OperatorSymbol, BuiltIn)	\
												\
  template <typename L>										\
  const mlc_binary_ret(OperatorName, L, BuiltIn)						\
  operator OperatorSymbol (const pw::abstract::function<L>& lhs,				\
                           BuiltIn rhs)								\
  {												\
    mlc::neq< oln_typeness_of(oln_pw_type_of(L, value)), typeness::binary_tag >::ensure();	\
    pw::literal<BuiltIn> rhs_(rhs);								\
    mlc_binary_ret(OperatorName, L, BuiltIn) tmp(lhs, rhs_);					\
    return tmp;											\
  }												\
												\
  template <typename R>										\
  const mlc_binary_ret(OperatorName, BuiltIn, R)						\
  operator OperatorSymbol (BuiltIn lhs,								\
                           const pw::abstract::function<R>& rhs)				\
  {												\
    mlc::neq< oln_typeness_of(oln_pw_type_of(R, value)), typeness::binary_tag >::ensure();	\
    pw::literal<BuiltIn> lhs_(lhs);								\
    mlc_binary_ret(OperatorName, BuiltIn, R) tmp(lhs_, rhs);					\
    return tmp;											\
  }												\
												\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




/// Macro oln_pw_decl_binary_arith_op.

# define oln_pw_decl_binary_arith_op(OperatorName, OperatorSymbol)				\
												\
  /*  1  <=>  pwf OP pwf  */									\
												\
  template <typename L, typename R>								\
  struct decl_overload< mlc::tag::OperatorName##_, 1, L, R >					\
    : mlc::where< mlc::and_< mlc_is_a(L, pw::abstract::function),				\
			     mlc_is_a(R, pw::abstract::function) > >				\
  {												\
    typedef pw::binary_op< mlc::f_##OperatorName##_type,					\
                           L,									\
                           R > ret;								\
  };												\
												\
  template <typename L, typename R>								\
  const mlc_binary_ret(OperatorName, L, R)							\
  operator OperatorSymbol (const pw::abstract::function<L>& lhs,				\
                           const pw::abstract::function<R>& rhs)				\
  {												\
    mlc::neq< oln_typeness_of(oln_pw_type_of(L, value)), typeness::binary_tag >::ensure();	\
    mlc::neq< oln_typeness_of(oln_pw_type_of(R, value)), typeness::binary_tag >::ensure();	\
    precondition(lhs.size() == rhs.size());							\
    pw::binary_op< mlc::f_##OperatorName##_type,						\
                   L, R > tmp(lhs, rhs);							\
    return tmp;											\
  }												\
												\
  /*  2  <=>  pwf OP builtin  */								\
  /*  3  <=>  builtin OP pwf  */								\
												\
  template <typename L, typename R>								\
  struct decl_overload< mlc::tag::OperatorName##_, 2, L, R >					\
    : mlc::where< mlc::and_< mlc_is_a(L, pw::abstract::function),				\
			     mlc::is_builtin<R> > >						\
  {												\
    typedef pw::binary_op< mlc::f_##OperatorName##_type,					\
                           L,									\
                           pw::literal<R> > ret;						\
  };												\
												\
  template <typename L, typename R>								\
  struct decl_overload< mlc::tag::OperatorName##_, 3, L, R >					\
    : mlc::where< mlc::and_< mlc::is_builtin<L>,						\
			     mlc_is_a(R, pw::abstract::function) > >				\
  {												\
    typedef pw::binary_op< mlc::f_##OperatorName##_type,					\
                           pw::literal<L>,							\
                           R > ret;								\
  };												\
												\
  /*  4  <=>  pwf OP constant  */								\
												\
  template <typename L, typename R>								\
  struct decl_overload< mlc::tag::OperatorName##_, 4, L, R >					\
    : mlc::where< mlc::and_< mlc_is_a(L, pw::abstract::function),				\
			     mlc_is_a(R, constant) > >						\
  {												\
    typedef pw::binary_op< mlc::f_##OperatorName##_type,					\
                           L,									\
                           pw::literal<typename R::value_type> > ret;				\
  };												\
												\
  template <typename L, typename R>								\
  const mlc_binary_ret(OperatorName, L, constant<R>)						\
  operator OperatorSymbol (const pw::abstract::function<L>& lhs,				\
                           const constant<R>& rhs)						\
  {												\
    mlc::neq< oln_typeness_of(oln_pw_type_of(L, value)), typeness::binary_tag >::ensure();	\
    mlc::neq< oln_typeness_of(R), typeness::binary_tag >::ensure();				\
    pw::literal<R> rhs_(rhs);									\
    mlc_binary_ret(OperatorName, L, constant<R>) tmp(lhs, rhs_);				\
    return tmp;											\
  }												\
												\
  /*  5  <=>  constant OP pwf  */								\
												\
  template <typename L, typename R>								\
  struct decl_overload< mlc::tag::OperatorName##_, 5, L, R >					\
    : mlc::where< mlc::and_< mlc_is_a(L, constant),						\
			     mlc_is_a(R, pw::abstract::function) > >				\
  {												\
    typedef pw::binary_op< mlc::f_##OperatorName##_type,					\
                           pw::literal<typename L::value_type>,					\
                           R > ret;								\
  };												\
												\
  template <typename L, typename R>								\
  const mlc_binary_ret(OperatorName, constant<L>, R)						\
  operator OperatorSymbol (const constant<L>& lhs,						\
                           const pw::abstract::function<R>& rhs)				\
  {												\
    mlc::neq< oln_typeness_of(L), typeness::binary_tag >::ensure();				\
    mlc::neq< oln_typeness_of(oln_pw_type_of(R, value)), typeness::binary_tag >::ensure();	\
    pw::literal<L> lhs_(lhs);									\
    mlc_binary_ret(OperatorName, constant<L>, R) tmp(lhs_, rhs);				\
    return tmp;											\
  }												\
												\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n








/// Macro oln_pw_decl_unary_arith_op.

# define oln_pw_decl_unary_arith_op(OperatorName, OperatorSymbol)				\
												\
  template <typename R>										\
  struct decl_overload< mlc::tag::OperatorName##_, 1, R >					\
    : mlc::where< mlc_is_a(R, pw::abstract::function) >						\
  {												\
    typedef pw::unary_op< mlc::f_##OperatorName##_type,						\
                          R > ret;								\
  };												\
												\
  template <typename R>										\
  mlc_unary_ret(OperatorName, R)								\
  operator OperatorSymbol (const pw::abstract::function<R>& rhs)				\
  {												\
    mlc::neq< oln_typeness_of(oln_pw_type_of(R, value)), typeness::binary_tag >::ensure();	\
    oln::pw::unary_op< mlc::f_##OperatorName##_type,						\
                       R > tmp(rhs);								\
    return tmp;											\
  }												\
												\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



namespace oln {

  oln_pw_decl_unary_arith_op(  inc,    ++ );
  oln_pw_decl_unary_arith_op(  dec,    -- );
  oln_pw_decl_unary_arith_op(  uminus, - );

  oln_pw_decl_binary_arith_op( plus,   + );
  oln_pw_decl_binary_arith_op( minus,  - );
  oln_pw_decl_binary_arith_op( times,  * );
  oln_pw_decl_binary_arith_op( div,    / );
  oln_pw_decl_binary_arith_op( mod,    % );

} // end of namespace oln




#endif // ! OLENA_CORE_PW_ARITH_HH
