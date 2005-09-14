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

#ifndef OLENA_CORE_OPS_HH
# define OLENA_CORE_OPS_HH

# include <mlc/cmp.hh>
# include <mlc/is_a.hh>
# include <mlc/ftraits.hh>

# include <oln/core/abstract/any.hh>


namespace oln {

  namespace tag {

    template <typename Tag>
    struct is_
    {};

  } // end of namespace oln::tag

  
  template < typename Tag,
	     unsigned i,
	     typename T1,
	     typename T2 = mlc::no_type,
	     typename T3 = mlc::no_type >
  struct decl_overload : public mlc::undefined_type // FIXME: inheritance is soon obsolete...
  {
  };

} // end of namespace oln



namespace mlc
{
  template <typename Tag,
	    unsigned i,
	    typename T1, typename T2, typename T3>
  struct decl_overload< oln::tag::is_<Tag>,
			i,
			T1, T2, T3 >
  :
    public oln::decl_overload< Tag, i, T1, T2, T3 >
  {
  };

} // end of namespace mlc



# define oln_decl_overload_binary_traits(Name)			\
								\
  template <typename T1, typename T2>				\
  struct decl_overload <tag::Name##_, oln_id, T1, T2>		\
    : public where< or_< oln_is_any(T1), oln_is_any(T2) > >	\
  {								\
    typedef overload<oln::tag::is_<tag::Name##_>, T1, T2> type;	\
    typedef or_< oln_is_any(T1), oln_is_any(T2) > cond; 	\
    typedef mlc_typedef_onlyif_of(type, ret, cond) ret;		\
  };								\
								\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



# define oln_decl_overload_unary_traits(Name)			\
								\
  template <typename T>						\
  struct decl_overload <tag::Name##_, oln_id, T>		\
    : public where< oln_is_any(T) >				\
  {								\
    typedef overload<oln::tag::is_<tag::Name##_>, T> type;	\
    typedef oln_is_any(T) cond;					\
    typedef mlc_typedef_onlyif_of(type, ret, cond) ret;		\
  };								\
								\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



namespace mlc
{

  // Arith.

  oln_decl_overload_binary_traits( plus  );
  oln_decl_overload_binary_traits( minus );
  oln_decl_overload_binary_traits( times );
  oln_decl_overload_binary_traits( div   );
  oln_decl_overload_binary_traits( mod   );

  oln_decl_overload_unary_traits( uminus );
  oln_decl_overload_unary_traits( inc );
  oln_decl_overload_unary_traits( dec );

  // Cmp.

  oln_decl_overload_binary_traits(  eq  );
  oln_decl_overload_binary_traits( neq  );
  oln_decl_overload_binary_traits( less );
  oln_decl_overload_binary_traits( leq  );
  oln_decl_overload_binary_traits( greater );
  oln_decl_overload_binary_traits( geq  );

  // Logic.

  oln_decl_overload_binary_traits(  and );
  oln_decl_overload_binary_traits( nand );
  oln_decl_overload_binary_traits(   or );
  oln_decl_overload_binary_traits(  nor );
  oln_decl_overload_binary_traits(  xor );
  oln_decl_overload_binary_traits( xnor );

  oln_decl_overload_unary_traits(   not );


} // end of namespace mlc


#endif // ! OLENA_CORE_OPS_HH
