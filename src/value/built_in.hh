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

#ifndef OLENA_VALUE_BUILT_IN_HH
# define OLENA_VALUE_BUILT_IN_HH

# include <limits.h>
# include <float.h>
# include "meta/ufuncs.hh"
# include "value/predefs.hh"


namespace oln {

  namespace internal {


    //
    //  C_info
    //
    ///////////////////////////

    template<class T> struct C_info;

    template<> struct C_info<unsigned char > { enum { nbits = 8  }; };
    template<> struct C_info<  signed char > { enum { nbits = 8  }; };
    template<> struct C_info<unsigned short> { enum { nbits = 16 }; };
    template<> struct C_info<  signed short> { enum { nbits = 16 }; };
    template<> struct C_info<unsigned long > { enum { nbits = 32 }; };
    template<> struct C_info<  signed long > { enum { nbits = 32 }; };

    template<> struct C_info<  signed int  > { enum { nbits = 8 * sizeof(  signed int) }; };
    template<> struct C_info<unsigned int  > { enum { nbits = 8 * sizeof(unsigned int) }; };



    //
    //  oln_for_C
    //
    ///////////////////////////

    template<class T> struct oln_for_C;

    template<> struct oln_for_C<unsigned char > { typedef int_u< C_info<unsigned char >::nbits > ret; };
    template<> struct oln_for_C<  signed char > { typedef int_s< C_info<  signed char >::nbits > ret; };
    template<> struct oln_for_C<unsigned short> { typedef int_u< C_info<unsigned short>::nbits > ret; };
    template<> struct oln_for_C<  signed short> { typedef int_s< C_info<  signed short>::nbits > ret; };
    template<> struct oln_for_C<unsigned long > { typedef int_u< C_info<unsigned long >::nbits > ret; };
    template<> struct oln_for_C<  signed long > { typedef int_s< C_info<  signed long >::nbits > ret; };

    template<> struct oln_for_C<unsigned int  > { typedef int_u< C_info<unsigned int  >::nbits > ret; };
    template<> struct oln_for_C<  signed int  > { typedef int_s< C_info<  signed int  >::nbits > ret; };

    template<> struct oln_for_C<bool>   { typedef bin ret; };

    template<> struct oln_for_C<float>  { typedef float_p1 ret; };
    template<> struct oln_for_C<double> { typedef float_p2 ret; };



    //
    //  C_for_oln
    //
    ///////////////////////////

    template<unsigned nbits>
    struct C_for_int_u
    {
      enum {
	mbits = meta::pow2sup<nbits>::value  // FIXME: rename!!!
      };
      typedef typename C_for_int_u<mbits>::type type;
      static type max() {
	static type val = (type(1) << nbits) - type(1);  // FIXME: is static?
	return val;
      }
    };

    template<unsigned nbits>
    struct C_for_int_s
    {
      enum { mbits = meta::pow2sup<nbits>::value };
      typedef typename C_for_int_s<mbits>::type type;
      static type min() {
	static type val = - max() - type(1);
	return val;
      }
      static type max() {
	static type val = (type(1) << (nbits-1)) - type(1);
	return val;
      }
    };

    template<>
    struct C_for_int_u<8>
    {
      typedef unsigned char type;
      static type max() { return UCHAR_MAX; }
    };


    template<>
    struct C_for_int_s<8>
    {
      typedef signed char type;
      static type min() { return CHAR_MIN; }
      static type max() { return CHAR_MAX; }
    };

    template<>
    struct C_for_int_u<16>
    {
      typedef unsigned short type;
      static type max() { return USHRT_MAX; }
    };

    template<>
    struct C_for_int_s<16>
    {
      typedef signed short type;
      static type min() { return SHRT_MIN; }
      static type max() { return SHRT_MAX; }
    };

    template<>
    struct C_for_int_u<32>
    {
      typedef unsigned long type;
      static type max() { return ULONG_MAX; }
    };

    template<>
    struct C_for_int_s<32>
    {
      typedef signed long type;
      static type min() { return LONG_MIN; }
      static type max() { return LONG_MAX; }
    };


    // IEEE : Take a look at epsilon for some conditional (FIXME)

//      FLT_RADIX                             2
//      FLT_MANT_DIG                         24
//      FLT_DIG                               6
//      FLT_MIN_EXP                        -125
//      FLT_MIN_10_EXP                      -37
//      FLT_MAX_EXP                         128
//      FLT_MAX_10_EXP                      +38
//      FLT_MIN                 1.17549435E-38F
//      FLT_MAX                 3.40282347E+38F
//      FLT_EPSILON             1.19209290E-07F
 
//      DBL_MANT_DIG                         53
//      DBL_DIG                              15
//      DBL_MIN_EXP                       -1021
//      DBL_MIN_10_EXP                     -307
//      DBL_MAX_EXP                        1024
//      DBL_MAX_10_EXP                      308
//      DBL_MAX         1.7976931348623157E+308
//      DBL_MIN         2.2250738585072014E-308
//      DBL_EPSILON     2.2204460492503131E-016


    template<unsigned prec>
    struct C_for_float_p;

    template <>
    struct C_for_float_p< 1 > 
    { 
      typedef float type;
      static type min() { return FLT_MIN; }
      static type max() { return FLT_MAX; }
    };

    template <unsigned prec>
    struct C_for_float_p
    { 
      typedef double type;
      static type min() { return DBL_MIN; }
      static type max() { return DBL_MAX; }
    };
    
  } // end of internal


} // end of oln


#endif // ! OLENA_VALUE_BUILT_IN_HH
