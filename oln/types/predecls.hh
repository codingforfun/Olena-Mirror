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

#ifndef OLENA_VALUE_PREDECLS_HH
# define OLENA_VALUE_PREDECLS_HH

namespace oln {

  struct non_sense;

  // rec types

  template<class Self> class rec_value;
  template<class Self> class rec_scalar;
  template<class Self> class rec_int;
  template<class Self> class rec_int_u;
  template<class Self> class rec_int_s;
  template<class Self> class rec_float;


  // behaviours
  class unsafe;
  class strict;
  class saturate;

  // scalar types

  template<unsigned nbits, class behaviour> class int_u;
  typedef int_u<0, strict>	int_uoo; // FIXME: does not exist yet!

  typedef int_u<8, strict>	int_u8;
  typedef int_u<8, unsafe>	int_u8u;
  typedef int_u<8, saturate>	int_u8s;

  typedef int_u<16, strict>	int_u16;
  typedef int_u<16, unsafe>	int_u16u;
  typedef int_u<16, saturate>	int_u16s;

  typedef int_u<32, strict>	int_u32;
  typedef int_u<32, unsafe>	int_u32u;
  typedef int_u<32, saturate>	int_u32s;

  // FIXME : decomment when ready
  //   template<> struct int_u<1> {}; // see line below
  //   class bin;
  //   typedef bin       int_u1;

  template<unsigned nbits, class behaviour> class int_s;
  typedef int_s<0, strict>	int_soo; // FIXME: does not exist yet!
  typedef non_sense		int_s1;

  typedef int_s<8, strict>	int_s8;
  typedef int_s<8, unsafe>	int_s8u;
  typedef int_s<8, saturate>	int_s8s;

  typedef int_s<16, strict>	int_s16;
  typedef int_s<16, unsafe>	int_s16u;
  typedef int_s<16, saturate>	int_s16s;

  typedef int_s<32, strict>	int_s32;
  typedef int_s<32, unsafe>	int_s32u;
  typedef int_s<32, saturate>	int_s32s;

  template<class T, class interval, class behaviour> class range;

  // FIXME : activate when ready

//   template<unsigned prec> struct float_p;
//   typedef float_p<0> float_poo; // FIXME: does not exist yet!
//   typedef float_p<1> float_p1;
//   typedef float_p<2> float_p2;
//   // for backward compatibility:
//   typedef float_p<1> sfloat;
//   typedef float_p<2> dfloat;


  // other types

//   template<class T> struct cplx;

//   template<unsigned N,class T> struct vec;

//   template <unsigned ncomps, unsigned qbits, template <unsigned>
//   class color_system>
//   struct color;
} // end of namespace oln_predelcs

#endif
