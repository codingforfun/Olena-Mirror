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
# include <ntg/all.hh>
# include <oln/core/abstract/image.hh>


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

      template <typename E>
      struct munary : public mlc::any<E>
      {
	template <typename T>
	const typename munary_result<E,T>::ret
	operator()(const T& arg) const
	{
	  return this->exact().impl_unop(arg);
	}
      protected:
	munary() {}
      };


    } // end of namespace oln::abstract

  


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
