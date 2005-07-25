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

# include <oln/utils/overload.hh>
# include <oln/core/pw/binary_op.hh>
# include <oln/core/pw/literal.hh>



# define oln_pw_decl_binary(OperatorName)			\
								\
  template <typename F1, typename F2>				\
  struct set_overload_2 < tag::op_##OperatorName,		\
			  where<F1, pw::abstract::function>,	\
			  where<F2, pw::abstract::function> >	\
  {								\
    typedef pw::binary_op< f_##OperatorName##_type,		\
			   F1, F2> ret;				\
    static ret exec(const F1& lhs, const F2& rhs)		\
    {								\
      ret tmp(lhs, rhs);					\
      return tmp;						\
    }								\
  };								\
								\
  template <typename F, typename T>				\
  struct set_overload_2 < tag::op_##OperatorName,		\
			  where<F, pw::abstract::function>,	\
			  where<T, whatever> >			\
  {								\
    typedef pw::literal<T> rhs_type;				\
    typedef pw::binary_op< oln::f_##OperatorName##_type,	\
			   F, rhs_type> ret;			\
    static ret exec(const F& lhs, const T& rhs)			\
    {								\
      rhs_type rhs_(rhs);					\
      ret tmp(lhs, rhs_);					\
      return tmp;						\
    }								\
  };								\
								\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n


#endif // ! OLENA_CORE_PW_MACROS_HH
