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

#ifndef OLENA_OPS_LOGIC_HH
# define OLENA_OPS_LOGIC_HH

# include <mlc/bool.hh>
# include <mlc/is_a.hh>

# include <oln/core/abstract/image.hh>
# include <oln/core/pw/unary_op.hh>
# include <oln/core/pw/binary_op.hh>
# include <oln/core/pw/image.hh>
# include <oln/core/pw/value.hh>
# include <oln/core/pw/logic.hh>


# define oln_decl_binary_logic_op(OperatorName, OperatorSymbol)		\
									\
  template <typename L, typename R>					\
  struct decl_overload< mlc::tag::OperatorName##_, 6, L, R >		\
    : mlc::where< mlc::and_< mlc_is_a(L, abstract::image),		\
			     mlc_is_a(R, abstract::image) > >		\
  {									\
    typedef pw::image< pw::binary_op< mlc::f_##OperatorName##_type,	\
                                      pw::value<L>,			\
                                      pw::value<R> > > ret;		\
  };									\
									\
  template <typename L, typename R>					\
  const mlc_binary_ret(OperatorName, L, R)				\
  operator OperatorSymbol(const abstract::image<L>& lhs,		\
			  const abstract::image<R>& rhs)		\
  {									\
    mlc::and_< mlc_is_a(L, abstract::binary_image),			\
               mlc_is_a(R, abstract::binary_image) >::ensure();		\
    return pw_image(pw_value(lhs) OperatorSymbol pw_value(rhs));	\
  }									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



namespace oln
{
  oln_decl_binary_logic_op( and, and );
  oln_decl_binary_logic_op(  or,  or );
  oln_decl_binary_logic_op( xor, xor );

  /// Unary operator 'not' on a binary image.

  template <typename I>
  struct decl_overload< mlc::tag::not_, 2, I >
    : mlc::where< mlc_is_a(I, abstract::image) >
  {
    typedef pw::image< pw::unary_op< mlc::f_not_type,
                                     pw::value<I> > > ret;
  };

  template <typename I>
  const mlc_unary_ret(not, I)
  operator not (const abstract::image<I>& rhs)
  {
    mlc_is_a(I, abstract::binary_image)::ensure();
    return pw_image(not pw_value(rhs));
  }


} // end of namespace oln


#endif // ! OLENA_OPS_LOGIC_HH
