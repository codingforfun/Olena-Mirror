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

#ifndef OLENA_VALUE_OPS_HH
# define OLENA_VALUE_OPS_HH

# include <oln/value/rec_value.hh>
# include <oln/value/op_traits.hh>
# include <oln/value/cast.hh>

# include <oln/meta/basics.hh>


// neq operators

template<class Self1, class Self2> inline
bool
operator!=(const oln::rec_value<Self1>& lhs,
	   const oln::rec_value<Self2>& rhs)
{
  return !(lhs.self() == rhs.self());
}



// cmp operators

template<class Self> inline
bool
operator>=(const oln::rec_value<Self>& lhs,
	   const oln::rec_value<Self>& rhs)
{
  return !(lhs.self() < rhs.self());
}

template<class Self> inline
bool
operator>(const oln::rec_value<Self>& lhs,
	  const oln::rec_value<Self>& rhs)
{
  return rhs.self() < lhs.self();
}

template<class Self> inline
bool
operator<=(const oln::rec_value<Self>& lhs,
	   const oln::rec_value<Self>& rhs)
{
  return !(rhs.self() < lhs.self());
}


// arithmetical binary operators


#define   DEC_OPERATOR_FOR_BUILTIN(OP, SYMBOL, BUILTIN)	\
template<class Self1>					\
inline							\
typename oln::internal::deduced_from_traits		\
<							\
  oln::internal::operator_##OP##_traits,		\
  BUILTIN,						\
  Self1							\
>::ret_t						\
SYMBOL(BUILTIN lhs, const oln::rec_value<Self1>& rhs)	\
{							\
  return oln::internal::deduced_from_traits		\
         <						\
           oln::internal::operator_##OP##_traits,	\
           BUILTIN,					\
           Self1					\
         >::operate(lhs, to_self(rhs));			\
}

#define  DEC_OPERATOR(OP, SYMBOL)			\
template<class Self,					\
	 class T>					\
inline							\
typename oln::internal::deduced_from_traits		\
<							\
  oln::internal::operator_##OP##_traits,		\
  Self,							\
  T							\
>::ret_t						\
SYMBOL(const oln::rec_value<Self>& lhs, const T& rhs)	\
{							\
  return oln::internal::deduced_from_traits		\
         <						\
           oln::internal::operator_##OP##_traits,	\
           Self,					\
           T						\
         >::operate(to_self(lhs), to_self(rhs));	\
}							\
DEC_OPERATOR_FOR_BUILTIN(OP,SYMBOL,int);		\
DEC_OPERATOR_FOR_BUILTIN(OP,SYMBOL,double);		\
DEC_OPERATOR_FOR_BUILTIN(OP,SYMBOL,float);

DEC_OPERATOR(plus , operator+);
DEC_OPERATOR(times, operator*);
DEC_OPERATOR(div  , operator/);
DEC_OPERATOR(minus, operator-);

/* We can't user DEC_OPERATOR to define min/max, because to would
   require a definition of `min=' and `max=' (aka min_self and
   max_self).  */

#define  DEC_OPERATOR_SIMPLE(OP, CODE)			\
template<class Self,					\
	 class T>					\
inline							\
typename oln::internal::deduced_from_traits		\
<							\
  oln::internal::operator_##OP##_traits,		\
  Self,							\
  T							\
>::ret_t						\
OP(const oln::rec_value<Self>& lhs, const T& rhs)	\
{							\
  typedef typename oln::internal::deduced_from_traits <	\
    oln::internal::operator_##OP##_traits,		\
    Self,						\
    T							\
    >::ret_t result_type;				\
  return (CODE);					\
}

namespace oln {
DEC_OPERATOR_SIMPLE(min,
		    to_self(lhs) < to_self(rhs) ?
		    result_type(to_self(lhs)) : result_type(to_self(rhs)));
DEC_OPERATOR_SIMPLE(max,
		    to_self(lhs) > to_self(rhs) ?
		    result_type(to_self(lhs)) : result_type(to_self(rhs)));
};

// arithmetical unary operators

template<class Self> inline
const Self&
operator++(oln::rec_value<Self>& val)
{
  val.self() += Self::unit();
  return val.self();
}

template<class Self> inline
Self
operator++(oln::rec_value<Self>& val, int)
{
  Self tmp(val.self());
  val.self() += Self::unit();
  return tmp;
}

template<class Self> inline
const Self&
operator+(const oln::rec_value<Self>& val)
{
  return val.self();
}

template<class Self> inline
const Self&
operator--(oln::rec_value<Self>& val)
{
  val.self() -= Self::unit();
  return val.self();
}

template<class Self> inline
Self
operator--(oln::rec_value<Self>& val, int)
{
  Self tmp(val.self());
  val.self() -= Self::unit();
  return tmp;
}

template<class Self> inline
Self
operator-(const oln::rec_value<Self>& val)
{
  return oln::cast::force<Self>(Self::zero() - val.self());
}


// logical operators

template<class Self> inline
const Self
operator|(const oln::rec_value<Self>& lhs,
	  const oln::rec_value<Self>& rhs)
{
  Self tmp(lhs.self());
  tmp |= rhs.self();
  return tmp;
}

template<class Self> inline
const Self
operator&(const oln::rec_value<Self>& lhs,
	  const oln::rec_value<Self>& rhs)
{
  Self tmp(lhs.self());
  tmp &= rhs.self();
  return tmp;
}

template<class Self> inline
const Self
operator^(const oln::rec_value<Self>& lhs,
	  const oln::rec_value<Self>& rhs)
{
  Self tmp(lhs.self());
  tmp ^= rhs.self();
  return tmp;
}



#endif // ! OLENA_VALUE_OPS_HH
