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

# include <string>

//  FIXME: Move static hiearchy tools into a dedicated header ?

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

  class bottom 
  {
  public:
    static std::string name() { return "bot"; }
  private:
    bottom();
  };

  template<class E>
  class any 
  {
  public:
    typedef E exact_type;

    // these reinterpret_casts allow for diamond hierarchies
    // whereas *static_cast<const E*>(this) is forbiden
    // in such cases; if a bug occurs because one of these
    // casts, please submit a bug report!

          E& exact()       { return reinterpret_cast<E&>(*this); }
    const E& exact() const { return reinterpret_cast<const E&>(*this); }

    // self() is only for bwd compatibility purpose
    // please now use exact() instead of self()
    inline       E& self();
    inline const E& self() const;


    static std::string name() { return std::string("any<") + E::name() + ">"; }
  protected:
    any() {}
  };

  //
  //  Helper for static hierarchies: 
  //  FIXME : should not be necessary 
  //
  ////////////////////////////////////

  // if Self is mlc::bottom return T else return Self

  template <class T, class Self>
  struct select_self
  { typedef Self ret; };
  
  template <class T>
  struct select_self<T, bottom>
  { typedef T ret; };

  //
  //  inferior traits
  //  FIXME : erase ?
  //
  ///////////////////////////////

  // template<class C>
  // struct inferior
  // {
  // typedef typename C::inferior ret;
  // };

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

# define Exact(Type) \
typename mlc::exact<Type>::ret
#define Exact_(Type) mlc::exact<Type>::ret

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
///////////////////////////////

template<class T> inline
Exact(T)&
to_exact(T& ref)
{
  return ref.exact();
}

template<class T> inline
const Exact(T)&
to_exact(const T& ref)
{
  return ref.exact();
}

template<class T> inline
Exact(T)*
to_exact(T* ptr)
{
  return &ptr->exact();
}

template<class T> inline
const Exact(T)*
to_exact(const T* ptr)
{
  return &ptr->exact();
}

//
//  code below is only for bwd compatibility purpose
//
//////////////////////////////////////////////////////

template<class E> inline E& mlc::any<E>::self(){ return reinterpret_cast<E&>(*this); }

template<class E> inline const E& mlc::any<E>::self() const { return reinterpret_cast<E&>(*this); }

# define Exact_ptr(Type, Var)			\
typedef Exact(Type##_) Type;			\
Type * Var = to_exact(_##Var);

# define Exact_cptr(Type, Var)			\
typedef Exact(Type##_) Type;			\
const Type * Var = to_exact(_##Var);
							\
# define Exact_ref(Type, Var)			\
typedef Exact(Type##_) Type;			\
Type & Var = to_exact(_##Var);

# define Exact_cref(Type, Var)			\
typedef Exact(Type##_) Type;			\
const Type & Var = to_exact(_##Var);

#endif // ! METALIC_TYPE_HH
