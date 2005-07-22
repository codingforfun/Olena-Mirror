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

#ifndef METALIC_FCMP_HH
# define METALIC_FCMP_HH

# include <mlc/afun.hh>



# define mlc_decl_mfun_cmp(OperatorName, OperatorSymbol)				\
											\
  template <typename T1, typename T2> struct OperatorName##_;				\
											\
  template <typename T1, typename T2>							\
  struct set_super_type < OperatorName##_<T1,T2> >					\
  {											\
    typedef abstract::binary_function< OperatorName##_<T1,T2> > ret;			\
  };											\
											\
  template <typename T1, typename T2>							\
  struct set_props < category::fun, OperatorName##_<T1,T2> >				\
  {											\
    typedef bool res_type;								\
    typedef T1 arg1_type;								\
    typedef T2 arg2_type;								\
  };											\
											\
  template <typename T1, typename T2>							\
  struct OperatorName##_ : public abstract::binary_function< OperatorName##_<T1,T2> >	\
  {											\
    bool impl_binop(const T1& arg1, const T2& arg2) const				\
    {											\
      return arg1 OperatorSymbol arg2;							\
    }											\
  };											\
  											\
  typedef mfun2<OperatorName##_> f_OperatorName##_type;					\
  static f_OperatorName##_type f_OperatorName##;					\
											\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



namespace mlc
{

  mlc_decl_mfun_cmp( eq,      == );
  mlc_decl_mfun_cmp( neq,     != );

  mlc_decl_mfun_cmp( less,    <  );
  mlc_decl_mfun_cmp( leq,     <= );

  mlc_decl_mfun_cmp( greater, >  );
  mlc_decl_mfun_cmp( geq,     >= );

} // end of namespace mlc



#endif // ! METALIC_FCMP_HH
