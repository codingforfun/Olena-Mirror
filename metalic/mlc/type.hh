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

// FIXME: make a hierarchy of any with the different implementations

// any and any_with_diamond types are not defined in namespace mlc to
// avoid namespace clashes. Indeed, a type inheriting from another in
// a namespace foo will have implicit access to the foo namespace.

namespace mlc_hierarchy
{

  /*-----------------.
  | any_with_diamond |
  `-----------------*/

  template<class E>
  class any_with_diamond
  {
  public:
    typedef E exact_type;

    E& exact()
    {
      return *(E*)((char*)this - exact_offset);
    }
    const E& exact() const
    {
      return *(const E*)((const char*)this - exact_offset);
    }

    static std::string name()
    {
      return std::string("any_with_diamond<") + E::name() + ">";
    }

  private:
    // This stores the actual value of the offset between the this
    // pointer and the address of the exact type. This enables diamond
    // shaped hierarchies (static_cast can't and reinterpret_cast or
    // (Exact*)(void*) cast are unsafe and compiler dependent).
    static const int exact_offset;

    // Utility members used to determine the exact_offset
    static const E exact_obj;
    static const any_with_diamond<E>& ref_exact_obj;
  };

  template <class E>
  const E any_with_diamond<E>::exact_obj = E();

  template <class E>
  const any_with_diamond<E>&
  any_with_diamond<E>::ref_exact_obj = any_with_diamond<E>::exact_obj;

  template <class E>
  const int any_with_diamond<E>::exact_offset =
  (const char*)(void*)(&any_with_diamond<E>::ref_exact_obj)
    - (const char*)(void*)(&any_with_diamond<E>::exact_obj);


  // Warning, this class does not allow diamond hierarchies, consider
  // any_with_diamond if you really need it.

  /*----.
  | any |
  `----*/

  template<class E>
  class any
  {
  public:
    typedef E exact_type;

    E& exact()             { return static_cast<E&>(*this); }
    const E& exact() const { return static_cast<const E&>(*this); }

    static std::string name()
    {
      return std::string("any<") + E::name() + ">";
    }

  protected:
    any() {}
  };

} // end of namespace mlc_hierarchy

namespace mlc
{

  /*-------------.
  | Useful types |
  `-------------*/

  struct undefined {};

  class final
  {
  public:
    static std::string name() { return "final"; }
  protected:
    final() {}
  };

  class top
  {
  public:
    static std::string name() { return ""; }
  protected:
    top() {}
  };

  /*------------------.
  | assign_exact_this |
  `------------------*/

  // Helper struct for any_with_diamond hierarchies. See
  // mlc_init_static_hierarchy.

  template <class Exact, class Final>
  struct assign_exact_offset
  {
    template <class E>
    static void doit(const E&)
    {}
  };

  template <>
  struct assign_exact_offset<mlc::final, mlc::final>
  {
    template <class E>
    static void doit(const E* t)
    {
      if (!mlc_hierarchy::any_with_diamond<E>::offset_assigned)
	{
	  mlc_hierarchy::any_with_diamond<E>::exact_offset =
	    (const char*)
	    static_cast<const mlc_hierarchy::any_with_diamond<E>*>(t)
	    - (const char*) t;
	  mlc_hierarchy::any_with_diamond<E>::offset_assigned = true;
	}
    }
  };

  /*-------------.
  | exact traits |
  `-------------*/

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

  /*---------.
  | exact_vt |
  `---------*/

  //
  //  exact virtual type traits
  //  (inheritance determination)
  //
  //  If Exact is mlc::final return T else return Exact

  template<class T, class Exact>
  struct exact_vt
  {
    typedef Exact ret;
  };

  template<class Exact>
  struct exact_vt<Exact,final>
  {
    typedef Exact ret;
  };

} // end of namespace mlc

/*-------------.
| exact macros |
`-------------*/

// Return the exact type of a given type.

# define mlc_exact_type(T) typename mlc::exact< T >::ret
# define mlc_exact_type_(T) mlc::exact< T >::ret

// Return the exact virtual type of two given types (inheritance determination).

# define mlc_exact_vt_type(T, Exact)		typename mlc::exact_vt<T, Exact>::ret
# define mlc_2_exact_vt_type(self, T, Exact)	typename mlc::exact_vt<self<T, Exact>, Exact>::ret
# define mlc_exact_vt_type_(T, Exact)		mlc::exact_vt<T, Exact>::ret
# define mlc_2_exact_vt_type_(self, T, Exact)	mlc::exact_vt<self<T, Exact>, Exact>::ret

namespace mlc
{

  /*-------------------.
  | to_exact functions |
  `-------------------*/

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

} // end of namespace mlc

/*---------------.
| mlc_find_exact |
`---------------*/

// FIXME: uncomment when variadic macros become standardized

// #define mlc_find_exact(...)
// typename mlc::exact_vt<__VA_ARGS__>::ret
// #define mlc_find_exact_(...)
// mlc::exact_vt<__VA_ARGS__>::ret

/*--------------------------.
| mlc_init_static_hierarchy |
`--------------------------*/

// # define mlc_init_static_hierarchy(Exact)
// mlc::assign_exact_offset<Exact, mlc::final>::doit(this)

// FIXME: this is for backward compatibility purposes only.  This
// macro is called in every constructor of the classes, so we let them
// in place for the moment until we are sure that the static offset
// method works fine.

# define mlc_init_static_hierarchy(Exact)

/*---------.
| dispatch |
`---------*/

# define mlc_dispatch(Fun)			return exact().Fun##_impl

#endif // ! METALIC_TYPE_HH
