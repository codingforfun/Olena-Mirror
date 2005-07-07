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

#ifndef OLENA_CORE_PW_CMP_HH
# define OLENA_CORE_PW_CMP_HH

# include <oln/funobj/cmp.hh>
# include <oln/core/pw/macros.hh>


namespace oln {

  namespace pw {

//     oln_pw_decl_binary_explicit(      eq, == );
//     oln_pw_decl_binary_explicit(     neq, != );
//     oln_pw_decl_binary_explicit(    less, <  );
// //     oln_pw_decl_binary_explicit(     leq, <= );
//     oln_pw_decl_binary_explicit( greater, >  );
//     oln_pw_decl_binary_explicit(     geq, >= );

    oln_pw_decl_binary(      eq, == );
    oln_pw_decl_binary(     neq, != );
    oln_pw_decl_binary(    less, <  );
    oln_pw_decl_binary(     leq, <= );
    oln_pw_decl_binary( greater, >  );
    oln_pw_decl_binary(     geq, >= );

  } // end of namespace oln::pw

} // end of namespace oln


# define oln_pw_decl_cmp_lit(LiteralType)		\
							\
oln_pw_decl_binary_with_lit(      eq, ==, LiteralType);	\
oln_pw_decl_binary_with_lit(     neq, !=, LiteralType);	\
oln_pw_decl_binary_with_lit(     leq, <=, LiteralType); \
oln_pw_decl_binary_with_lit(    less, < , LiteralType);	\
oln_pw_decl_binary_with_lit( greater, > , LiteralType);	\
oln_pw_decl_binary_with_lit(     geq, >=, LiteralType);	\
							\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




namespace oln {

  namespace pw {

    oln_pw_decl_cmp_lit(int);
    oln_pw_decl_cmp_lit(float);
    oln_pw_decl_cmp_lit(double);

  } // end of namespace oln::pw

} // end of namespace oln



#endif // ! OLENA_CORE_PW_CMP_HH
