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

#ifndef NTG_CORE_INTERNAL_GLOBAL_OPS_DEFS_HH
# define NTG_CORE_INTERNAL_GLOBAL_OPS_DEFS_HH

/*
  Global operators macros, used to factorize global operators
  definitions.
*/

/*
  For every operator, we need a specialization for every builtin type
  to avoid overloading problems. So here are the defined operators: 

  operatorX(ntg_type, T2) (1)
  operatorX(builtin1, T2) (2)
  operatorX(builtin2, T2)
  ...
  
  This handles all cases without ambiguity: 
  
  ntg_type X ntg_type ==> (1)
  ntg_type X builtin  ==> (1)
  builtin1 X ntg_type ==> (2)
*/

/*-------------.
| ASSIGNEMENTS |
`-------------*/

/*---------------------------------.
| Global assignements for builtins |
`---------------------------------*/

# define GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, Builtin)	\
template <class T2> inline				\
Builtin&						\
Op(Builtin& lhs, const value<T2>& rhs)			\
{ return optraits<Builtin>::Name(lhs, rhs.self()); }

/*----------------------------------------------------.
| Global assignements for both ntg_types and builtins |
`----------------------------------------------------*/

# define GLOBAL_ASSIGN_OP(Op, Name)			\
template <class T1, class T2> inline			\
T1&							\
Op(value<T1>& lhs, const T2& rhs)			\
{ return optraits<T1>::Name(lhs.self(), rhs); }		\
							\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, signed   long);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, unsigned long);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, signed   int);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, unsigned int);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, signed   short);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, unsigned short);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, signed   char);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, unsigned char);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, float);		\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, double);		\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, bool);

/*---------------------.
| ARITHMETIC OPERATORS |
`---------------------*/

/*-----------------------------------------.
| Global arithmetic operators for builtins |
`-----------------------------------------*/

# define GLOBAL_ARITH_OP_BUILTIN(Op, Name, Builtin)			\
template <class T2> inline						\
ntg_return_type(Name, Builtin, T2)					\
Op(Builtin lhs, const value<T2>& rhs)					\
{									\
  typedef ntg_deduced_traits_type(Name, Builtin, T2) deduced_type;	\
									\
  typedef typename deduced_type::impl impl;				\
  typedef typename deduced_type::lhs_type lhs_type;			\
  typedef typename deduced_type::rhs_type rhs_type;			\
									\
  return optraits<impl>::Name(static_cast<lhs_type>(lhs),		\
	    	              static_cast<rhs_type>(rhs.self()));	\
}

/*------------------------------------------------------------.
| Global arithmetic operators for both ntg_types and builtins |
`------------------------------------------------------------*/

# define GLOBAL_ARITH_OP(Op, Name)					\
template <class T1, class T2> inline					\
ntg_return_type(Name, T1, T2)						\
Op(const value<T1>& lhs, const T2& rhs)					\
{									\
 /* 									\
   This code is an example of debugging information, disabled until	\
   a good way to handle debug is found.					\
  */									\
									\
 /*									\
 if (ntg_is_debug_active)						\
 {									\
     std::ostringstream s;						\
     s << typename_of<ntg_return_type(Name, T1, T2)>() << " "		\
	       << #Op << "(" << typename_of<T1>() << " lhs, "		\
	    << typename_of<T2>() << " rhs)"  << std::endl		\
     << "\twith lhs = " << lhs.exact() << " and rhs = " << rhs;		\
     ntg_debug_context_set(s.str());					\
 }									\
 */									\
									\
  typedef ntg_deduced_traits_type(Name, T1, T2) deduced_type;		\
									\
  typedef typename deduced_type::impl impl;				\
  typedef typename deduced_type::lhs_type lhs_type;			\
  typedef typename deduced_type::rhs_type rhs_type;			\
									\
  return optraits<impl>::Name(static_cast<lhs_type>(lhs.self()),	\
			      static_cast<rhs_type>(rhs));		\
}									\
									\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, signed   long);			\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, unsigned long);			\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, signed   int);			\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, unsigned int);			\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, signed   short);			\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, unsigned short);			\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, signed   char);			\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, unsigned char);			\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, float);				\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, double);				\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, bool);

/*------------------.
| LOGICAL OPERATORS |
`------------------*/

/*--------------------------------------.
| Global logical operators for builtins |
`--------------------------------------*/

# define GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, Builtin)			\
template <class T2> inline						\
ntg_return_type(logical, Builtin, T2)					\
Op(const Builtin& lhs, const value<T2>& rhs)				\
{									\
  typedef ntg_deduced_traits_type(logical, Builtin, T2) deduced_type;	\
									\
  typedef typename deduced_type::impl impl;				\
  typedef typename deduced_type::lhs_type lhs_type;			\
  typedef typename deduced_type::rhs_type rhs_type;			\
									\
  return optraits<impl>::Name(static_cast<lhs_type>(lhs),		\
			      static_cast<rhs_type>(rhs.self()));	\
}

/*----------------------------------------------------.
| Global logical operators for ntg_types and builtins |
`----------------------------------------------------*/

# define GLOBAL_LOGICAL_OP(Op, Name)					\
template <class T1, class T2> inline					\
ntg_return_type(logical, T1, T2)					\
Op(const value<T1>& lhs, const T2& rhs)					\
{									\
  typedef ntg_deduced_traits_type(logical, T1, T2) deduced_type;	\
									\
  typedef typename deduced_type::impl impl;				\
  typedef typename deduced_type::lhs_type lhs_type;			\
  typedef typename deduced_type::rhs_type rhs_type;			\
									\
  return optraits<impl>::Name(static_cast<lhs_type>(lhs.self()),	\
			      static_cast<rhs_type>(rhs));		\
}									\
									\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, signed   long);			\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, unsigned long);			\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, signed   int);			\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, unsigned int);			\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, signed   short);			\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, unsigned short);			\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, signed   char);			\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, unsigned char);			\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, float);				\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, double);				\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, bool);

/*---------------------.
| COMPARISON OPERATORS |
`---------------------*/

/*-----------------------------------------.
| Global comparison operators for builtins |
`-----------------------------------------*/

# define GLOBAL_CMP_OP_BUILTIN(Op, Name, Builtin)			\
template <class T2> inline						\
bool									\
Op(const Builtin& lhs, const value<T2>& rhs)				\
{									\
  typedef ntg_deduced_traits_type(cmp, Builtin, T2) deduced_type;	\
									\
  typedef typename deduced_type::impl impl;				\
  typedef typename deduced_type::lhs_type lhs_type;			\
  typedef typename deduced_type::rhs_type rhs_type;			\
									\
  return optraits<impl>::Name(static_cast<lhs_type>(lhs),		\
			      static_cast<rhs_type>(rhs.self()));	\
}

/*------------------------------------------------------------.
| Global comparison operators for both ntg_types and builtins |
`------------------------------------------------------------*/

# define GLOBAL_CMP_OP(Op, Name)				\
template <class T1, class T2> inline				\
bool								\
Op(const value<T1>& lhs, const T2& rhs)				\
{								\
  typedef ntg_deduced_traits_type(cmp, T1, T2) deduced_type;	\
								\
  typedef typename deduced_type::impl impl;			\
  typedef typename deduced_type::lhs_type lhs_type;		\
  typedef typename deduced_type::rhs_type rhs_type;		\
								\
 return optraits<impl>::Name(static_cast<lhs_type>(lhs.self()),	\
			      static_cast<rhs_type>(rhs));	\
}								\
								\
GLOBAL_CMP_OP_BUILTIN(Op, Name, signed   long);			\
GLOBAL_CMP_OP_BUILTIN(Op, Name, unsigned long);			\
GLOBAL_CMP_OP_BUILTIN(Op, Name, signed   int);			\
GLOBAL_CMP_OP_BUILTIN(Op, Name, unsigned int);			\
GLOBAL_CMP_OP_BUILTIN(Op, Name, signed   short);		\
GLOBAL_CMP_OP_BUILTIN(Op, Name, unsigned short);		\
GLOBAL_CMP_OP_BUILTIN(Op, Name, signed   char);			\
GLOBAL_CMP_OP_BUILTIN(Op, Name, unsigned char);			\
GLOBAL_CMP_OP_BUILTIN(Op, Name, float);				\
GLOBAL_CMP_OP_BUILTIN(Op, Name, double);			\
GLOBAL_CMP_OP_BUILTIN(Op, Name, bool);

#endif // !NTG_CORE_INTERNAL_GLOBAL_OPS_DEFS_HH
