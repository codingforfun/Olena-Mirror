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

#ifndef NTG_REAL_BUILTIN_FLOAT_HH
# define NTG_REAL_BUILTIN_FLOAT_HH

# include <ntg/config/system.hh>
# include <ntg/real/behavior.hh>
# include <ntg/core/internal/global_ops_traits.hh>
# include <ntg/core/internal/traits.hh>
# include <ntg/real/builtin_properties.hh>
# include <ntg/real/optraits_real.hh>
# include <ntg/core/predecls.hh>

# include <mlc/cmp.hh>

namespace ntg {

  namespace internal {

    /*------------------.
    | typetraits<float> |
    `------------------*/

    template <>
    struct typetraits<float> : public typetraits<float_value<float> >
    {
      typedef float	self;
      typedef decimal	abstract_type;
      typedef self	ntg_type;

      ntg_build_value_type(float_value<E>);

      typedef optraits<self>	optraits_type;
      typedef unsafe::get<self>	behavior_type;
      typedef unsafe		abstract_behavior_type;

      typedef self base_type;
      typedef self storage_type;
      typedef self signed_type;
      typedef self unsigned_type;
      typedef self cumul_type;
      typedef self largest_type;
      typedef self signed_largest_type;
      typedef self signed_cumul_type;
      typedef self unsigned_largest_type;
      typedef self unsigned_cumul_type;
      typedef self integer_type;
    };

    /*-------------------.
    | typetraits<double> |
    `-------------------*/

    template <>
    struct typetraits<double> : public typetraits<float_value<double> >
    {
      typedef double	self;
      typedef decimal	abstract_type;
      typedef self	ntg_type;

      ntg_build_value_type(float_value<E>);

      typedef optraits<self>	optraits_type;
      typedef unsafe::get<self> behavior_type;
      typedef unsafe		abstract_behavior_type;

      typedef self base_type;
      typedef self storage_type;
      typedef self signed_type;
      typedef self unsigned_type;
      typedef self cumul_type;
      typedef self largest_type;
      typedef self signed_largest_type;
      typedef self signed_cumul_type;
      typedef self unsigned_largest_type;
      typedef self unsigned_cumul_type;
      typedef self integer_type;
    };

    /*----------------.
    | optraits<float> |
    `----------------*/

    template <>
    struct optraits<float> : public optraits<float_value<float> >
    {
    private:
      typedef float self;
      typedef typetraits<self>::storage_type storage_type_;

    public:
      // Don't define min() and max() for float and double.
      // This is error-prone because max()-1 == max().
      // Furthermore max()-min() can't be computed.
      static storage_type_ sup() { return NTG_FLOAT_INFINITY; }
      static storage_type_ inf() { return - sup (); }
      static std::string name()  { return "float"; }
    };

    /*--------------------------.
    | operator traits for float |
    `--------------------------*/

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
    struct operator_traits<operator_plus, float, int_u<nbits, B> >
      : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_plus, float, int_s<nbits, B> >
      : all_float_traits {};

    // We need this because ntg_cast(float) gives float.
    // So when using cycle<float,..> + float, it searches
    // operator_traits<operator_plus, float,float>
    template <>
    struct operator_traits<operator_plus, float, float>
      : all_float_traits {};

    //
    // minus
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_minus, float, int_u<nbits, B> >
      : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_minus, float, int_s<nbits, B> >
      : all_float_traits {};

    template <>
    struct operator_traits<operator_minus, float, float>
      : all_float_traits {};

    //
    // times
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_times, float, int_u<nbits, B> >
      : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_times, float, int_s<nbits, B> >
      : all_float_traits {};

    template <>
    struct operator_traits<operator_times, float, float>
      : all_float_traits {};

    //
    // div
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_div, float, int_u<nbits, B> >
      : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_div, float, int_s<nbits, B> >
      : all_float_traits {};

    template <>
    struct operator_traits<operator_div, float, float>
      : all_float_traits {};

    //
    // min
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_min, float, int_u<nbits, B> >
      : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_min, float, int_s<nbits, B> >
      : all_float_traits {};

    template <>
    struct operator_traits<operator_min, float, float>
      : all_float_traits {};

    //
    // max
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_max, float, int_u<nbits, B> >
      : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_max, float, int_s<nbits, B> >
      : all_float_traits {};

    template <>
    struct operator_traits<operator_max, float, float>
      : all_float_traits {};

    //
    // Comparison
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_cmp, float, int_u<nbits, B> >
      : all_float_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_cmp, float, int_s<nbits, B> >
      : all_float_traits {};

    template <>
    struct operator_traits<operator_cmp, float, float>
      : all_float_traits {};

    /*-----------------.
    | optraits<double> |
    `-----------------*/

    template <>
    struct optraits<double> : public optraits<float_value<double> >
    {
    private:
      // shortcuts
      typedef double self;
      typedef typetraits<self>::storage_type storage_type_;

    public:
      // Don't define min() and max() for float and double.
      // This is error-prone because max()-1 == max().
      // Furthermore max()-min() can't be computed.
      static storage_type_ sup() { return NTG_DOUBLE_INFINITY; }
      static storage_type_ inf() { return - sup (); }
      static std::string name()  { return "double"; }
    };

    /*---------------------------.
    | operator traits for double |
    `---------------------------*/

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
    struct operator_traits<operator_plus, double, int_u<nbits, B> >
      : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_plus, double, int_s<nbits, B> >
      : all_double_traits {};

    template <>
    struct operator_traits<operator_plus, double, double>
      : all_double_traits {};

    template <>
    struct operator_traits<operator_plus, double, float>
      : all_double_traits {};

    //
    // minus
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_minus, double, int_u<nbits, B> >
      : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_minus, double, int_s<nbits, B> >
      : all_double_traits {};

    template <>
    struct operator_traits<operator_minus, double, double>
      : all_double_traits {};

    template <>
    struct operator_traits<operator_minus, double, float>
      : all_double_traits {};

    //
    // times
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_times, double, int_u<nbits, B> >
      : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_times, double, int_s<nbits, B> >
      : all_double_traits {};

    template <>
    struct operator_traits<operator_times, double, double>
      : all_double_traits {};

    template <>
    struct operator_traits<operator_times, double, float>
      : all_double_traits {};

    //
    // div
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_div, double, int_u<nbits, B> >
      : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_div, double, int_s<nbits, B> >
      : all_double_traits {};

    template <>
    struct operator_traits<operator_div, double, double>
      : all_double_traits {};

    template <>
    struct operator_traits<operator_div, double, float>
      : all_double_traits {};

    //
    // min
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_min, double, int_u<nbits, B> >
      : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_min, double, int_s<nbits, B> >
      : all_double_traits {};

    template <>
    struct operator_traits<operator_min, double, double>
      : all_double_traits {};

    template <>
    struct operator_traits<operator_min, double, float>
      : all_double_traits {};

    //
    // max
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_max, double, int_u<nbits, B> >
      : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_max, double, int_s<nbits, B> >
      : all_double_traits {};

    template <>
    struct operator_traits<operator_max, double, double>
      : all_double_traits {};

    template <>
    struct operator_traits<operator_max, double, float>
      : all_double_traits {};

    //
    // Comparison
    //

    template <unsigned nbits, class B>
    struct operator_traits<operator_cmp, double, int_u<nbits, B> >
      : all_double_traits {};

    template <unsigned nbits, class B>
    struct operator_traits<operator_cmp, double, int_s<nbits, B> >
      : all_double_traits {};

    template <>
    struct operator_traits<operator_cmp, double, double>
      : all_double_traits {};

    template <>
    struct operator_traits<operator_cmp, double, float>
      : all_double_traits {};

  } // end of internal.

} // end of ntg.

#endif // !NTG_REAL_BUILTIN_FLOAT_HH
