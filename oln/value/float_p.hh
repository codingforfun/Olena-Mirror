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

#ifndef OLENA_VALUE_FLOAT_P_HH
# define OLENA_VALUE_FLOAT_P_HH

# include <oln/core/contract.hh>
# include <oln/meta/cmp.hh>
# include <oln/value/rec_float.hh>
# include <oln/value/rec_int.hh>
# include <oln/value/predefs.hh>
# include <oln/value/built_in.hh>

namespace oln
{

  namespace internal {

    // for arithm binops

    // plus

    template<unsigned prec, unsigned nbits>
    struct operator_plus_traits<float_p<prec>, int_u<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec, unsigned nbits>
    struct operator_plus_traits<float_p<prec>, int_s<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec1, unsigned prec2>
    struct operator_plus_traits< float_p<prec1>,float_p<prec2> >
    {typedef float_p< meta::max<prec1,prec2>::ret > ret_t;};

    // minus

    template<unsigned prec, unsigned nbits>
    struct operator_minus_traits<float_p<prec>, int_u<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec, unsigned nbits>
    struct operator_minus_traits<float_p<prec>, int_s<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec1, unsigned prec2>
    struct operator_minus_traits< float_p<prec1>,float_p<prec2> >
    {typedef float_p< meta::max<prec1,prec2>::ret > ret_t;};


    // times

    template<unsigned prec, unsigned nbits>
    struct operator_times_traits<float_p<prec>, int_u<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec, unsigned nbits>
    struct operator_times_traits<float_p<prec>, int_s<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec1, unsigned prec2>
    struct operator_times_traits< float_p<prec1>,float_p<prec2> >
    {typedef float_p< meta::max<prec1,prec2>::ret > ret_t;};


    // div

    template<unsigned prec, unsigned nbits>
    struct operator_div_traits<float_p<prec>, int_u<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec, unsigned nbits>
    struct operator_div_traits<float_p<prec>, int_s<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec1, unsigned prec2>
    struct operator_div_traits< float_p<prec1>,float_p<prec2> >
    {typedef float_p< meta::max<prec1,prec2>::ret > ret_t;};

    // min

    template<unsigned prec, unsigned nbits>
    struct operator_min_traits<float_p<prec>, int_u<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec, unsigned nbits>
    struct operator_min_traits<float_p<prec>, int_s<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec1, unsigned prec2>
    struct operator_min_traits< float_p<prec1>,float_p<prec2> >
    {typedef float_p< meta::max<prec1,prec2>::ret > ret_t;};

    // max

    template<unsigned prec, unsigned nbits>
    struct operator_max_traits<float_p<prec>, int_u<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec, unsigned nbits>
    struct operator_max_traits<float_p<prec>, int_s<nbits> >
    {typedef float_p<prec> ret_t;};

    template<unsigned prec1, unsigned prec2>
    struct operator_max_traits< float_p<prec1>,float_p<prec2> >
    {typedef float_p< meta::max<prec1,prec2>::ret > ret_t;};


    // other traits

    template<unsigned prec>
    struct value_traits< float_p<prec> >
    {
      typedef typename internal::C_for_float_p<prec>::type ret_t;
    };



    template<unsigned prec>
    struct larger_traits< float_p<prec> >
    {
      typedef float_p<prec> ret_t;
    };

    template<unsigned prec>
    struct signed_traits< float_p<prec> >
    {
      typedef float_p<prec> ret_t;
    };

    template<unsigned prec>
    struct unsigned_traits< float_p<prec> >
    {
      typedef float_p<prec> ret_t;
    };

    template<unsigned prec>
    struct cumul_traits< float_p<prec> >
    {
      typedef float_p<prec> ret_t;
    };

  } // end of internal


  template<unsigned prec> // FIXME: prec == 3 should mean infinite precision?
  struct float_p : public rec_float< float_p<prec> >
  {
  public:
    typedef typename rec_value< float_p<prec> >::value_t value_t;

    float_p(value_t val = 0) { _value = val; }
    operator value_t() const { return _value; }

    template<unsigned prec2>
    float_p(const float_p<prec2>& rhs)
    {
      _value = rhs.value();
    }

    template<class Self>
    float_p(const rec_int<Self>& i)
    {
      _value = i.self().value();
    }

    bool operator==(const float_p<prec>& rhs) const { return _value == rhs._value; }
    bool operator< (const float_p<prec>& rhs) const { return _value < rhs._value; }
    bool operator< (float rhs)  const { return _value < rhs; }
    bool operator< (double rhs) const { return _value < rhs; }
    float_p<prec>& operator+=(const float_p<prec>& rhs) { _value += rhs._value; return *this; }
    float_p<prec>& operator-=(const float_p<prec>& rhs) { _value -= rhs._value; return *this; }
    float_p<prec>& operator*=(const float_p<prec>& rhs) { _value *= rhs._value; return *this; }
    float_p<prec>& operator/=(const float_p<prec>& rhs) { _value /= rhs._value; return *this; }

    static const float_p<prec> unit() { return 1; }
    static const float_p<prec> zero() { return 0; }
    static const float_p<prec> min()  { return internal::C_for_float_p<prec>::min(); }
    static const float_p<prec> max()  { return internal::C_for_float_p<prec>::max(); }

    ~float_p() { meta::less<prec,3>::ensure(); }

    static string name() {
      std::ostringstream out;
      out << "float_p<" << int(prec) << ">"<< std::ends;
      return out.str();
    }

  private:
    // w/o impl
//     bool operator==(bool) const;
//     bool operator<(bool) const;
//     int operator+=(bool);
//     int operator-=(bool);
//     int operator*=(bool);
//     int operator/=(bool);
  };


} // end of oln




template<unsigned prec>
inline
std::ostream& operator<<(std::ostream& ostr, const oln::float_p<prec>& rhs)
{
  return ostr << rhs.value();
}



#endif
