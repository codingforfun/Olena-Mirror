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

#ifndef OLENA_CORE_FUNOBJ_CONVERSION_HH
# define OLENA_CORE_FUNOBJ_CONVERSION_HH

# include <oln/funobj/abstract/unary.hh>
# include <oln/funobj/abstract/binary.hh>

namespace oln
{

  namespace f_ {
    template <typename F1, typename F2> struct compose_uu_;
  } // end of namespace oln::f_

  template <typename F1, typename F2>
  struct set_super_type < f_::compose_uu_<F1, F2> > { typedef f_::abstract::unary< f_::compose_uu_<F1, F2> > ret; };

  template <typename F1, typename F2>
  struct set_props < category::fun1, f_::compose_uu_<F1, F2> >
  {
    typedef oln_fun1_type_of(F1, res) res_type;
    typedef oln_fun1_type_of(F2, arg) arg_type;
  };


  /*-----------------------------------.
  | Composition of two unary functor.  |
  `-----------------------------------*/

  namespace f_
  {

    /*! \class compose_uu_
    **
    ** This functor performs a composition between two unary functors.
    */
    template <typename F1, typename F2>
    struct compose_uu_ : public oln_super2_of_(f_::compose_uu_<F1, F2>)
    {
      typedef compose_uu_<F1, F2> self_type;
      typedef oln_fun1_type_of(self_type, res) res_type;
      typedef oln_fun1_type_of(self_type, arg) arg_type;

      compose_uu_(const F1& f1, const F2& f2) :
	f1_(f1.exact()), f2_(f2.exact())
	{
	}

      const res_type impl_unop(const arg_type& arg) const
	{
	  return f1_(f2_(arg));
	}

    private:
      const F1 f1_;
      const F2 f2_;
    };

  } // end of namespace oln::f_

  /// Compose two unary functors.
  template<class UF1, class UF2>
  f_::compose_uu_<UF1, UF2>
  compose_uu(const UF1& f1, const UF2& f2)
  {
    return f_::compose_uu_<UF1, UF2>(f1, f2);
  }


  /*-------------------.
  | Identity functor.  |
  `-------------------*/

  namespace f_ {
    template <typename T> struct identity_;
  } // end of namespace oln::f_

  template <typename T>
  struct set_super_type < f_::identity_<T> > { typedef f_::abstract::unary< f_::identity_<T> > ret; };

  template <typename T>
  struct set_props < category::fun1, f_::identity_<T> >
  {
    typedef T res_type;
    typedef T arg_type;
  };

  namespace f_
  {

  /*! \class f_identity
  **
  ** This functor returns its argument.
  */
  template<class T>
  struct identity_ : public oln_super_of_(f_::identity_<T>)
  {
    const T impl_unop(const T& t) const
    {
      return t;
    }
  };

  typedef f_::unary_meta<f_::identity_> f_identity_type;
  static f_identity_type f_identity;

  } // end of namespace oln::f_

} // end of namespace oln

#endif // ! OLENA_CORE_FUNOBJ_CONVERSION_HH

