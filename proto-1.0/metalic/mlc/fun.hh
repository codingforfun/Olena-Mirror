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

#ifndef METALIC_FUN_HH
# define METALIC_FUN_HH

# include <mlc/any.hh>


// fwd decls
namespace std {
  template <class Arg, class Ret> struct unary_function;
  template <class Arg1, class Arg2, class Ret> struct binary_function;
}



namespace mlc
{

  // Fwd decl.
  template <typename T>
  struct function_traits;


  /*---------------.
  | Abstractions.  |
  `---------------*/

  namespace abstract {

    template <typename E>
    class unary_function : public mlc::any<E>
    {
    public:
      typedef E exact_type;
      typedef typename function_traits<exact_type>::argument_type
        argument_type;
      typedef typename function_traits<exact_type>::result_type
        result_type;

    public:
      result_type
      operator()(const argument_type& arg) const
      {
	// Check for "exact_type::impl_op_paren".
	typedef
	  result_type (exact_type::*fun_type)(const argument_type&) const;
	static fun_type fun_adr = &exact_type::impl_op_paren;
	assert(fun_adr != 0);

	return this->exact().impl_op_paren(arg);
      }
    };

    // FIXME: Add meta_unary_function.

    template <typename E>
    class binary_function : public mlc::any<E>
    {
    public:
      typedef E exact_type;
      typedef typename function_traits<exact_type>::argument1_type
        argument1_type;
      typedef typename function_traits<exact_type>::argument2_type
        argument2_type;
      typedef typename function_traits<exact_type>::result_type
        result_type;

    public:
      result_type
      operator()(const argument1_type& arg1, const argument2_type& arg2) const
      {
	// Check for "exact_type::impl_op_paren".
	typedef
	  result_type (exact_type::*fun_type)(const argument1_type&,
					      const argument2_type&) const;
	static fun_type fun_adr = &exact_type::impl_op_paren;
	assert(fun_adr != 0);

	return this->exact().impl_op_paren(arg1, arg2);
      }
    };
    
    // FIXME: Add meta_binary_function.

  } // end of namespace abstract


  enum function_type
    {
      unknown_function_tag = 0,
      // C-like functions.
      classic_unary_function_tag,
      classic_unary_function_ptr_tag,
      classic_binary_function_tag,
      classic_binary_function_ptr_tag,
      // Functions deriving from the std namespace.
      std_unary_function_tag,
      std_binary_function_tag,
      // Functions deriving from the oln namespace.
      oln_unary_function_tag,
      oln_binary_function_tag,
      oln_meta_unary_function_tag
    };

  struct fun_info_helper
  {
    template <function_type t>
    struct case_ { char dummy[t]; };

    // C-like functions.

    template <typename A, typename R>
    static case_<classic_unary_function_tag> on(R (**)(A));

    template <typename A, typename R>
    static case_<classic_unary_function_ptr_tag> on(R (*)(A));

    template <typename A1, typename A2, typename R>
    static case_<classic_binary_function_tag> on(R (**)(A1, A2));

    template <typename A1, typename A2, typename R>
    static case_<classic_binary_function_ptr_tag> on(R (*)(A1, A2));

    // Functions deriving from the std namespace.

    template <typename A, typename R>
    static case_<std_unary_function_tag>
    on(const std::unary_function<A, R>*);

    template <typename A1, typename A2, typename R>
    static case_<std_binary_function_tag>
    on(const std::binary_function<A1, A2, R>*);

    // Functions deriving from the oln namespace.
    // FIXME: To do.

    static case_<unknown_function_tag> on(...); 
  };


  /*--------------------------------.
  | Helpers for classic functions.  |
  `--------------------------------*/

  template <typename A>
  struct get_result;

  template <typename A, typename R>
  struct get_result <R (*)(A)> { typedef R ret; };

  template <typename A1, typename A2, typename R>
  struct get_result <R (*)(A1,A2)> { typedef R ret; };


  template <typename A>
  struct get_argument;

  template <typename A, typename R>
  struct get_argument <R (*)(A)> { typedef A ret; };


  template <typename A>
  struct get_argument1;

  template <typename A1, typename A2, typename R>
  struct get_argument1 <R (*)(A1, A2)> { typedef A1 ret; };


  template <typename A>
  struct get_argument2;

  template <typename A1, typename A2, typename R>
  struct get_argument2 <R (*)(A1, A2)> { typedef A2 ret; };


  /*-----------------------.
  | Function information.  |
  `-----------------------*/

  // Create a static pointer on type T.
  template <typename T>
  struct make
  {
    static T* ptr();
  };


  // Information on result type and argument type(s).
  template <typename F, typename T, unsigned>
  struct fun_info;

  // R (*)(A).
  template <typename F, typename T>
  struct fun_info <F, T, classic_unary_function_tag>
  {
    typedef typename get_argument<F>::ret argument_type;
    typedef typename get_result<F>::ret result_type;
  };

  // R (**)(A).
  template <typename F, typename T>
  struct fun_info <F, T, classic_unary_function_ptr_tag>
  {
    typedef typename get_argument<F*>::ret argument_type;
    typedef typename get_result<F*>::ret result_type;
  };

  // R (*)(A1, A2).
  template <typename F, typename T>
  struct fun_info <F, T, classic_binary_function_tag>
  {
    typedef typename get_argument1<F>::ret argument1_type;
    typedef typename get_argument2<F>::ret argument2_type;
    typedef typename get_result<F>::ret result_type;
  };

  // R (**)(A1, A2).
  template <typename F, typename T>
  struct fun_info <F, T, classic_binary_function_ptr_tag>
  {
    typedef typename get_argument1<F*>::ret argument1_type;
    typedef typename get_argument2<F*>::ret argument2_type;
    typedef typename get_result<F*>::ret result_type;
  };

  // std::unary_function<A, R>
  template <typename F, typename T>
  struct fun_info <F, T, std_unary_function_tag>
  {
    typedef typename F::argument_type argument_type;
    typedef typename F::result_type result_type;
  };

  // std::binary_function<A1, A2, R>
  template <typename F, typename T>
  struct fun_info <F, T, std_binary_function_tag>
  {
    typedef typename F::first_argument_type argument1_type;
    typedef typename F::second_argument_type argument2_type;
    typedef typename F::result_type result_type;
  };

  // FIXME: To be re-enabled later.
# if 0
  // oln::unary_function<E>
  template <typename F, typename T>
  struct fun_info <F, T, oln_unary_function_tag>
  {
    typedef oln_result_type(F) result_type;
  };

  // oln::binary_function<E>
  template <typename F, typename T>
  struct fun_info <F, T, oln_binary_function_tag>
  {
    typedef oln_result_type(F) result_type;
  };

  // oln::meta_unary_function<E>
  template <typename F, typename T>
  struct fun_info <F, T, oln_meta_unary_function_tag>
  {
    typedef oln_result_type_from(F,T) result_type;
  };
# endif


  /*-------------------------------------.
  | Unary functions, with their traits.  |
  `-------------------------------------*/

  // Fwd decl.
  template <typename F>
  class unary_function;

  template <typename F>
  struct function_traits< unary_function <F> >
  {
    static const int case_id = sizeof(fun_info_helper::on(make<F>::ptr()));
    typedef typename fun_info<F, void, case_id>::argument_type argument_type;
    typedef typename fun_info<F, void, case_id>::result_type result_type;
  };

  template <typename F>
  class unary_function :
    public mlc::abstract::unary_function< unary_function<F> >
  {
  public:
    typedef unary_function<F> self_type;
    typedef abstract::unary_function< unary_function<F> > super_type;

    typedef typename function_traits<self_type>::argument_type argument_type;
    typedef typename function_traits<self_type>::result_type result_type;

  public:
    unary_function(F f) : super_type(), f_(f)
    {
    }

    result_type
    impl_op_paren (const argument_type& arg) const
    {
      return f_(arg);
    }

  private:
    F f_;
  };

  template <typename F>
  unary_function<F>
  make_unary_fun(F f)
  {
    return unary_function<F> (f);
  }


  /*--------------------------------------.
  | Binary functions, with their traits.  |
  `--------------------------------------*/

  // Fwd decl.
  template <typename F>
  class binary_function;

  template <typename F>
  struct function_traits< binary_function <F> >
  {
    static const int case_id = sizeof(fun_info_helper::on(make<F>::ptr()));
    typedef typename fun_info<F, void, case_id>::argument1_type argument1_type;
    typedef typename fun_info<F, void, case_id>::argument2_type argument2_type;
    typedef typename fun_info<F, void, case_id>::result_type result_type;
  };

  template <typename F>
  class binary_function :
    public mlc::abstract::binary_function< binary_function<F> >
  {
  public:
    typedef binary_function<F> self_type;
    typedef abstract::binary_function< binary_function<F> > super_type;

    typedef typename function_traits<self_type>::argument1_type argument1_type;
    typedef typename function_traits<self_type>::argument2_type argument2_type;
    typedef typename function_traits<self_type>::result_type result_type;

  public:
    binary_function(F f) : super_type(), f_(f)
    {
    }

    result_type
    impl_op_paren (const argument1_type& arg1,
		   const argument1_type& arg2) const
    {
      return f_(arg1, arg2);
    }

  private:
    F f_;
  };

  template <typename F>
  binary_function<F>
  make_binary_fun(F f)
  {
    return binary_function<F> (f);
  }


  /*------------------------------------------.
  | Meta-unary functions, with their traits.  |
  `------------------------------------------*/

  // FIXME: To be re-enabled later.
# if 0
  template <typename F>
  struct meta_fun_info
  {
    template <typename T>
    struct with
    {
      static const case_id = sizeof(fun_info_helper::on(make<F>::ptr()));
      typedef typename fun_info_result_type<F, T, case_id>::ret result_type;
    };
  };
#endif


} // end of namespace mlc


#endif // ! METALIC_FUN_HH
