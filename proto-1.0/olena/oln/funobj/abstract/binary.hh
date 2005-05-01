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

#ifndef OLENA_CORE_FUNOBJ_ABSTRACT_BINARY_HH
# define OLENA_CORE_FUNOBJ_ABSTRACT_BINARY_HH

# include <mlc/any.hh>
# include <mlc/contract.hh>
# include <mlc/types.hh>
# include <ntg/all.hh>
# include <oln/core/abstract/image.hh>


// Macros.

# define oln_fun2_type_of(Fun2Type, Alias) \
mlc_type_of(oln, oln::category::fun2, Fun2Type, Alias)

# define oln_fun2_type_2_of(Fun2Type,_2, Alias) \
mlc_type_2_of(oln, oln::category::fun2, Fun2Type,_2, Alias)




namespace oln
{

  namespace category
  {
    struct fun2;
  }


  template <>
  struct set_default_props < category::fun2 >
  {
    typedef mlc::undefined_type res_type;
    typedef mlc::undefined_type left_type;
    typedef mlc::undefined_type right_type;
  };


  template <typename F>
  struct get_props < category::fun2, F >
  {
    typedef oln_fun2_type_of(F, res)   res_type;
    typedef oln_fun2_type_of(F, left)  left_type;
    typedef oln_fun2_type_of(F, right) right_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::fun1, " << mlc_to_string(F) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t res_type   = " << mlc_to_string(res_type)   << std::endl
	   << "\t left_type  = " << mlc_to_string(left_type)  << std::endl
	   << "\t right_type = " << mlc_to_string(right_type) << std::endl
	   << "}" << std::endl;
    }

    static void ensure()
    {
      mlc::is_ok< res_type   >::ensure();
      mlc::is_ok< left_type  >::ensure();
      mlc::is_ok< right_type >::ensure();
    }
  };



  namespace f_
  {
  

    template <typename F, typename L, typename R>
    struct binary_meta_result
    {
      typedef mlc::no_type ret;
    };

//     template <typename F, typename T>
//     struct binary1_meta_result
//     {
//       typedef mlc::no_type ret;
//     };

    namespace abstract {

      // binary

      template <typename E>
      struct binary : public mlc::any<E>
      {
	typedef oln_fun2_type_of(E, res)   res_type;
	typedef oln_fun2_type_of(E, left)  left_type;
	typedef oln_fun2_type_of(E, right) right_type;

	const res_type operator()(const left_type& left, const right_type& right) const
	{
	  return this->exact().impl_binop(left, right);
	}

      protected:

	binary()
	{}

	~binary()
	{
	  get_props<category::fun2, E>::ensure();
	  mlc_check_method_impl_2(E, const res_type, binop, const left_type&,
				                            const right_type&, const);
	}
      };


      // mbinary

      template <typename E>
      struct mbinary : public mlc::any<E>
      {
	template <typename L, typename R>
	const typename binary_meta_result<E,L,R>::ret
	operator()(const L& left, const R& right) const
	{
	  return this->exact().impl_binop(left, right);
	}

      protected:

	mbinary() {}
      };


      template <typename E>
      struct mbinary1 : public mlc::any<E>
      {
	template <typename T>
	const typename binary_meta_result<E,T,T>::ret
	operator()(const T& left, const T& right) const
	{
	  return this->exact().impl_binop(left, right);
	}

      protected:

	mbinary1() {}
      };


    } // end of namespace oln::abstract




    // fwd decl
    template <template <typename, typename> class F> struct binary_meta;
    template <template <typename> class F> struct binary1_meta;

    // result
    template <template <typename, typename> class F, typename L, typename R>
    struct binary_meta_result < binary_meta<F>, L, R >
    {
      typedef oln_fun2_type_2_of(F<L,R>, res) ret;
    };
    template <template <typename> class F, typename T>
    struct binary_meta_result < binary1_meta<F>, T, T >
    {
      typedef oln_fun2_type_of(F<T>, res) ret;
    };

    // class
    template <template <typename, typename> class F>
    struct binary_meta : public abstract::mbinary< binary_meta<F> >
    {
      template <typename L, typename R>
      const oln_fun2_type_2_of(F<L,R>, res)
	impl_binop(const L& l, const R& r) const
      {
	static F<L,R> f;
	return f(l, r);
      }
    };

    template <template <typename> class F>
    struct binary1_meta : public abstract::mbinary1< binary1_meta<F> >
    {
      template <typename T>
      const oln_fun2_type_of(F<T>, res)
	impl_binop(const T& l, const T& r) const
      {
	static F<T> f;
	return f(l, r);
      }
    };

  } // end of namespace oln::f_

} // end of namespace oln



#endif // ! OLENA_CORE_FUNOBJ_ABSTRACT_BINARY_HH
