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

#ifndef OLENA_CORE_PW_BINARY_OP_HH
# define OLENA_CORE_PW_BINARY_OP_HH

# include <mlc/is_a.hh>
# include <mlc/afun.hh>
# include <oln/core/pw/abstract/binary_function.hh>
# include <oln/core/pw/value.hh>
# include <oln/core/pw/image.hh>


namespace oln {

  // FIXME: binary_op should be named binary_fun?

  // fwd decl
  namespace pw {
    template <typename F, typename L, typename R> struct binary_op;
  }

  // super type
  template <typename F, typename L, typename R>
  struct set_super_type < pw::binary_op<F,L,R> > { typedef pw::abstract::binary_function< L, R, pw::binary_op<F,L,R> > ret; };

  // props
  template <typename F, typename L, typename R>
  struct set_props < category::pw, pw::binary_op<F,L,R> >
  {
    typedef
    typename mlc::mfun_res_traits< F,
				   oln_pw_type_of(L, value),
				   oln_pw_type_of(R, value) >::ret
    value_type;
  };



  namespace pw {

    // FIXME: binary_op only works with mlc::abstract::meta_binary_function
    // FIXME: think about another type that works with mlc::abstract::binary_function...

    template <typename F, typename L, typename R>
    struct binary_op : public abstract::binary_function< L, R, binary_op<F,L,R> >
    {
      typedef binary_op<F,L,R> self_type;
      typedef abstract::binary_function<L,R,self_type> super_type;

      F fun;

      binary_op(const abstract::function<L>& left,
		const abstract::function<R>& right) :
	super_type(left, right),
	fun()
      {
	mlc_is_a(F, mlc::abstract::meta_binary_function)::ensure();
      }

      binary_op(const F& fun,
		const abstract::function<L>& left,
		const abstract::function<R>& right) :
	super_type(left, right),
	fun(fun)
      {
	mlc_is_a(F, mlc::abstract::meta_binary_function)::ensure();
      }

      typedef oln_pw_type_of(self_type, point) point_type;
      typedef oln_pw_type_of(self_type, value) value_type;

      const value_type impl_get(const point_type& p) const
      {
	const value_type tmp = this->fun(this->left(p), this->right(p));
	return tmp;
      }

    };


  } // end of namespace oln::pw

} // end of namespace oln



namespace mlc
{


  template <typename F, typename A1, typename A2>
  struct decl_overload <tag::mbfun, 1,
			F, A1, A2> : public where< mlc::and_<mlc_is_a(A1, oln::pw::abstract::function),
							     mlc_is_a(A2, oln::pw::abstract::function)> >
  {
    typedef oln_pw_type_of(A1, exact) A1_;
    typedef oln_pw_type_of(A2, exact) A2_;

    typedef oln::pw::binary_op<F,A1_,A2_> ret;

    template <class A1_, class A2_>
    static ret exec(const F& fun,
		    const oln::pw::abstract::function<A1_>& arg1,
		    const oln::pw::abstract::function<A2_>& arg2)
    {
      // safety check:
      mlc_is_a(F, mlc::abstract::meta_binary_function)::ensure();
      ret tmp(arg1, arg2);
      return tmp;
    }
  };


  template <typename F, typename I1, typename I2>
  struct decl_overload <tag::mbfun, 2,
			F, I1, I2> : public where< mlc::and_<mlc_is_a(I1, oln::abstract::image),
							     mlc_is_a(I2, oln::abstract::image)> >
  {
    // FIXME: oln_type_of(I, exact) does not work yet...
    typedef I1 /*oln_type_of(I1, exact)*/ I1_;
    typedef I2 /*oln_type_of(I2, exact)*/ I2_;

    typedef oln::pw::binary_op<F,
                               oln::pw::value<I1_>,
                               oln::pw::value<I2_> > PWF;
    typedef oln::pw::image<PWF> ret;

    template <class I1_, class I2_>
    static ret exec(const F& fun,
		    const oln::abstract::image<I1_>& arg1,
		    const oln::abstract::image<I2_>& arg2)
    {
      // safety check:
      mlc_is_a(F, mlc::abstract::meta_binary_function)::ensure();
      
      oln::pw::value<I1_> left_pw_val(arg1);
      oln::pw::value<I2_> right_pw_val(arg2);
      PWF pw_fun(fun, left_pw_val, right_pw_val);
      ret tmp(pw_fun);
      return tmp;
    }
  };

} // end of namespace mlc


#endif // ! OLENA_CORE_PW_BINARY_OP_HH
