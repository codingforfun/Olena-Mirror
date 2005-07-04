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

#ifndef OLENA_CORE_FUNOBJ_ABSTRACT_UNARY_HH
# define OLENA_CORE_FUNOBJ_ABSTRACT_UNARY_HH

# include <mlc/any.hh>
# include <mlc/contract.hh>
# include <mlc/if.hh>
# include <mlc/is_a.hh>
# include <ntg/all.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/gen/internal/value_box.hh>
# include <oln/core/pw/abstract/function.hh>

// FIXME: break cycling dependance due to...
// FIXME: ...the recursive classes oln::pw::unary_op and oln::f_::abstract::munary

// # include <oln/core/pw/unary_op.hh>


// Macros.

# define oln_fun1_type_of(Fun1Type, Alias) \
mlc_type_of(oln, oln::category::fun1, Fun1Type, Alias)


namespace oln
{

  namespace category
  {
    struct fun1;
  }


  template <>
  struct set_default_props < category::fun1 >
  {
    typedef mlc::undefined_type res_type;
    typedef mlc::undefined_type arg_type;
  };


  template <typename F>
  struct get_props < category::fun1, F >
  {
    typedef oln_fun1_type_of(F, res) res_type;
    typedef oln_fun1_type_of(F, arg) arg_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::fun1, " << mlc_to_string(F) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t res_type = " << mlc_to_string(res_type) << std::endl
	   << "\t arg_type = " << mlc_to_string(arg_type) << std::endl
	   << "}" << std::endl;
    }

    static void ensure()
    {
      mlc::is_ok< res_type >::ensure();
      mlc::is_ok< arg_type >::ensure();
    }
  };


  namespace f_
  {

    template <typename F, typename T>
    struct munary_result
    {
      typedef mlc::no_type ret;
    };


    namespace abstract {

      // unary

      template <typename E>
      struct unary : public mlc::any<E>
      {
	typedef oln_fun1_type_of(E, res) res_type;
	typedef oln_fun1_type_of(E, arg) arg_type;
	const res_type operator()(const arg_type& arg) const
	{
	  return this->exact().impl_unop(arg);
	}
      protected:
	unary() {}
	~unary()
	{
	  get_props<category::fun1, E>::ensure();
	  mlc_check_method_impl(E, const res_type, unop, const arg_type&, const);
	}
      };


      // munary

      namespace internal {

	// FIXME: move to internal/?

	struct arg_is_a_value_box;
	struct arg_is_a_point_wise_function;
	struct arg_is_an_image;
	struct arg_is_a_value;

	template <typename E, typename Arg, typename arg_kind>
	struct munary_on;

	// on a value box
	template <typename E, typename I>
	struct munary_on <E, oln::value_box<I>, arg_is_a_value_box>
	{
	  typedef typename munary_result<E, oln_type_of(I, value)>::ret ret;
	  static const ret exec(const E& target, const oln::value_box<I>& arg)
	  {
	    return target.impl_unop(arg.value());
	  }
	};

	// on a point-wise function
// 	template <typename E, typename F>
// 	struct munary_on <E, oln::pw::abstract::function<F>, arg_is_a_point_wise_function>
// 	{
// 	  typedef oln::pw::unary_op<E,F> ret;
// 	  static const ret exec(const E& target, const oln::pw::abstract::function<F>& arg)
// 	  {
// 	    oln::pw::unary_op<E,F> tmp(arg);
// 	    return tmp;
// 	  }
// 	};

	// on a (regular) value
	template <typename E, typename T>
	struct munary_on <E, T, arg_is_a_value>
	{
	  typedef typename munary_result<E, T>::ret ret;
	  static const ret exec(const E& target, const T& arg)
	  {
	    return target.impl_unop(arg);
	  }
	};

	// on an image
	template <typename E, typename I>
	struct munary_on <E, oln::abstract::image<I>, arg_is_an_image>
	{
	  // FIXME: fake code here
	  typedef int ret;
	  static const ret exec(const E& target, const oln::abstract::image<I>& arg)
	  {
	    return 0;
	  }	  
	};

	template <typename E, typename Arg>
	struct munary_helper
	{
	  // arg_kind
	  typedef typename
	  mlc::if_< mlc_is_a(Arg, oln::value_box),
		    arg_is_a_value_box, typename
		    mlc::if_< mlc_is_a(Arg, oln::pw::abstract::function),
			      arg_is_a_point_wise_function, typename
			      mlc::if_< mlc_is_a(Arg, oln::abstract::image),
					arg_is_an_image,
					arg_is_a_value
				      >::ret
	                    >::ret
	           >::ret
	  arg_kind;
	  // ret
	  typedef typename munary_on<E, Arg, arg_kind>::ret ret;
	  // exec
	  static const ret exec(const E& target, const Arg& arg)
	  {
	    return munary_on<E, Arg, arg_kind>::exec(target, arg);
	  }	  
	};

      } // end of namespace oln::f_::abstract::internal


      template <typename E>
      struct munary : public mlc::any<E>
      {
	template <typename T>
	struct helper : public internal::munary_helper<E,T>
	{
	  // using internal::munary_helper<E,T>::ret;
	  // using internal::munary_helper<E,T>::exec;
	};
	template <typename T>
	const typename helper<T>::ret
	operator()(const T& arg) const
	{
	  return helper<T>::exec(this->exact(), arg);
	}
      protected:
	munary() {}
      };


    } // end of namespace oln::f_::abstract

  


    /// Meta unary function that relies on a unary function.


    // fwd decl
    template <template <typename> class F> struct unary_meta;

    // result
    template <template <typename> class F, typename T>
    struct munary_result < unary_meta<F>, T >
    {
      typedef oln_fun1_type_of(F<T>, res) ret;
    };

    // class
    template <template <typename> class F>
    struct unary_meta : public abstract::munary< unary_meta<F> >
    {
      template <typename T>
      const oln_fun1_type_of(F<T>, res)
	impl_unop(const T& arg) const
      {
	static F<T> f;
	return f(arg);
      }

    };


  } // end of namespace oln::f_

} // end of namespace oln



#endif // ! OLENA_CORE_FUNOBJ_ABSTRACT_UNARY_HH
