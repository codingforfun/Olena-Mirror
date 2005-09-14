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

#ifndef OLENA_OPS_ARITH_HH
# define OLENA_OPS_ARITH_HH

# include <mlc/bool.hh>
# include <mlc/is_a.hh>
# include <mlc/typedef.hh>

# include <oln/core/abstract/image.hh>
# include <oln/core/pw/image.hh>
# include <oln/core/pw/value.hh>
# include <oln/core/pw/arith.hh>

# include <oln/ops/overload.hh>


# define oln_are_2_images(I1, I2)		\
mlc::and_< mlc_is_a(I1, abstract::image),	\
           mlc_is_a(I2, abstract::image) >


# define oln_decl_binary_operator_overload(OperatorName, OperatorSymbol)	\
										\
  template <typename L, typename R>						\
  struct decl_overload< mlc::tag::OperatorName##_, 6, L, R >			\
    : mlc::where< oln_are_2_images(L, R) >					\
  {										\
    typedef pw::image< pw::binary_op< mlc::f_##OperatorName##_type,		\
                                      pw::value<L>,				\
                                      pw::value<R> > > ret;			\
  };										\
										\
  template <typename L, typename R>						\
  mlc_binary_ret(OperatorName, L, R)						\
  operator OperatorSymbol (const abstract::image<L>& lhs,			\
			   const abstract::image<R>& rhs)			\
  {										\
    return pw_image(pw_value(lhs) OperatorSymbol pw_value(rhs));		\
  }										\
										\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



# define oln_decl_unary_operator_overload(OperatorName, OperatorSymbol)	\
									\
  template <typename R>							\
  struct decl_overload< mlc::tag::OperatorName##_, 6, R >		\
    : mlc::where< mlc_is_a(R, abstract::image) >			\
  {									\
    typedef pw::image< pw::unary_op< mlc::f_##OperatorName##_type,	\
                                     pw::value<R> > > ret;		\
  };									\
									\
  template <typename R>							\
  mlc_unary_ret(OperatorName, R)					\
  operator OperatorSymbol (const abstract::image<R>& rhs)		\
  {									\
    return pw_image(OperatorSymbol pw_value(rhs));			\
  }									\
									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



namespace oln
{

  // Binary arithmetical operators between images.

  oln_decl_binary_operator_overload(  plus, + );  
  oln_decl_binary_operator_overload( minus, - );  
  oln_decl_binary_operator_overload( times, * );  
  oln_decl_binary_operator_overload(   div, / );  
  oln_decl_binary_operator_overload(   mod, % );  

  oln_decl_unary_operator_overload(    inc, ++ );  
  oln_decl_unary_operator_overload(    dec, -- );  
  oln_decl_unary_operator_overload( uminus, - );  


//   /// Operator-like "min" and "max" between 2 images.

//   template <typename L, typename R>
//   image_from_pwf< pw::binary_op< f_min_type,
// 				 pw::value<L>,
// 				 pw::value<R> > >
//   min (const abstract::image<L>& lhs,
//        const abstract::image<R>& rhs)
//   {
//     return image_for_all_p( oln::min(pw_value(lhs), pw_value(rhs)) );
//   }

//   template <typename L, typename R>
//   image_from_pwf< pw::binary_op< f_max_type,
// 				 pw::value<L>,
// 				 pw::value<R> > >
//   max (const abstract::image<L>& lhs,
//        const abstract::image<R>& rhs)
//   {
//     return image_for_all_p( oln::max(pw_value(lhs), pw_value(rhs)) );
//   }

//   /// Operator-like "inf" and "sup" between 2 images.

//   template <typename L, typename R>
//   image_from_pwf< pw::binary_op< f_inf_type,
// 				 pw::value<L>,
// 				 pw::value<R> > >
//   inf (const abstract::image<L>& lhs,
//        const abstract::image<R>& rhs)
//   {
//     return image_for_all_p( oln::inf(pw_value(lhs), pw_value(rhs)) );
//   }

//   template <typename L, typename R>
//   image_from_pwf< pw::binary_op< f_sup_type,
// 				 pw::value<L>,
// 				 pw::value<R> > >
//   sup (const abstract::image<L>& lhs,
//        const abstract::image<R>& rhs)
//   {
//     return image_for_all_p( oln::sup(pw_value(lhs), pw_value(rhs)) );
//   }



} // end of namespace oln




// template <typename I, typename F>
// void operator + (const abstract::image<I>&,
// 		 const pw::abstract::function<F>&)
// {
//   struct OLENA_ERROR__args_are_not_compatible();
// }
// template <typename F, typename I>
// void operator + (const pw::abstract::function<F>&,
// 		 const abstract::image<I>&)
// {
//   struct OLENA_ERROR__args_are_not_compatible();
// }
// FIXME: to be continued...


#endif // ! OLENA_OPS_ARITH_HH
