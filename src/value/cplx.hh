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


#ifndef OLENA_CPLX_HH
# define OLENA_CPLX_HH

#include <utility>

#include "value/op_traits.hh"
#include "meta/ucmp.hh"
#include "value/predefs.hh"
#include "debug.hh"

namespace oln
{

  template<class T>
  class cplx;

  namespace internal {

    // for arithm binops

#define DECLARE_TRAITS_FOR_CPLX_AND_SCALAR_OP(TYPE, OP)		\
    template<class T, unsigned N>				\
    struct operator_##OP##_traits< cplx<T>, TYPE<N> >		\
    {								\
      typedef cplx<typename oln::internal::deduced_from_traits	\
                    <						\
                      oln::internal::operator_##OP##_traits,	\
                      TYPE<N>,					\
                      T						\
                    >::ret_t> ret_t;				\
    };

#define DECLARE_TRAITS_FOR_CPLX_AND_SELF(OP)			\
    template<class T1, class T2>				\
    struct operator_##OP##_traits< cplx<T1>, cplx<T2> >		\
    {								\
      typedef cplx<typename oln::internal::deduced_from_traits	\
                    <						\
                      oln::internal::operator_##OP##_traits,	\
                      T1,					\
                      T2					\
                    >::ret_t> ret_t;				\
    };

#define DECLARE_TRAITS_FOR_CPLX(OP)			\
    DECLARE_TRAITS_FOR_CPLX_AND_SELF(OP)		\
    DECLARE_TRAITS_FOR_CPLX_AND_SCALAR_OP(int_u, OP)	\
    DECLARE_TRAITS_FOR_CPLX_AND_SCALAR_OP(int_s, OP)	\
    DECLARE_TRAITS_FOR_CPLX_AND_SCALAR_OP(float_p, OP)

    DECLARE_TRAITS_FOR_CPLX(plus);
    DECLARE_TRAITS_FOR_CPLX(minus);
    DECLARE_TRAITS_FOR_CPLX(times);
    DECLARE_TRAITS_FOR_CPLX(div);

    // other traits
    template<class T> struct value_traits< cplx<T> > { typedef std::pair<T, T> ret_t; };

    template<class T>
    struct real_value_traits< cplx<T> >
    {
      typedef typename real_value_traits<T>::ret_t comp_t;
      typedef std::pair<comp_t,comp_t> ret_t;
    };

    template<class T>
    struct larger_traits< cplx<T> >
    {
      typedef cplx<typename larger_traits<T>::ret_t> ret_t;
    };

    template<class T>
    struct signed_traits< cplx<T> >
    {
      typedef cplx<typename signed_traits<T>::ret_t> ret_t;
    };

    template<class T>
    struct unsigned_traits< cplx<T> >
    {
      typedef cplx<typename unsigned_traits<T>::ret_t> ret_t;
    };

    template<class T>
    struct cumul_traits< cplx<T> >
    {
      typedef cplx<typename cumul_traits<T>::ret_t> ret_t;
    };

  } // end of internal




  template<class T>
  struct cplx : public rec_value< cplx<T> >
  {
    typedef typename rec_value< cplx<T> >::value_t value_t;
  public:

    cplx() {
      _value = value_t(0,0);
    }

    cplx(value_t val) { _value = val; }
    operator value_t() const { return _value; }

    cplx(const T& re, const T& im) {
      _value = value_t(re, im);
    }

    template<class T2>
    cplx(const cplx<T2>& rhs)
    {
      _value = value_t(rhs.value());
    }

    template<class T2>
    cplx<T>& operator=(const cplx<T2>& rhs)
    {
      _value  = value_t(rhs.value());
      return *this;
    }

    template<class T2>
    cplx(const T2& re) {
      _value = value_t(T(to_self(re)), T(0));
    }
     template<class T2>
    cplx<T>& operator=(const T2& re) {
      _value = value_t(T(to_self(re)), T(0));
      return *this;
    }

    // accessors

    const T real() const { return _value.first; }
    T& real() { return _value.first; }
    const T imag() const { return _value.second; }
    T& imag() { return _value.second; }

    // methods

    const cplx<T> conj() const
    {
      return cplx<T>(real(), -imag());
    }

    // operators
    bool operator==(const cplx<T>& rhs) const { return _value == rhs.value(); }

    cplx<T>& operator+=(const cplx<T>& rhs) {
      real() += rhs.real();
      imag() += rhs.imag();
      return *this;
    }
    cplx<T>& operator-=(const cplx<T>& rhs) {
      real() -= rhs.real();
      imag() -= rhs.imag();
      return *this;
    }
    cplx<T>& operator*=(const cplx<T>& rhs) {
      T tmp_real = real();

      real() = real() * rhs.real() - imag() * rhs.imag();
      imag() = tmp_real * rhs.imag() + imag() * rhs.real();
      return *this;
    }

    cplx<T>& operator/=(const cplx<T>& rhs) {
      T tmp_denom = rhs.real() * rhs.real() + rhs.imag() * rhs.imag();
      cplx<T> & tmp_cplx = *this;
      tmp_cplx *= rhs.transp();
      real() /= tmp_denom;
      imag() /= tmp_denom;
      return *this;
    }

    static const cplx<T> unit() { return 1; }
    static const cplx<T> zero() { return 0; }
    ~cplx() { }

    static string name() {
      return string("cplx< ") + internal::debug_type<T>::name() + " >";
    }

  };
} // oln


template<class T>
inline
std::ostream& operator<<(std::ostream& ostr, const oln::cplx<T>& rhs)
{
  return ostr << rhs.real() << " + "
	      << rhs.imag() << "i";
}

#endif // OLENA_CPLX_HH
