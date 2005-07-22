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

#ifndef METALIC_AFUN_HH
# define METALIC_AFUN_HH

# include <mlc/any.hh>
# include <mlc/contract.hh>
# include <mlc/overload.hh>
# include <mlc/properties.hh>
# include <mlc/to_string.hh>




// FIXME: break cycling dependance due to...
// FIXME: ...the recursive classes oln::pw::unary_op and oln::f_::abstract::munary




// Macro.

# define mlc_fun_type_of(FunType, Alias) \
mlc_type_of(mlc, mlc::category::fun, FunType, Alias)



namespace mlc
{

  mlc_equip_namespace_with_properties();

  mlc_decl_typedef(res_type);
  mlc_decl_typedef(arg_type);
  mlc_decl_typedef(arg1_type);
  mlc_decl_typedef(arg2_type);
  mlc_decl_typedef(arg3_type);


  // *the* category of functions.

  namespace category
  {
    struct fun;
  }


  // fwd decls of function abstractions.

  namespace abstract {

    template <typename F> struct unary_function;
    template <typename F> struct meta_unary_function;

    template <typename F> struct binary_function;
    template <typename F> struct meta_binary_function;

    template <typename F> struct ternary_function;
    template <typename F> struct meta_ternary_function;

  } // end of mlc::abstract



  template <>
  struct set_default_props < category::fun >
  {
    typedef mlc::undefined_type res_type;
    // for unary functions
    typedef mlc::undefined_type arg_type;
    // for binary functions
    typedef mlc::undefined_type arg1_type;
    typedef mlc::undefined_type arg2_type;
    // for ternary functions
    typedef mlc::undefined_type arg3_type;
  };


  template <typename F>
  struct get_props < category::fun, F >
  {
    typedef mlc_fun_type_of(F, res)  res_type;
    typedef mlc_fun_type_of(F, arg)  arg_type;
    typedef mlc_fun_type_of(F, arg1) arg1_type;
    typedef mlc_fun_type_of(F, arg2) arg2_type;
    typedef mlc_fun_type_of(F, arg3) arg3_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( mlc::category::fun, " << mlc_to_string(F) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t res_type  = " << mlc_to_string(res_type)  << std::endl
	   << "\t arg_type  = " << mlc_to_string(arg_type)  << std::endl
	   << "\t arg1_type = " << mlc_to_string(arg1_type) << std::endl
	   << "\t arg2_type = " << mlc_to_string(arg2_type) << std::endl
	   << "\t arg3_type = " << mlc_to_string(arg3_type) << std::endl
	   << "}" << std::endl;
    }

    static void ensure()
    {
      mlc::is_ok< res_type  >::ensure();
      mlc::is_ok< arg_type  >::ensure();
      mlc::is_ok< arg1_type >::ensure();
      mlc::is_ok< arg2_type >::ensure();
      mlc::is_ok< arg3_type >::ensure();
    }
  };




  // traits for result type of meta functions.

  template <typename F,
	    typename A1,
	    typename A2 = mlc::undefined_type,
	    typename A3 = mlc::undefined_type>
  struct mfun_res_traits
  {
    typedef mlc::undefined_type ret;
  };




  // tags used for overloading abstract::*_function::operator().


  namespace tag
  {
    // for meta functions
    struct mufun; // unary
    struct mbfun; // binary
    struct mtfun; // ternary

  } // end of mlc::tag





  /// default impl of mlc::abstract::meta_unary_function<F>::operator()(const A&)

  template <typename F, typename A>
  struct decl_overload <tag::mufun, 0,
			F, A>
  {
    typedef typename mfun_res_traits<F, A>::ret ret;

    static ret exec(const F& fun, const A& arg)
    {
      // safety check:
      mlc_is_a(F, mlc::abstract::meta_unary_function)::ensure();

      ret tmp = fun.impl_unop(arg);
      return tmp;
    }
  };



  /// default impl of mlc::abstract::meta_binary_function<F>::operator()(const A1&, const A2&)

  template <typename F, typename A1, typename A2>
  struct decl_overload <tag::mbfun, 0,
			F, A1, A2>
  {
    typedef typename mfun_res_traits<F, A1, A2>::ret ret;

    static ret exec(const F& fun, const A1& arg1, const A2& arg2)
    {
      // safety check:
      mlc_is_a(F, mlc::abstract::meta_binary_function)::ensure();

      ret tmp = fun.impl_binop(arg1, arg2);
      return tmp;
    }
  };




  // Properties of abstractions.

  template <typename F>
  struct set_props < category::fun, abstract::unary_function<F> >
  {
    typedef mlc::no_type arg1_type;
    typedef mlc::no_type arg2_type;
    typedef mlc::no_type arg3_type;
  };

  template <typename F>
  struct set_props < category::fun, abstract::binary_function<F> >
  {
    typedef mlc::no_type arg_type;
    typedef mlc::no_type arg3_type;
  };

  template <typename F>
  struct set_props < category::fun, abstract::ternary_function<F> >
  {
    typedef mlc::no_type arg_type;
  };



  namespace abstract {


    /// Class mlc::abstract::unary_function<F>.

    template <typename F> // F is the exact type
    struct unary_function : public mlc::any<F>
    {
      typedef mlc_fun_type_of(F, res) res_type;
      typedef mlc_fun_type_of(F, arg) arg_type;

      res_type operator()(const arg_type& arg) const
      {
	return this->exact().impl_unop(arg);
      }

    protected:
      unary_function() {}
      ~unary_function()
      {
	get_props<category::fun, F>::ensure();
	mlc_check_method_impl(F, res_type, unop, const arg_type&, const);
      }
    };


    /// Class mlc::abstract::meta_unary_function<F>.

    template <typename F>
    struct meta_unary_function : public mlc::any<F>
    {
      template <typename A>
      typename overload<tag::mufun, F, A>::ret
      operator()(const A& arg) const
      {
	return overload<tag::mufun, F, A>::exec(this->exact(), arg);
      }
    protected:
      meta_unary_function() {}
    };




    /// Class mlc::abstract::binary_function<F>.

    template <typename F> // F is the exact type
    struct binary_function : public mlc::any<F>
    {
      typedef mlc_fun_type_of(F, res) res_type;
      typedef mlc_fun_type_of(F, arg1) arg1_type;
      typedef mlc_fun_type_of(F, arg2) arg2_type;

      res_type operator()(const arg1_type& arg1, const arg2_type& arg2) const
      {
	return this->exact().impl_binop(arg1, arg2);
      }

    protected:
      binary_function() {}
      ~binary_function()
      {
	get_props<category::fun, F>::ensure();
	mlc_check_method_impl_2(F, res_type, binop, const arg1_type&, const arg2_type&, const);
      }
    };


    /// Class mlc::abstract::meta_binary_function<F>.

    template <typename F>
    struct meta_binary_function : public mlc::any<F>
    {
      template <typename A1, typename A2>
      typename overload<tag::mbfun, F, A1, A2>::ret
      operator()(const A1& arg1, const A2& arg2) const
      {
	return overload<tag::mbfun, F, A1, A2>::exec(this->exact(), arg1, arg2);
      }
    protected:
      meta_binary_function() {}
    };


  } // end of namespace mlc::abstract



  /// Meta unary function that relies on a unary function.

  template <template <typename> class F>
  struct mfun1;

  template <template <typename> class F,
	    typename A>
  struct mfun_res_traits < mfun1<F>, A >
  {
    typedef mlc_fun_type_of(F<A>, res) ret;
  };

  template <template <typename> class F>
  struct mfun1 : public abstract::meta_unary_function< mfun1<F> >
  {
    template <typename A>
    mlc_fun_type_of(F<A>, res)
    impl_unop(const A& arg) const
    {
      mlc_is_a(F<A>, mlc::abstract::unary_function)::ensure();
      static const F<A> fun;
      return fun(arg);
    }
  };



  /// Meta binary function that relies on a binary function.

  template <template <typename, typename> class F>
  struct mfun2;

  template <template <typename, typename> class F,
	    typename A1, typename A2>
  struct mfun_res_traits < mfun2<F>, A1, A2 >
  {
    typedef F<A1, A2> Fun;
    typedef mlc_fun_type_of(Fun, res) ret;
  };

  template <template <typename, typename> class F>
  struct mfun2 : public abstract::meta_binary_function< mfun2<F> >
  {
    template <typename A1, typename A2>
    struct res {
      typedef F<A1, A2> Fun;
      typedef mlc_fun_type_of(Fun, res) ret;
    };

    template <typename A1, typename A2>
    typename res<A1,A2>::ret
    impl_binop(const A1& arg1, const A2& arg2) const
    {
      typedef F<A1, A2> Fun;
      mlc_is_a(Fun, mlc::abstract::binary_function)::ensure();
      static const F<A1,A2> fun;
      return fun(arg1, arg2);
    }
  };


} // end of namespace mlc



#endif // ! METALIC_AFUN_HH
