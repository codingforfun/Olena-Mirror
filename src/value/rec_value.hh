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

// FIXME : Why is there a difference in presentation (comments ...) ?

#ifndef OLENA_VALUE_REC_VALUE_HH
# define OLENA_VALUE_REC_VALUE_HH

# include <iostream>
using std::cout;
using std::endl;

# include "value/predefs.hh"
# include "value/debug.hh"

# include "op_traits.hh"


namespace oln {


  // HERE: we start to provide functions from type to type
  // in oln namespace

  template<unsigned x, class T>
  struct x_larger
  {
    typedef typename x_larger<x-1, typename T::larger_t>::ret ret;
  };

  template<class T>
  struct x_larger<0,T>
  {
    typedef T ret;
  };



  namespace internal {


    // ops traits

    template<class T_lhs, class T_rhs> struct operator_plus_traits;
    template<class T_lhs, class T_rhs> struct operator_minus_traits;
    template<class T_lhs, class T_rhs> struct operator_times_traits;
    template<class T_lhs, class T_rhs> struct operator_div_traits;


    // other traits

    template<class T> struct value_traits;

    template<class T>
    struct real_value_traits
    {
      typedef typename value_traits<T>::ret_t ret_t;
    };

    template<class T> struct larger_traits; // FIXME: ensure that T is rec_value<U>
    template<class T> struct signed_traits; // FIXME: what about T is a built-in type?
    template<class T> struct unsigned_traits; // FIXME: what about T is a built-in type?
    template<class T> struct cumul_traits;

    template<class T>
    struct slarger_traits
    {
      typedef typename signed_traits< typename larger_traits<T>::ret_t >::ret_t ret_t;
    };

#define _OLN_DEF_REAL_VALUE_BUILT_IN(X)    				\
    template<> struct real_value_traits<X> { typedef X ret_t; };
    _OLN_DEF_REAL_VALUE_BUILT_IN(void)
    _OLN_DEF_REAL_VALUE_BUILT_IN(bool)
    _OLN_DEF_REAL_VALUE_BUILT_IN(unsigned char)
    _OLN_DEF_REAL_VALUE_BUILT_IN(char)
    _OLN_DEF_REAL_VALUE_BUILT_IN(unsigned short)
    _OLN_DEF_REAL_VALUE_BUILT_IN(int)
    _OLN_DEF_REAL_VALUE_BUILT_IN(unsigned int)
    _OLN_DEF_REAL_VALUE_BUILT_IN(long)
    _OLN_DEF_REAL_VALUE_BUILT_IN(unsigned long)
    _OLN_DEF_REAL_VALUE_BUILT_IN(float)
    _OLN_DEF_REAL_VALUE_BUILT_IN(double)


    template<class T>
    struct zero_value_traits
    {
      static T zero() { return T::zero(); }
    };

#define _OLN_DEF_ZERO_VALUE_BUILT_IN(X)		\
    template<> struct zero_value_traits<X> { 	\
      static X zero() { return 0; }		\
    };
    _OLN_DEF_ZERO_VALUE_BUILT_IN(bool)
    _OLN_DEF_ZERO_VALUE_BUILT_IN(unsigned char)
    _OLN_DEF_ZERO_VALUE_BUILT_IN(char)
    _OLN_DEF_ZERO_VALUE_BUILT_IN(unsigned short)
    _OLN_DEF_ZERO_VALUE_BUILT_IN(int)
    _OLN_DEF_ZERO_VALUE_BUILT_IN(unsigned int)
    _OLN_DEF_ZERO_VALUE_BUILT_IN(long)
    _OLN_DEF_ZERO_VALUE_BUILT_IN(unsigned long)
    _OLN_DEF_ZERO_VALUE_BUILT_IN(float)
    _OLN_DEF_ZERO_VALUE_BUILT_IN(double)



    // self_traits

    template<class T>
    struct self_traits
    {
      typedef T ret_t;
      static const T to_self(T val) { return val; }
    };

    template<class Self>
    struct self_traits< rec_value<Self> >
    {
      typedef Self ret_t;
      static const Self to_self(const rec_value<Self>& val) { return val.self(); }
    };


  } // end of internal


  // v------- FIXME : strange comment ...

  //
  //  to_self
  //
  //////////////////////////////


  template<class T> inline
  typename internal::self_traits<T>::ret_t
  to_self(const T& val)
  {
    return internal::self_traits<T>::to_self(val);
  }



  //
  //  rec_value<Self>
  //
  //////////////////////////////


  template<class Self>
  class rec_value
  {
  public:

    typedef typename internal::larger_traits<Self>::ret_t   larger_t;
    typedef typename internal::unsigned_traits<Self>::ret_t unsigned_t;
    typedef typename internal::signed_traits<Self>::ret_t   signed_t;
    typedef typename internal::slarger_traits<Self>::ret_t  slarger_t;
    typedef typename internal::cumul_traits<Self>::ret_t    cumul_t;

    typedef Self                                              self_t;
    typedef typename internal::value_traits<Self>::ret_t      value_t;
    typedef typename internal::real_value_traits<Self>::ret_t real_value_t;

    Self& self() {
      return static_cast<Self&>(*this);
    }
    const Self& self() const {
      return static_cast<const Self&>(*this);
    }

    value_t& value(/* int uncomment_to_check_use */) { return _value; }  // hook
    const value_t value() const { return _value; }  // hook

    const real_value_t real_value() const { return self()._real_value(); }


    static const rec_value<Self> unit() { return self().unit(); }
    static const rec_value<Self> zero() { return self().zero(); }
    static const rec_value<Self> min()  { return self().min(); }
    static const rec_value<Self> max()  { return self().max(); }

    // FIXMe : There are others things to integrate in this interface : operations that are in op.hh

  protected:

    value_t _value;  // attribute

    rec_value() {}

    const real_value_t _real_value() const   // default impl.
    {
      return _value;
    }
  };



  //
  //  rec_scalar<Self>
  //
  //////////////////////////////


  template<class Self>
  class rec_scalar : public rec_value<Self>
  {
  };

  // FIXME: if not scalar, composed or vected?




  //
  //  real_value(val) proc
  //
  //////////////////////////////


  template<class T> inline
  const T
  real_value(const T& val) { return val; }

  template<class Self> inline
  const typename Self::real_value_t
  real_value(const rec_value<Self>& val) { return val.real_value(); }


} // end of oln



#endif // ! OLENA_VALUE_REC_VALUE_HH
