// Copyright (C) 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef NTG_OPTRAITS_BUILTINS_FLOAT_HH
# define NTG_OPTRAITS_BUILTINS_FLOAT_HH

# include <ntg/config/system.hh> // for OLN_FLOAT_INFINITY

# include <mlc/cmp.hh>

# include <ntg/core/builtins_properties.hh>
# include <ntg/core/global_ops_traits.hh>
# include <ntg/real/optraits_scalar.hh>
# include <ntg/real/typetraits_builtins_float.hh>

namespace ntg
{

  //
  //  float
  //
  //////////

  template <>
  struct optraits<float> : public optraits_float<float>
  {
  private:
    // shortcuts
    typedef float self;
    typedef typetraits<self>::storage_type storage_type;

  public:
    // Don't define min() and max() for float and double.
    // This is error-prone because max()-1 == max().
    // Furthermore max()-min() can't be computed.
    static storage_type sup() { return OLN_FLOAT_INFINITY; }
    static storage_type inf() { return - sup (); }
  };

  namespace internal
  {

    struct all_float_traits
    {
      enum { commutative = true };
      typedef float ret;
      typedef float impl;
    };

    //
    // plus
    //

    template <unsigned nbits, class B>
    struct operator_plus_traits<float, int_u<nbits, B> > : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_plus_traits<float, int_s<nbits, B> > : all_float_traits {};

    // We need this because to_oln<float> gives float.
    // So when using cycle<float,..> + float, it searches
    // operator_plus_traits<float,float>
    template <>
    struct operator_plus_traits<float, float> : all_float_traits {};

    //
    // minus
    //

    template <unsigned nbits, class B>
    struct operator_minus_traits<float, int_u<nbits, B> > : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_minus_traits<float, int_s<nbits, B> > : all_float_traits {};

    template <>
    struct operator_minus_traits<float, float> : all_float_traits {};

    //
    // times
    //

    template <unsigned nbits, class B>
    struct operator_times_traits<float, int_u<nbits, B> > : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_times_traits<float, int_s<nbits, B> > : all_float_traits {};

    template <>
    struct operator_times_traits<float, float> : all_float_traits {};

    //
    // div
    //

    template <unsigned nbits, class B>
    struct operator_div_traits<float, int_u<nbits, B> > : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_div_traits<float, int_s<nbits, B> > : all_float_traits {};

    template <>
    struct operator_div_traits<float, float> : all_float_traits {};

    //
    // min
    //

    template <unsigned nbits, class B>
    struct operator_min_traits<float, int_u<nbits, B> > : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_min_traits<float, int_s<nbits, B> > : all_float_traits {};

    template <>
    struct operator_min_traits<float, float> : all_float_traits {};


    //
    // max
    //

    template <unsigned nbits, class B>
    struct operator_max_traits<float, int_u<nbits, B> > : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_max_traits<float, int_s<nbits, B> > : all_float_traits {};

    template <>
    struct operator_max_traits<float, float> : all_float_traits {};


    //
    // Comparison
    //

    template <unsigned nbits, class B>
    struct operator_cmp_traits<float, int_u<nbits, B> > : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_cmp_traits<float, int_s<nbits, B> > : all_float_traits {};


    template <>
    struct operator_cmp_traits<float, float> : all_float_traits {};

  } // end of internal


  //
  //  double
  //
  ///////////

  template <>
  struct optraits<double> : public optraits_float<double>
  {
  private:
    // shortcuts
    typedef double self;
    typedef typetraits<self>::storage_type storage_type;

  public:
    // Don't define min() and max() for float and double.
    // This is error-prone because max()-1 == max().
    // Furthermore max()-min() can't be computed.
    static storage_type sup() { return OLN_DOUBLE_INFINITY; }
    static storage_type inf() { return - sup (); }
  };

  namespace internal
  {

    struct all_double_traits
    {
      enum { commutative = true };
      typedef double ret;
      typedef double impl;
    };

    //
    // plus
    //

    template <unsigned nbits, class B>
    struct operator_plus_traits<double, int_u<nbits, B> > : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_plus_traits<double, int_s<nbits, B> > : all_double_traits {};

    template <>
    struct operator_plus_traits<double, double> : all_double_traits {};

    template <>
    struct operator_plus_traits<double, float> : all_double_traits {};

    //
    // minus
    //

    template <unsigned nbits, class B>
    struct operator_minus_traits<double, int_u<nbits, B> > : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_minus_traits<double, int_s<nbits, B> > : all_double_traits {};

    template <>
    struct operator_minus_traits<double, double> : all_double_traits {};

    template <>
    struct operator_minus_traits<double, float> : all_double_traits {};

    //
    // times
    //

    template <unsigned nbits, class B>
    struct operator_times_traits<double, int_u<nbits, B> > : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_times_traits<double, int_s<nbits, B> > : all_double_traits {};

    template <>
    struct operator_times_traits<double, double> : all_double_traits {};

    template <>
    struct operator_times_traits<double, float> : all_double_traits {};

    //
    // div
    //

    template <unsigned nbits, class B>
    struct operator_div_traits<double, int_u<nbits, B> > : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_div_traits<double, int_s<nbits, B> > : all_double_traits {};

    template <>
    struct operator_div_traits<double, double> : all_double_traits {};

    template <>
    struct operator_div_traits<double, float> : all_double_traits {};

    //
    // min
    //

    template <unsigned nbits, class B>
    struct operator_min_traits<double, int_u<nbits, B> > : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_min_traits<double, int_s<nbits, B> > : all_double_traits {};

    template <>
    struct operator_min_traits<double, double> : all_double_traits {};

    template <>
    struct operator_min_traits<double, float> : all_double_traits {};


    //
    // max
    //

    template <unsigned nbits, class B>
    struct operator_max_traits<double, int_u<nbits, B> > : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_max_traits<double, int_s<nbits, B> > : all_double_traits {};

    template <>
    struct operator_max_traits<double, double> : all_double_traits {};

    template <>
    struct operator_max_traits<double, float> : all_double_traits {};

   
    //
    // Comparison
    //

    template <unsigned nbits, class B>
    struct operator_cmp_traits<double, int_u<nbits, B> > : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_cmp_traits<double, int_s<nbits, B> > : all_double_traits {};

    template <>
    struct operator_cmp_traits<double, double> : all_double_traits {};

    template <>
    struct operator_cmp_traits<double, float> : all_double_traits {};


  } // end of internal

} // end of ntg

#endif // ndef NTG_OPTRAITS_BUILTINS_FLOAT_HH
