// Copyright 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_TYPE_HH
# define OLENA_CORE_TYPE_HH

# include <string>


namespace type
{



  //
  //  top, bottom, and any
  //
  ///////////////////////////////

  class top {
  public:
    static std::string name() { return ""; }
  protected:
    top() {}
  };

  class bottom {
  public:
    static std::string name() { return "bot"; }
  private:
    bottom();
  };

  template<class Inferior = bottom>
  class any : public top {
  public:
    typedef Inferior inferior;
    static std::string name() { return std::string("any<") + Inferior::name() + ">"; }
  protected:
    any() {}
  };



  //
  //  inferior traits
  //
  ///////////////////////////////

  template<class C>
  struct inferior
  {
    typedef typename C::inferior ret;
  };



  //
  //  find exact
  //
  ///////////////////////////////

  namespace internal {

    // fwd decl

    template<class T> struct _find_exact;

    // helper

    template<class T, class I>
    struct _find_exact_helper
    {
      typedef typename _find_exact<I>::ret ret;
    };

    template<class T>
    struct _find_exact_helper<T, bottom>
    {
      typedef T ret;
    };

    // find exact

    template<class T>
    struct _find_exact
    {
      typedef typename inferior<T>::ret I;
      typedef typename _find_exact_helper<T,I>::ret ret;
    };

  } // end of namespace internal



  //
  //  exact traits
  //
  ///////////////////////////////

  template<class T>
  struct exact {
    typedef typename internal::_find_exact<T>::ret ret;
  };

  template<class T>
  struct exact<T*> {
    typedef typename exact<T>::ret* ret;
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
  template<>
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


} // end of namespace type



//
//  to_exact procs
//
///////////////////////////////


template<class T> inline
typename type::exact<T>::ret*
to_exact(type::any<T>* ptr)
{
  return static_cast<typename type::exact<T>::ret*>(ptr);
}

template<class T> inline
const typename type::exact<T>::ret*
to_exact(const type::any<T>* ptr)
{
  return static_cast<const typename type::exact<T>::ret*>(ptr);
}

template<class T> inline
typename type::exact<T>::ret&
to_exact(type::any<T>& ref)
{
  return static_cast<typename type::exact<T>::ret&>(ref);
}

template<class T> inline
const typename type::exact<T>::ret&
to_exact(const type::any<T>& ref)
{
  return static_cast<const typename type::exact<T>::ret&>(ref);
}

# define Exact(Type) \
typename type::exact<Type>::ret

# define Exact_ptr(Type, Var)			\
typedef Exact(_##Type) Type;			\
Type * Var = to_exact(_##Var);

# define Exact_cptr(Type, Var)			\
typedef Exact(_##Type) Type;			\
const Type * Var = to_exact(_##Var);
							\
# define Exact_ref(Type, Var)			\
typedef Exact(_##Type) Type;			\
Type & Var = to_exact(_##Var);

# define Exact_cref(Type, Var)			\
typedef Exact(_##Type) Type;			\
const Type & Var = to_exact(_##Var);


#endif // ! OLENA_CORE_TYPE_HH
