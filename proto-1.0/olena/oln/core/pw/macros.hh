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

#ifndef OLENA_CORE_PW_MACROS_HH
# define OLENA_CORE_PW_MACROS_HH

# include <oln/core/pw/unary_op.hh>
# include <oln/core/pw/binary_op.hh>
# include <oln/core/pw/literal.hh>


# define oln_pw_decl_binary(OperatorName, OperatorSymbol)		\
									\
template <typename L, typename R>					\
const oln::pw::binary_op< oln::f_##OperatorName##_type,			\
                           L, R >					\
operator OperatorSymbol (const oln::pw::abstract::function<L>& lhs,	\
			 const oln::pw::abstract::function<R>& rhs)	\
{									\
  precondition(lhs.size() == rhs.size());				\
  oln::pw::binary_op< oln::f_##OperatorName##_type,			\
                       L, R > tmp(lhs, rhs);				\
  return tmp;								\
}									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n






# define oln_pw_decl_binary_with_lit(OperatorName, OperatorSymbol, LiteralType)	\
										\
template <typename L>								\
const oln::pw::binary_op< oln::f_##OperatorName##_type,				\
			   L, oln::pw::literal<LiteralType> >			\
operator OperatorSymbol (const oln::pw::abstract::function<L>& lhs,		\
			 const LiteralType& rhs)				\
{										\
  oln::pw::literal<LiteralType> rhs_(rhs);					\
  oln::pw::binary_op< oln::f_##OperatorName##_type,				\
                      L, oln::pw::literal<LiteralType> > tmp(lhs, rhs_);	\
  return tmp;									\
}										\
										\
template <typename R>								\
const oln::pw::binary_op< oln::f_##OperatorName##_type,				\
			   oln::pw::literal<LiteralType>, R >			\
operator OperatorSymbol (const LiteralType& lhs,				\
			 const oln::pw::abstract::function<R>& rhs)		\
{										\
  oln::pw::literal<LiteralType> lhs_(lhs);					\
  oln::pw::binary_op< oln::f_##OperatorName##_type,				\
                      oln::pw::literal<LiteralType>, R > tmp(lhs_, rhs);	\
  return tmp;									\
}										\
										\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




/// Explicit versions of macros.


# define oln_pw_decl_binary_explicit(OperatorName, OperatorSymbol)	\
									\
template <typename L, typename R>					\
const oln::pw::binary_op< oln::f_##OperatorName##_type,			\
                           L, R >					\
OperatorName (const oln::pw::abstract::function<L>& lhs,		\
	      const oln::pw::abstract::function<R>& rhs)		\
{									\
  precondition(lhs.size() == rhs.size());				\
  oln::pw::binary_op< oln::f_##OperatorName##_type,			\
                       L, R > tmp(lhs, rhs);				\
  return tmp;								\
}									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



# define oln_pw_decl_binary_with_lit_explicit(OperatorName, OperatorSymbol, LiteralType)	\
												\
template <typename L>										\
const oln::pw::binary_op< oln::f_##OperatorName##_type,						\
			   L, oln::pw::literal<LiteralType> >					\
OperatorName (const oln::pw::abstract::function<L>& lhs,					\
	      const LiteralType& rhs)								\
{												\
  oln::pw::literal<LiteralType> rhs_(rhs);							\
  oln::pw::binary_op< oln::f_##OperatorName##_type,						\
                      L, oln::pw::literal<LiteralType> > tmp(lhs, rhs_);			\
  return tmp;											\
}												\
												\
template <typename R>										\
const oln::pw::binary_op< oln::f_##OperatorName##_type,						\
			   oln::pw::literal<LiteralType>, R >					\
OperatorName (const LiteralType& lhs,								\
	      const oln::pw::abstract::function<R>& rhs)					\
{												\
  oln::pw::literal<LiteralType> lhs_(lhs);							\
  oln::pw::binary_op< oln::f_##OperatorName##_type,						\
                      oln::pw::literal<LiteralType>, R > tmp(lhs_, rhs);			\
  return tmp;											\
}												\
												\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



#endif // ! OLENA_CORE_PW_MACROS_HH
