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

#ifndef OLENA_VALUE_REC_INT_HH
# define OLENA_VALUE_REC_INT_HH

#include <oln/value/rec_value.hh>
#include <oln/value/to_oln.hh>


namespace oln
{

  // recursive super-classes for integer types

  // for instance:
  //
  //   rec_value< int_u8 >
  //    |
  //   rec_scalar< int_u8 >
  //    |
  //   rec_int< int_u8 >
  //    |
  //   rec_int_u< int_u8 >
  //    |
  //   int_u<8>  ==  int_u8


  template<class Self>
  struct rec_int : public rec_scalar<Self>
  {
  protected:
    rec_int() {}
  };

  template<class Self>
  struct rec_int_u : public rec_int<Self>
  {
//     static const Self median() { return Self::max() / 2 + 1; }
  protected:
    rec_int_u() {}
  };


  template<class Self>
  struct rec_int_s : public rec_int<Self>
  {
//     static const Self median() { return 0; }
  protected:
    rec_int_s() {}
  };


  namespace internal {

    template<class Self>
    struct self_traits< rec_scalar<Self> >
    {
      typedef Self ret_t;
      static const Self to_self(const rec_scalar<Self>& val) { return val.self(); }
    };

    template<class Self>
    struct self_traits< rec_int_u<Self> >
    {
      typedef Self ret_t;
      static const Self to_self(const rec_int_u<Self>& val) { return val.self(); }
    };

    template<class Self>
    struct self_traits< rec_int_s<Self> >
    {
      typedef Self ret_t;
      static const Self to_self(const rec_int_s<Self>& val) { return val.self(); }
    };

    // any_int is used to allow downcast between oln integer types
    // these downcasts are explicit due to cast::sure, Cf. below.
    // Trick:
    // int_u<nbits>::operator=(const internal::any_int<T>&)
    // has a different behavior than int_u<nbits>::operator=(const int_u<mbits>&).
    template<class Self>
    struct any_int
    {
      any_int(const rec_int<Self>& value) : value(value.self()) {}
      typename Self::value_t value;
    };

  } // end of internal



  namespace cast {

    template<class Self> inline
    oln::internal::any_int<Self> sure(const rec_int<Self>& i)
    {
      oln::internal::any_int<Self> tmp(i);
      return tmp;
    }

  } // end of cast



} // end of oln


#endif // ! OLENA_VALUE_REC_INT_HH
