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

#ifndef OLENA_CORE_FUNOBJ_ARITH_HH
# define OLENA_CORE_FUNOBJ_ARITH_HH

# include <oln/funobj/abstract/unary.hh>
# include <oln/funobj/abstract/binary.hh>



# define oln_decl_funobj_binary(OperatorName, OperatorSymbol)			\
										\
  namespace f_ {								\
    template <typename L, typename R> struct OperatorName##_;			\
  }										\
										\
  template <typename L, typename R>						\
  struct set_super_type < f_::OperatorName##_<L,R> >				\
  {										\
    typedef f_::abstract::binary< f_::OperatorName##_<L,R> > ret;		\
  };										\
										\
  template <typename L, typename R>						\
  struct set_props < category::fun2, f_::OperatorName##_<L,R> >			\
  {										\
    typedef ntg_return_type(OperatorName, L, R) res_type;			\
    typedef L left_type;							\
    typedef R right_type;							\
  };										\
										\
  namespace f_									\
  {										\
										\
    template <typename L, typename R>						\
    struct OperatorName##_ : public oln_super2_of_(f_::OperatorName##_<L,R>)	\
    {										\
      typedef OperatorName##_<L,R> self_type;					\
										\
      const oln_fun2_type_of(self_type, res)					\
	impl_binop(const L& left, const R& right) const				\
	{									\
	  return left OperatorSymbol right;					\
	}									\
    };										\
										\
  }										\
										\
  typedef f_::binary_meta<f_::OperatorName##_> f_##OperatorName##_type;		\
  static f_##OperatorName##_type f_##OperatorName;				\
										\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



namespace oln
{

  oln_decl_funobj_binary(plus,  +);
  oln_decl_funobj_binary(minus, -);
  oln_decl_funobj_binary(times, *);
  oln_decl_funobj_binary(div,   /);
  oln_decl_funobj_binary(mod,   %);

  namespace f_ {
    template <typename T> struct uminus_;
    template <typename T> struct min_;
    template <typename T> struct max_;
  } // end of namespace oln::f_

  template <typename T>
  struct set_super_type < f_::uminus_<T> > { typedef f_::abstract::unary< f_::uminus_<T> > ret; };

  template <typename T>
  struct set_super_type < f_::min_<T> > { typedef f_::abstract::binary< f_::min_<T> > ret; };

  template <typename T>
  struct set_super_type < f_::max_<T> > { typedef f_::abstract::binary< f_::max_<T> > ret; };

  template <typename T>
  struct set_props < category::fun1, f_::uminus_<T> >
  {
    typedef T res_type;
    typedef T arg_type;
  };

  template <typename T>
  struct set_props < category::fun2, f_::min_<T> >
  {
    typedef T res_type;
    typedef T left_type;
    typedef T right_type;
  };

  template <typename T>
  struct set_props < category::fun2, f_::max_<T> >
  {
    typedef T res_type;
    typedef T left_type;
    typedef T right_type;
  };

  namespace f_
  {

    template <typename T>
    struct uminus_ : public oln_super_of_(f_::uminus_<T>)
    {
      const T impl_unop(const T& arg) const
	{
	  return -arg;
	}
    };

    template <typename T>
    struct min_ : public oln_super_of_(f_::min_<T>)
    {
      const T impl_binop(const T& left, const T& right) const
	{
	  return left < right ? left : right;
	}
    };

    template <typename T>
    struct max_ : public oln_super_of_(f_::max_<T>)
    {
      const T impl_binop(const T& left, const T& right) const
	{
	  return right < left ? left : right;
	}
    };

  } // end of namespace oln::f_

  typedef f_::unary_meta<f_::uminus_> f_uminus_type;
  static f_uminus_type f_uminus;

  typedef f_::binary1_meta<f_::min_> f_min_type;
  static f_min_type f_min;

  typedef f_::binary1_meta<f_::max_> f_max_type;
  static f_max_type f_max;

} // end of namespace oln


#endif // ! OLENA_CORE_FUNOBJ_ARITH_HH
