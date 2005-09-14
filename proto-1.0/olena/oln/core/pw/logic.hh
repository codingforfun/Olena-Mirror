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

#ifndef OLENA_CORE_PW_LOGIC_HH
# define OLENA_CORE_PW_LOGIC_HH


# include <mlc/cmp.hh>
# include <mlc/ftraits.hh>
# include <mlc/flogic.hh>

# include <oln/core/pw/image.hh>
# include <oln/core/pw/unary_op.hh>
# include <oln/core/pw/binary_op.hh>
# include <oln/core/pw/literal.hh>

# include <oln/ops/overload.hh>


// overload of UNARY op ids:
// 1  <=>  pwf      OP pwf
// 2  <=>  image OP image  (in oln/core/ops/logic.hh)
// ...



// overload of BINARY ops ids:
// 1  <=>  pwf      OP pwf
// 2  <=>  pwf      OP bool
// 3  <=>  bool     OP pwf
// 4  <=>  image OP image  (in oln/core/ops/logic.hh)
// ...






/// Macro oln_pw_decl_binary_logic_op.

# define oln_pw_decl_binary_logic_op(OperatorName, OperatorSymbol)	\
									\
  /*  1  <=>  pwf OP pwf  */						\
									\
  template <typename L, typename R>					\
  struct decl_overload< mlc::tag::OperatorName##_, 1, L, R >		\
    : mlc::where< mlc::and_< mlc_is_a(L, pw::abstract::function),	\
			     mlc_is_a(R, pw::abstract::function) > >	\
  {									\
    typedef pw::binary_op< mlc::f_##OperatorName##_type,		\
                           L,						\
                           R > ret;					\
  };									\
									\
  template <typename L, typename R>					\
  const mlc_binary_ret(OperatorName, L, R)				\
  operator OperatorSymbol (const pw::abstract::function<L>& lhs,	\
                           const pw::abstract::function<R>& rhs)	\
  {									\
    mlc::is_Boolean<oln_pw_type_of(L, value)>::ensure();		\
    mlc::is_Boolean<oln_pw_type_of(R, value)>::ensure();		\
    precondition(lhs.size() == rhs.size());				\
    pw::binary_op< mlc::f_##OperatorName##_type,			\
                   L, R > tmp(lhs, rhs);				\
    return tmp;								\
  }									\
									\
  /*  2  <=>  pwf OP bool  */						\
									\
  template <typename L, typename R>					\
  struct decl_overload< mlc::tag::OperatorName##_, 2, L, R >		\
    : mlc::where< mlc::and_< mlc_is_a(L, pw::abstract::function),	\
			     mlc::is_Boolean<R> > >			\
  {									\
    typedef pw::binary_op< mlc::f_##OperatorName##_type,		\
                           L,						\
                           pw::literal<R> > ret;			\
  };									\
									\
  template <typename L>							\
  const mlc_binary_ret(OperatorName, L, bool)				\
  operator OperatorSymbol (const pw::abstract::function<L>& lhs,	\
                           bool rhs)					\
  {									\
    mlc::is_Boolean<oln_pw_type_of(L, value)>::ensure();		\
    pw::literal<bool> rhs_(rhs);					\
    mlc_binary_ret(OperatorName, L, bool) tmp(lhs, rhs_);		\
    return tmp;								\
  }									\
									\
  /*  3  <=>  bool OP pwf  */						\
									\
  template <typename L, typename R>					\
  struct decl_overload< mlc::tag::OperatorName##_, 3, L, R >		\
    : mlc::where< mlc::and_< mlc::is_Boolean<L>,			\
			     mlc_is_a(R, pw::abstract::function) > >	\
  {									\
    typedef pw::binary_op< mlc::f_##OperatorName##_type,		\
                           pw::literal<L>,				\
                           R > ret;					\
  };									\
									\
  template <typename R>							\
  const mlc_binary_ret(OperatorName, bool, R)				\
  operator OperatorSymbol (bool lhs,					\
                           const pw::abstract::function<R>& rhs)	\
  {									\
    mlc::is_Boolean<oln_pw_type_of(R, value)>::ensure();		\
    pw::literal<bool> lhs_(lhs);					\
    mlc_binary_ret(OperatorName, bool, R) tmp(lhs_, rhs);		\
    return tmp;								\
  }									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n








/// Macro oln_pw_decl_unary_logic_op.

# define oln_pw_decl_unary_logic_op(OperatorName, OperatorSymbol)	\
									\
  template <typename R>							\
  struct decl_overload< mlc::tag::OperatorName##_, 1, R >		\
    : mlc::where< mlc_is_a(R, pw::abstract::function) >			\
  {									\
    typedef pw::unary_op< mlc::f_##OperatorName##_type,			\
                          R > ret;					\
  };									\
									\
  template <typename R>							\
  mlc_unary_ret(OperatorName, R)					\
  operator OperatorSymbol (const pw::abstract::function<R>& rhs)	\
  {									\
    mlc::is_Boolean<oln_pw_type_of(R, value)>::ensure();		\
    oln::pw::unary_op< mlc::f_##OperatorName##_type,			\
                       R > tmp(rhs);					\
    return tmp;								\
  }									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




namespace oln {

  oln_pw_decl_unary_logic_op(  not, not );

  oln_pw_decl_binary_logic_op( and, and );
  oln_pw_decl_binary_logic_op( or,  or  );
  oln_pw_decl_binary_logic_op( xor, xor );

} // end of namespace oln


#endif // ! OLENA_CORE_PW_LOGIC_HH
