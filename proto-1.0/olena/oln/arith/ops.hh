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

#ifndef OLENA_ARITH_OPS_HH
# define OLENA_ARITH_OPS_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image_typeness.hh>
# include <oln/core/pw/all.hh>


/// Operator + between 2 images.

template <typename L, typename R>
oln::image_from_pw< oln::pw::plus< oln::pw::image<L>,
				   oln::pw::image<R> > >
operator + (const oln::abstract::image<L>& lhs,
	    const oln::abstract::image<R>& rhs)
{
  return oln::for_all_p(oln::p_value(lhs) + oln::p_value(rhs));
}


/// Operator - between 2 images.

template <typename L, typename R>
oln::image_from_pw< oln::pw::minus< oln::pw::image<L>,
				    oln::pw::image<R> > >
operator - (const oln::abstract::image<L>& lhs,
	    const oln::abstract::image<R>& rhs)
{
  return oln::for_all_p(oln::p_value(lhs) - oln::p_value(rhs));
}


/// Operator - (unary) on image.

template <typename R>
oln::image_from_pw< oln::pw::minus< oln::pw::literal< oln_pw_type_of(R, value) >,
				    oln::pw::image<R> > >
operator - (const oln::abstract::image<R>& rhs)
{
  return oln::for_all_p( - oln::p_value(rhs));
}


/// Operator * between 2 images.

template <typename L, typename R>
oln::image_from_pw< oln::pw::times< oln::pw::image<L>,
				    oln::pw::image<R> > >
operator * (const oln::abstract::image<L>& lhs,
	    const oln::abstract::image<R>& rhs)
{
  return oln::for_all_p(oln::p_value(lhs) * oln::p_value(rhs));
}


/// Operator / between 2 images.

template <typename L, typename R>
oln::image_from_pw< oln::pw::div< oln::pw::image<L>,
				  oln::pw::image<R> > >
operator / (const oln::abstract::image<L>& lhs,
	    const oln::abstract::image<R>& rhs)
{
  return oln::for_all_p(oln::p_value(lhs) / oln::p_value(rhs));
}



# define oln_decl_binary_operator(NAME, SYMBOL, TYPE)			\
template <typename L>							\
oln::image_from_pw< oln::pw::NAME< oln::pw::image<L>,			\
				   oln::pw::literal<TYPE> > >		\
operator SYMBOL (const oln::abstract::image<L>& lhs,			\
		 TYPE value)						\
{									\
  return oln::for_all_p(oln::p_value(lhs) SYMBOL oln::p_lit(value));	\
}									\
template <typename R>							\
oln::image_from_pw< oln::pw::NAME< oln::pw::literal<TYPE>,		\
				   oln::pw::image<R> > >		\
operator SYMBOL (TYPE value,						\
		 const oln::abstract::image<R>& rhs)			\
{									\
  return oln::for_all_p(oln::p_lit(value) SYMBOL oln::p_value(rhs));	\
}



oln_decl_binary_operator(plus, +, int)
oln_decl_binary_operator(plus, +, float)
oln_decl_binary_operator(plus, +, double)

oln_decl_binary_operator(minus, -, int)
oln_decl_binary_operator(minus, -, float)
oln_decl_binary_operator(minus, -, double)

oln_decl_binary_operator(times, *, int)
oln_decl_binary_operator(times, *, float)
oln_decl_binary_operator(times, *, double)

oln_decl_binary_operator(div, /, int)
oln_decl_binary_operator(div, /, float)
oln_decl_binary_operator(div, /, double)



template <typename I, typename F>
void operator + (const oln::abstract::image<I>&,
		 const oln::pw::abstract::function<F>&)
{
  struct OLENA_ERROR__args_are_not_compatible();
}
template <typename F, typename I>
void operator + (const oln::pw::abstract::function<F>&,
		 const oln::abstract::image<I>&)
{
  struct OLENA_ERROR__args_are_not_compatible();
}
// FIXME: to be continued...


#endif // ! OLENA_ARITH_OPS_HH
