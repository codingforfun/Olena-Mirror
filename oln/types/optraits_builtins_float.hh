// Copyright 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_OPTRAITS_BUILTINS_FLOAT_HH
# define OLENA_VALUE_OPTRAITS_BUILTINS_FLOAT_HH

# include <oln/meta/cmp.hh>

# include <oln/types/builtins_properties.hh>

# include <oln/types/global_ops_traits.hh>

# include <oln/types/optraits_scalar.hh>

# include <oln/types/typetraits_builtins_float.hh>

namespace oln
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
    typedef typetraits<self>::store store_type;

  public:
    static store_type min() { return C_for_sfloat::min(); }
    static store_type max() { return C_for_sfloat::max(); }
  };

  namespace internal
  {

    //
    // plus
    //

    template <unsigned nbits, class B>
    struct operator_plus_traits<float, int_u<nbits, B> >
    {
      enum { commutative = true };
      typedef float ret;
      typedef float impl;
    };

    template <unsigned nbits, class B>
    struct operator_plus_traits<float, int_s<nbits, B> >
    {
      enum { commutative = true };
      typedef float ret;
      typedef float impl;
    };

    //
    // minus
    //

    template <unsigned nbits, class B>
    struct operator_minus_traits<float, int_u<nbits, B> >
    {
      enum { commutative = true };
      typedef float ret;
      typedef float impl;
    };

    template <unsigned nbits, class B>
    struct operator_minus_traits<float, int_s<nbits, B> >
    {
      enum { commutative = true };
      typedef float ret;
      typedef float impl;
    };

    //
    // times
    //

    template <unsigned nbits, class B>
    struct operator_times_traits<float, int_u<nbits, B> >
    {
      enum { commutative = true };
      typedef float ret;
      typedef float impl;
    };

    template <unsigned nbits, class B>
    struct operator_times_traits<float, int_s<nbits, B> >
    {
      enum { commutative = true };
      typedef float ret;
      typedef float impl;
    };

    //
    // div
    //

    template <unsigned nbits, class B>
    struct operator_div_traits<float, int_u<nbits, B> >
    {
      enum { commutative = true };
      typedef float ret;
      typedef float impl;
    };

    template <unsigned nbits, class B>
    struct operator_div_traits<float, int_s<nbits, B> >
    {
      enum { commutative = true };
      typedef float ret;
      typedef float impl;
    };

  } // end of namespace internal


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
    typedef typetraits<self>::store store_type;

  public:
    static store_type min() { return C_for_dfloat::min(); }
    static store_type max() { return C_for_dfloat::max(); }
  };

  namespace internal
  {

    //
    // plus
    //

    template <unsigned nbits, class B>
    struct operator_plus_traits<double, int_u<nbits, B> >
    {
      enum { commutative = true };
      typedef double ret;
      typedef double impl;
    };

    template <unsigned nbits, class B>
    struct operator_plus_traits<double, int_s<nbits, B> >
    {
      enum { commutative = true };
      typedef double ret;
      typedef double impl;
    };

    //
    // minus
    //

    template <unsigned nbits, class B>
    struct operator_minus_traits<double, int_u<nbits, B> >
    {
      enum { commutative = true };
      typedef double ret;
      typedef double impl;
    };

    template <unsigned nbits, class B>
    struct operator_minus_traits<double, int_s<nbits, B> >
    {
      enum { commutative = true };
      typedef double ret;
      typedef double impl;
    };

    //
    // times
    //

    template <unsigned nbits, class B>
    struct operator_times_traits<double, int_u<nbits, B> >
    {
      enum { commutative = true };
      typedef double ret;
      typedef double impl;
    };

    template <unsigned nbits, class B>
    struct operator_times_traits<double, int_s<nbits, B> >
    {
      enum { commutative = true };
      typedef double ret;
      typedef double impl;
    };

    //
    // div
    //

    template <unsigned nbits, class B>
    struct operator_div_traits<double, int_u<nbits, B> >
    {
      enum { commutative = true };
      typedef double ret;
      typedef double impl;
    };

    template <unsigned nbits, class B>
    struct operator_div_traits<double, int_s<nbits, B> >
    {
      enum { commutative = true };
      typedef double ret;
      typedef double impl;
    };

  } // end of namespace internal

} // end of namespace oln

#endif // ndef OLENA_VALUE_OPTRAITS_BUILTINS_FLOAT_HH
