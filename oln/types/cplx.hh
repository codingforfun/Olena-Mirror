// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_CPLX_HH
# define OLENA_VALUE_CPLX_HH

# include <math.h>
# include <oln/types/typetraits.hh>
# include <oln/types/vec.hh>
# include <oln/meta/cmp.hh>
# include <oln/types/cplx_representation.hh>

namespace oln {

  template <type_definitions::cplx_representation R, class T>
  struct typetraits<cplx<R, T> >
  {
    typedef cplx<R, T> self;
    typedef optraits<self> optraits;

    typedef typename typetraits<T>::base_type  base_type[2];
    typedef T storage_type[2];
    typedef typename typetraits<T>::cumul_type cumul_type[2];

    // FIXME: useful?
    typedef self op_traits;
  };

  namespace type_definitions {

    //
    // Non-specialized cplx class (declared in predecls.hh) :
    // template <cplx_representation R, class T> class cplx;
    //
    //////////////////////////////////////
    
    //
    // cplx<rect, T>
    //
    //////////////////////////////////////

    template <class T>
    class cplx<rect, T> : public vec<2, T>
    {
    public:

      cplx(const T& re = 0, const T& im = 0)
      {
	_value[0] = re;
	_value[1] = im;
      }

      cplx<rect, T>& operator=(const T& r)
      {
	_value[0] = r;
	_value[1] = 0;
	return *this;
      }

      template<cplx_representation R, class T2>
      cplx(const cplx<R, T2>& rhs)
      {
	_value[0] = (T)rhs.real();
	_value[1] = (T)rhs.imag();
      }

      template<cplx_representation R, class T2>
      cplx<rect, T>& operator=(const cplx<R, T2>& rhs)
      {
	_value[0] = (T)rhs.real();
	_value[1] = (T)rhs.imag();
	return *this;
      }

      template<class T2>
      cplx(const vec<2, T2>& rhs)
      {
	_value[0] = (T)rhs[0];
	_value[1] = (T)rhs[1];
      }

      template<class T2>
      cplx<rect, T>& operator=(const vec<2, T2>& rhs)
      {
	_value[0] = (T)rhs[0];
	_value[1] = (T)rhs[1];
	return *this;
      }

      ~cplx()
      {
	is_a(optraits<T>, oln::optraits_scalar)::ensure();
      }

      // accessors

      const T real() const { return _value[0]; }
      T& real() { return _value[0]; }

      const T imag() const { return _value[1]; }
      T& imag() { return _value[1]; }

      const T first() const { return _value[0]; }
      T& first() { return _value[0]; }

      const T second() const { return _value[1]; }
      T& second() { return _value[1]; }

      // methods

      const dfloat magn() const
      {
	return sqrt(_value[0] * _value[0] + _value[1] * _value[1]);
      }

      const dfloat angle() const
      {
	return atan2(_value[1], _value[0]);
      }

      const cplx<rect, T> conj() const
      {
	return cplx<rect, T>(_value[0], -_value[1]);
      }

      const cplx<rect, T> invert() const
      {
	return cplx<rect, T>(-_value[0], -_value[1]);
      }

      const cplx<polar, dfloat> to_polar() const
      {
	return cplx<polar, dfloat>(magn(), angle());
      }

    };

    //
    // cplx<polar, T>
    //
    //////////////////////////////////////

    template <class T>
    class cplx<polar, T> : public rec_vector<cplx<polar, T> >
    {
    public:

      cplx(const T& ma = 0, const T& an = 0)
      {
	precondition (ma >= 0);
	_value[0] = ma;
	_value[1] = an;
      }

      cplx<rect, T>& operator=(const T& r)
      {
	_value[0] = r;
	_value[1] = 0;
	return *this;
      }

      template<cplx_representation R, class T2>
      cplx(const cplx<R, T2>& rhs)
      {
	_value[0] = (T)rhs.magn();
	_value[1] = (T)rhs.angle();
      }

      template<cplx_representation R, class T2>
      cplx<polar, T>& operator=(const cplx<R, T2>& rhs)
      {
	_value[0] = (T)rhs.magn();
	_value[1] = (T)rhs.angle();
	return *this;
      }

      ~cplx()
      {
	is_a(optraits<T>, oln::optraits_scalar)::ensure();
      }

      // accessors

      const T magn() const { return _value[0]; }
      T& magn() { return _value[0]; }

      const T angle() const { return _value[1]; }
      T& angle() { return _value[1]; }

      const T first() const { return _value[0]; }
      T& first() { return _value[0]; }

      const T second() const { return _value[1]; }
      T& second() { return _value[1]; }

      // methods

      const dfloat real() const
      {
	return _value[0] * cos(_value[1]);
      }

      const dfloat imag() const
      {
	return _value[0] * sin(_value[1]);
      }

      const cplx<polar, T> conj() const
      {
	return cplx<polar, T>(_value[0], -_value[1]);
      }

      const cplx<rect, T> invert() const
      {
	return cplx<rect, T>(_value[0], _value[1] + M_PI);
      }

      const cplx<rect, dfloat> to_rect() const
      {
	return cplx<rect, dfloat>(real(), imag());
      }

    };

  } // end of namespace type_definitions

} // end of oln

template<class T>
inline
std::ostream& operator<<(std::ostream& ostr, const oln::cplx<oln::rect, T>& rhs)
{
  return ostr << rhs.real() << " + "
	      << rhs.imag() << "i";
}

template<class T>
inline
std::ostream& operator<<(std::ostream& ostr, const oln::cplx<oln::polar, T>& rhs)
{
  return ostr << rhs.magn() << " * exp("
	      << rhs.angle() << "i)";
}

#endif // ! OLENA_VALUE_CPLX_HH
