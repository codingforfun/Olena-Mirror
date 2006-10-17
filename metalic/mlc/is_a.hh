// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef MLC_IS_A_HH
# define MLC_IS_A_HH

# include <mlc/bool.hh>
# include <mlc/bexpr.hh>
# include <mlc/wrap.hh>
# include <mlc/basic.hh>


// private macro so do _not_ use it
# define mlc_internal_is_a_result_		\
sizeof(helper<T,U>::select((T*)helper<T,U>::makeT())) == sizeof(internal::yes_)


namespace mlc
{

  namespace form
  {
    enum {
      class_                          = 1,
      template_l_class_r_class_       = 2,
      template_l_class_class_r_class_ = 3,
      template_l_template_l_class_r_class_r_class_       = 4,
      template_l_template_l_class_class_r_class_r_class_ = 5
      // ...
    };

    template<unsigned id>
    struct desc
    { char dummy[id]; };

    template   < class T>
    static desc< class_ > of();

    template   < template < class > class T>
    static desc< template_l_class_r_class_ > of();

    template<    template < class,class > class T>
    static desc< template_l_class_class_r_class_ > of();

    template<    template < template < class > class > class T>
    static desc< template_l_template_l_class_r_class_r_class_ > of();

    template<    template < template < class,class > class > class T>
    static desc< template_l_template_l_class_class_r_class_r_class_ > of();

    // ...

  } // end of namespace mlc::form


  namespace internal
  {

    typedef char yes_;
    struct no_ { char dummy[2]; };

  } // end of namespace mlc::internal


  // dev note : below, is_a_<T,id> is a better factorization
  //            but g++ 2.95.4 has some trouble with it

  template<unsigned id>
  struct is_a_;

  // class_

  template<>
  struct is_a_< form::class_ >
  {
    typedef is_a_< form::class_ > self;

    template<class T,    class U>
    struct helper
    {
      static internal::yes_ select(U*);
      static internal::no_  select(...);
      static T* makeT();
    };

    template<class T,    class U>
    struct ret
      : public mlc::bexpr_<( mlc_internal_is_a_result_ )>
    {
    };
  };

  // template_l_class_r_class_

  template<>
  struct is_a_< form::template_l_class_r_class_ >
  {
    typedef is_a_< form::template_l_class_r_class_ > self;

    template<class T,    template < class > class U>
    struct helper
    {
      template<class V>
      static internal::yes_ select(U<V>*);
      static internal::no_  select(...);
      static T* makeT();
    };

    template<class T,    template < class > class U>
    struct ret
      : public mlc::bexpr_<( mlc_internal_is_a_result_ )>
    {
    };
  };

  // template_l_class_class_r_class_

  template<>
  struct is_a_< form::template_l_class_class_r_class_ >
  {
    typedef is_a_< form::template_l_class_class_r_class_ > self;

    template<class T,    template < class,class > class U>
    struct helper
    {
      template<class V, class W>
      static internal::yes_ select(U<V,W>*);
      static internal::no_  select(...);
      static T* makeT();
    };

    template<class T,    template < class,class > class U>
    struct ret
      : public mlc::bexpr_<( mlc_internal_is_a_result_ )>
    {};
  };

  // template_l_template_l_class_r_class_r_class_

  template<>
  struct is_a_< form::template_l_template_l_class_r_class_r_class_ >
  {
    typedef is_a_< form::template_l_template_l_class_r_class_r_class_ > self;

    template<class T,    template < template < class > class > class U>
    struct helper
    {
      template<template<class> class V>
      static internal::yes_ select(U<V>*);
      static internal::no_  select(...);
      static T* makeT();
    };

    template<class T,    template < template < class > class > class U>
    struct ret
      : public mlc::bexpr_<( mlc_internal_is_a_result_ )>
    {};
  };

  // template_l_template_l_class_class_r_class_r_class_

  template<>
  struct is_a_< form::template_l_template_l_class_class_r_class_r_class_ >
  {
    typedef is_a_< form::template_l_template_l_class_class_r_class_r_class_ > self;

    template<class T,    template < template < class,class > class > class U>
    struct helper
    {
      template<template<class,class> class V>
      static internal::yes_ select(U<V>*);
      static internal::no_  select(...);
      static T* makeT();
    };

    template<class T,    template < template < class,class > class > class U>
    struct ret
      : public mlc::bexpr_<( mlc_internal_is_a_result_ )>
    {};
  };

} // end of namespace mlc



/*! \def mlc_is_a(T, U)
**
** Macro that answers if T is an U.  T should be a type and U can have
** different forms: class, template class, etc.  The result is a
** Boolean expression type.
**
** Sample use:  With:
**   template <class B> struct base {};
**   struct derived : public base<int> {};
** the expression mlc_is_a(derived, base) derives from mlc::true_.
** A constraint on the parameter of a class, which should be any
** subclass of base<B>, whatever B, can then be easily written:
**   template <class T>
**   struct foo : private mlc::assert_< mlc_is_a(T, base) > {
**     // ...
**   };
**
** Limitations: 1) When U is a template class defined with a default
** parameter, the form of U is ambiguous.  For instance, with the
** definition "template <class P1, class P2 = P1> struct foo {};", foo
** can be considered as well as a "template<class> class" and as a
** "template <class, class> class.  The call mlc_is_a(T, foo) is thus
** ambiguous.  2) The forms of U that are handled by the meta-program
** behind the macro are limited to the few cases listed below:
**      class
**      template < class > class
**      template < class, class > class
**      template < template < class > class > class
**      template < template < class, class > class > class
*/

# define mlc_is_a(T, U)												\
mlc::wrap_<													\
  typename mlc::is_a_< sizeof(mlc::form::of< U >()) >::template ret< typename mlc::basic_<T>::ret, U >	\
>

# define mlc_is_a_(T, U) \
mlc::is_a_< sizeof(mlc::form::of< U >()) >::ret< mlc::basic_<T>::ret, U >


/*! \def mlc_is_not_a(T, U)
**
** Macro equivalent as "not mlc_is_a(T, U)".  The result is a Boolean
** expression type.
**
** \see mlc_is_a(T, U)
*/

# define mlc_is_not_a(T, U)											\
mlc::not_<													\
  typename mlc::is_a_<sizeof(mlc::form::of<U >())>::template ret< typename mlc::basic_<T>::ret, U >	\
>

# define mlc_is_not_a_(T, U) \
mlc::not_< mlc::is_a_< sizeof(mlc::form::of<U >())>::ret< mlc::basic_<T>::ret, U > >


#endif // ! MLC_IS_A_HH
