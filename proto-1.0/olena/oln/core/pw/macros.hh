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



// FIXME: rename?
# define oln_pw_operator(NAME, SYMBOL, TYPE)			\
template <typename L>						\
oln::pw::NAME<L, oln::pw::literal<TYPE> >			\
operator SYMBOL (const oln::pw::abstract::function<L>& lhs,	\
		 const TYPE& value)				\
{								\
  return lhs SYMBOL oln::pw::literal<TYPE>(value);		\
}								\
template <typename R>						\
oln::pw::NAME<oln::pw::literal<TYPE>, R>			\
operator SYMBOL (const TYPE& value,				\
		 const oln::pw::abstract::function<R>& rhs)	\
{								\
  return oln::pw::literal<TYPE>(value) SYMBOL rhs;		\
}



// FIXME: rename?
# define oln_pw_cmp_operator(NAME, SYMBOL, TYPE)			\
template <typename L>							\
oln::pw::cmp<L, oln::pw::literal<TYPE>, oln::pw::internal::NAME>	\
operator SYMBOL (const oln::pw::abstract::function<L>& lhs,		\
		 const TYPE& value)					\
{									\
  return lhs SYMBOL oln::pw::literal<TYPE>(value);			\
}									\
template <typename R>							\
oln::pw::cmp<oln::pw::literal<TYPE>, R, oln::pw::internal::NAME>	\
operator SYMBOL (const TYPE& value,					\
		 const oln::pw::abstract::function<R>& rhs)		\
{									\
  return oln::pw::literal<TYPE>(value) SYMBOL rhs;			\
}


// FIXME: rename?
# define oln_pw_cmp_operators(TYPE)		\
oln_pw_cmp_operator(eq, ==, TYPE)		\
oln_pw_cmp_operator(neq, !=, TYPE)		\
oln_pw_cmp_operator(geq, >=, TYPE)		\
oln_pw_cmp_operator(leq, <=, TYPE)		\
oln_pw_cmp_operator(g, >, TYPE)			\
oln_pw_cmp_operator(l, <, TYPE)


#endif // ! OLENA_CORE_PW_MACROS_HH
