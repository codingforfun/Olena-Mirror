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

#ifndef OLENA_VALUE_BIN_HH
# define OLENA_VALUE_BIN_HH

# include <oln/value/rec_value.hh>
# include <oln/value/int_u.hh>
# include <oln/value/int_s.hh>

# include <map>
# include <iostream>

namespace oln {


  namespace internal {

    template<>
    struct value_traits< bin >
    {
      typedef bool ret_t;
    };

    // FIXME: should be provided!
    template<> struct larger_traits<bin>   { typedef int_u8 ret_t; };
    template<> struct signed_traits<bin>   { typedef int_s8 ret_t; };
    template<> struct unsigned_traits<bin> { typedef int_u8 ret_t; };
    template<> struct cumul_traits<bin>    { typedef int_u8 ret_t; };

  } // end of internal



  class bin :
    public rec_scalar<bin>
  {
  public:
    typedef bool value_t;

    bin(value_t val = false) { _value = val;}
    operator value_t() const { return _value; }

    bin(const bin& rhs) : rec_scalar<bin>(rhs) { _value = rhs._value; }
    bin& operator=(const bin& rhs) { _value = rhs._value; return *this; }

    const bin operator!() const { bin tmp(!_value); return tmp; }

    bin& operator&=(const bin& rhs) { _value &= rhs._value; return *this; }
    bin& operator|=(const bin& rhs) { _value |= rhs._value; return *this; }
    bin& operator^=(const bin& rhs) { _value ^= rhs._value; return *this; }

    // FIXME: these 4 methods are required by src/utils/histogram.hh
    // it is definitely a bad thing :(
    // they should be removed...
    static const bin zero() { return 0; }
    static const bin unit() { return 1; }
    static const bin min()  { return 0; }
    static const bin max()  { return 1; }

    static string name() { return "bin"; }
  };


} // end of oln




namespace std {

  template<class _Key> struct hash;

  struct hash< oln::bin > {
    size_t operator()(const oln::bin& val) const {
      return val.value();
    }
  };

} // end of std



inline std::ostream&
operator<<(std::ostream& ostr, const oln::bin& rhs)
{
  return ostr << (rhs ? '|' : '-');
}


#endif // ! OLENA_VALUE_BIN_HH
