// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef METALIC_TYPE_HH
# define METALIC_TYPE_HH

# include <mlc/config/system.hh>
# include <mlc/bool.hh>

# include <string>
# include <cassert>

//  FIXME: Move static hiearchy tools into a dedicated header ?

namespace mlc_hierarchy 
{

  template<class E>
  class any_with_diamond
  {
  public:
    typedef E exact_type;
    
    E& exact()       { assert(exact_this != 0); return *exact_this; }
    const E& exact() const { assert(exact_this != 0); return *exact_this; }
   
    static std::string name() 
    { 
      return std::string("any_with_diamond<") + E::name() + ">"; 
    }

  protected:
    any_with_diamond() 
    {
      exact_this = 0;
    }

  protected:
    // This stores the actual value of the exact type pointer. This
    // enables diamond hierarchies, as static_cast cannot perform it
    // and reinterpret_cast or (Exact*)(void*) cast are unsafe and
    // compiler dependent.  This member should be initialized by every
    // concrete classes, using the mlc_init_static_hierarchy macro.
    E* exact_this;
  };

  // Warning, this class does not allow diamond hierarchies, consider
  // any_with_diamond if you really need it.

  template<class E>
  class any
  {
  public:
    typedef E exact_type;
    
    E& exact()       { return static_cast<E&>(*this); }
    const E& exact() const { return static_cast<const E&>(*this); }
   
    static std::string name() { return std::string("any<") + E::name() + ">"; }

  protected:
    any() {}
  };

}

namespace mlc
{

  //
  //  Useful types
  //
  /////////////////

  struct undefined {};

  //
  //  final and any
  //
  /////////////////////////

  class final
  {
  public:
    static std::string name() { return "final"; } 
  protected:
    final() {} 
  };

  class top {
  public:
    static std::string name() { return ""; }
  protected:
    top() {}
  };

  /*----------------.
  | Type comparison |
  `----------------*/
  
  template <class T, class U>
  struct type_eq : returns_bool_<false> {};

  template <class T>
  struct type_eq<T, T> : returns_bool_<true> {};

  template <class Exact, class Final>
  struct assign_exact_this
  {
    template <class V>
    static void doit(V*, Exact**)
    {}
  };

  template <>
  struct assign_exact_this<mlc::final, mlc::final>
  {
    template <class U>
    static void doit(U* src, U** dest)
    {
      *dest = src;
    }
  };

  //
  //  Helper for static hierarchies: 
  //  FIXME : should not be necessary 
  //
  ////////////////////////////////////

  // if Self is mlc::final return T else return Self

  template <class T, class Self>
  struct select_self
  { typedef Self ret; };
  
  template <class T>
  struct select_self<T, final>
  { typedef T ret; };
  
  //
  //  exact traits
  //
  ///////////////////////////////

  template<class T>
  struct exact
  {
    typedef typename T::exact_type ret;
  };
  
  template<class T>
  struct exact<const T>
  {
    typedef const typename exact<T>::ret ret;
  };
  
  
  template<class T>
  struct exact<T*>
  {
    typedef typename exact<T>::ret* ret;
  };
  
  template<class T>
  struct exact<T&>
  {
    typedef typename exact<T>::ret& ret;
  };
  
  //
  //  exact virtual type traits
  //  (inheritance determination)
  //
  ////////////////////////////////

  template<class T, class E>
  struct exact_vt
  {
    typedef E ret;
  };
  
  template<class T>
  struct exact_vt<T,final>
  {
    typedef T ret;
  };

  //
  //  name_of and exact_name_of
  //
  ///////////////////////////////

  template<class T>
  std::string name_of()
  {
    return T::name();
  }

  template<class T>
  std::string name_of(const T&)
  {
    return T::name();
  }

  // FIXME: many other builtin-types should be specialized this way.
  template<> inline
  std::string name_of<int>()
  {
    return std::string("int");
  }

  template<class T>
  std::string exact_name_of()
  {
    typedef typename exact<T>::ret e;
    return e::name();
  }

  template<class T>
  std::string exact_name_of(const T&)
  {
    typedef typename exact<T>::ret e;
    return e::name();
  }

} // end of mlc

//
//  Exact macros
//
///////////////////////////////

# define mlc_exact_type(T) typename mlc::exact<T>::ret
# define mlc_exact_type_(T) mlc::exact<T>::ret

//
//  find exact : FIXME: variadic macros !!!
//
////////////////////////////////////////////

// #define FindExact(...) 
// typename mlc::exact_vt<__VA_ARGS__>::ret
// #define FindExact_(...) 
// mlc::exact_vt<__VA_ARGS__>::ret

//
//  to_exact procs
//
////////////////////

template<class T> inline
mlc_exact_type(T)&
to_exact(T& ref)
{
  return ref.exact();
}

template<class T> inline
const mlc_exact_type(T)&
to_exact(const T& ref)
{
  return ref.exact();
}

template<class T> inline
mlc_exact_type(T)*
to_exact(T* ptr)
{
  return &ptr->exact();
}

template<class T> inline
const mlc_exact_type(T)*
to_exact(const T* ptr)
{
  return &ptr->exact();
}

/*------------.
| Misc macros |
`------------*/

# define mlc_init_static_hierarchy(Exact) \
mlc::assign_exact_this<Exact, mlc::final>::doit(this, &(this->exact_this))

/*-------------------------.
| bwd compatibility macros |
`-------------------------*/

# define Exact(Type) \
typename mlc::exact< Type >::ret
#define Exact_(Type) mlc::exact< Type >::ret

# define Exact_ptr(Type, Var)			\
typedef Exact(Type##_) Type;			\
Type * Var = to_exact(_##Var);

# define Exact_cptr(Type, Var)			\
typedef Exact(Type##_) Type;			\
const Type * Var = to_exact(_##Var);

# define Exact_ref(Type, Var)			\
typedef Exact(Type##_) Type;			\
Type & Var = to_exact(_##Var);

# define Exact_cref(Type, Var)			\
typedef Exact(Type##_) Type;			\
const Type & Var = to_exact(_##Var);

#endif // ! METALIC_TYPE_HH
