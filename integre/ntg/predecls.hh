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

#ifndef NTG_PREDECLS_HH
# define NTG_PREDECLS_HH

# include <mlc/type.hh>
# include <ntg/cplx_representation.hh>

namespace ntg
{

  struct non_sense;

  // behaviours
  class unsafe;
  class strict;
  class saturate;

  // Put types and operators definitions in a subnamespace, then
  // the important types in ntg::, and rely on Koenig lookup to find
  // relevant operators.  This way our template operators do not
  // supersede the build-in operator.
  namespace type_definitions
  {
    // intervals
    template <class T, T i_min, T i_max> class		bounded;
    template <unsigned i_min, unsigned i_max> class	bounded_u;
    template <signed i_min, signed i_max> class		bounded_s;

    // scalar types
    template <unsigned nbits, class behaviour = strict>	class int_u;
    template <unsigned nbits, class behaviour = strict>	class int_s;
    template <class T, class interval, class behaviour = strict> class range;
    template <class T, class interval> class		cycle;
  } // end of type_definitions

  using type_definitions::bounded_u;
  using type_definitions::bounded_s;
  using type_definitions::int_u;
  using type_definitions::int_s;
  using type_definitions::range;
  using type_definitions::cycle;

  // floats
  typedef float						sfloat;
  typedef double					dfloat;

  // enumerated types
  namespace type_definitions
  {
    class bin;
  }
  using type_definitions::bin;

  //
  // vectorial types
  //

  // vec

  namespace type_definitions 
  {
    template <unsigned N, class T, class Self = mlc::bottom> class vec;
  }
  using type_definitions::vec;

  // cplx

  using type_definitions::cplx_representation;
  namespace type_definitions 
  {
    template <cplx_representation R, class T> class cplx;
  }
  using type_definitions::cplx;
  using type_definitions::rect;
  using type_definitions::polar;

  //
  // Colors
  //

  namespace type_definitions
  {
    template <unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct color;

    template<int lval, int uval>
    struct interval;
  }
  using type_definitions::color;
  using type_definitions::interval;

  //
  // Scalar types
  //

  // int_u

  typedef int_u<8, strict>	int_u8;
  typedef int_u<8, unsafe>	int_u8u;
  typedef int_u<8, saturate>	int_u8s;

  typedef int_u<16, strict>	int_u16;
  typedef int_u<16, unsafe>	int_u16u;
  typedef int_u<16, saturate>	int_u16s;

  typedef int_u<32, strict>	int_u32;
  typedef int_u<32, unsafe>	int_u32u;
  typedef int_u<32, saturate>	int_u32s;


  // int_s

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

} // end of ntg

#endif
