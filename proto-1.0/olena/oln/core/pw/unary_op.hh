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

#ifndef OLENA_CORE_PW_UNARY_OP_HH
# define OLENA_CORE_PW_UNARY_OP_HH

# include <mlc/is_a.hh>
# include <mlc/afun.hh>
# include <oln/core/pw/abstract/unary_function.hh>
# include <oln/core/pw/value.hh>
# include <oln/core/pw/image.hh>



namespace oln {


  // fwd decl
  namespace pw {
    template <typename F, typename A> struct unary_op;
  }

  // super type
  template <typename F, typename A>
  struct set_super_type < pw::unary_op<F,A> > { typedef pw::abstract::unary_function< A, pw::unary_op<F,A> > ret; };

  // props
  template <typename F, typename A>
  struct set_props < category::pw, pw::unary_op<F,A> >
  {
    typedef
    typename mlc::mfun_res_traits< F,
				   oln_pw_type_of(A, value) >::ret
    value_type;
  };


  namespace pw {

    // FIXME: unary_op only works with mlc::abstract::meta_unary_function
    // FIXME: think about another type that works with mlc::abstract::unary_function...

    template <typename F, typename A>
    struct unary_op : public abstract::unary_function< A, pw::unary_op<F,A> >
    {
      typedef unary_op<F,A> self_type;
      typedef abstract::unary_function<A,self_type> super_type;

      F fun;

      unary_op(const abstract::function<A>& arg) :
	super_type(arg),
	fun()
      {
	mlc_is_a(F, mlc::abstract::meta_unary_function)::ensure();
      }
      unary_op(const F& fun,
	       const abstract::function<A>& arg) :
	super_type(arg),
	fun(fun)
      {
	mlc_is_a(F, mlc::abstract::meta_unary_function)::ensure();
      }

      typedef oln_pw_type_of(self_type, point) point_type;
      typedef oln_pw_type_of(self_type, value) value_type;

      const value_type impl_get(const point_type& p) const
      {
	const value_type tmp = this->fun(this->arg(p));
	return tmp;
      }

    };


  } // end of namespace oln::pw

} // end of namespace oln




namespace mlc
{

  template <typename F, typename A>
  struct decl_overload <tag::mufun, 1,
			F, A> : public where< mlc_is_a(A, oln::pw::abstract::function) >
  {
    typedef oln_pw_type_of(A, exact) A_;

    typedef oln::pw::unary_op<F,A_> ret;

    template <class A_>
    static ret exec(const F& fun,
		    const oln::pw::abstract::function<A_>& arg)
    {
      // safety check:
      mlc_is_a(F, mlc::abstract::meta_unary_function)::ensure();
      ret tmp(fun, arg);
      return tmp;
    }
  };


  template <typename F, typename I>
  struct decl_overload <tag::mufun, 2,
			F, I> : public where< mlc_is_a(I, oln::abstract::image) >
  {
    // FIXME: oln_type_of(I, exact) does not work yet...
    typedef I /*oln_type_of(I, exact)*/ I_;

    typedef oln::pw::unary_op<F, oln::pw::value<I_> > PWF;
    typedef oln::pw::image<PWF> ret;

    template <class I_>
    static ret exec(const F& fun,
		    const oln::abstract::image<I_>& arg)
    {
      // safety check:
      mlc_is_a(F, mlc::abstract::meta_unary_function)::ensure();
      
      oln::pw::value<I_> pw_val(arg);
      PWF pw_fun(fun, pw_val);
      ret tmp(pw_fun);
      return tmp;
    }
  };

} // end of namespace mlc


#endif // ! OLENA_CORE_PW_UNARY_OP_HH
