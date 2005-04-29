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

# include <oln/funobj/logic.hh>
# include <oln/core/pw/macros.hh>


oln_pw_decl_binary(and, and);
oln_pw_decl_binary(or,  or);
oln_pw_decl_binary(xor, xor);

// FIXME: not?


# define oln_pw_decl_logic_lit(LiteralType)		\
							\
oln_pw_decl_binary_with_lit(and, and, LiteralType);	\
oln_pw_decl_binary_with_lit(or,  or,  LiteralType);	\
oln_pw_decl_binary_with_lit(xor, xor, LiteralType);	\
							\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



namespace ntg {
  struct bin;
}

oln_pw_decl_logic_lit(bool);
oln_pw_decl_logic_lit(ntg::bin);




//       template <typename E>
//       not_<E>
//       function<E>::operator!() const
//       {
// 	mlc::eq< oln_typeness_of(oln_pw_type_of(E, value)), typeness::binary_tag >::ensure();
// 	not_<E> tmp(this->exact());
// 	return tmp;
//       }



#endif // ! OLENA_CORE_PW_LOGIC_HH
