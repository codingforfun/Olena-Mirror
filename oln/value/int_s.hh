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

#ifndef OLENA_VALUE_INT_S_HH
# define OLENA_VALUE_INT_S_HH

# include <oln/meta/cmp.hh>
# include <oln/value/rec_int.hh>
# include <oln/value/built_in.hh>
# include <oln/value/predefs.hh>

# include <map>
# include <fstream>

namespace oln
{


  namespace internal {

    // for arithm binops

    // plus

    template<unsigned nbits, unsigned mbits>
    struct operator_plus_traits<int_s<nbits>,int_s<mbits> >
    {typedef int_s<meta::max<nbits,mbits>::ret + 1> ret_t;};

    template<unsigned nbits, unsigned mbits>
    struct operator_plus_traits<int_s<nbits>,int_u<mbits> >
    {typedef int_s<meta::max<nbits,mbits+1>::ret + 1> ret_t;};

    // minus

    template<unsigned nbits, unsigned mbits>
    struct operator_minus_traits<int_s<nbits>,int_s<mbits> >
    {typedef int_s<meta::max<nbits,mbits>::ret + 1> ret_t;};

    template<unsigned nbits, unsigned mbits>
    struct operator_minus_traits<int_s<nbits>,int_u<mbits> >
    {typedef int_s<meta::max<nbits,mbits+1>::ret + 1> ret_t;};

    // times

    template<unsigned nbits, unsigned mbits>
    struct operator_times_traits<int_s<nbits>,int_s<mbits> >
    {typedef int_s<mbits + nbits> ret_t;};

    template<unsigned nbits, unsigned mbits>
    struct operator_times_traits<int_s<nbits>,int_u<mbits> >
    {typedef int_s<nbits + mbits+1> ret_t;};


    // div

    template<unsigned nbits, unsigned mbits>
    struct operator_div_traits<int_s<nbits>,int_s<mbits> >
    {typedef int_s<nbits> ret_t;};

    template<unsigned nbits, unsigned mbits>
    struct operator_div_traits<int_s<nbits>,int_u<mbits> >
    {typedef int_s<nbits> ret_t;};

    // min

    template<unsigned nbits, unsigned mbits>
    struct operator_min_traits<int_s<nbits>,int_s<mbits> >
    {typedef int_s<meta::max<nbits,mbits>::ret> ret_t;};

    // We don't allow (indirect) comparisons between unsigned and signed.
    // template<unsigned nbits, unsigned mbits>
    // struct operator_min_traits<int_s<nbits>,int_u<mbits> >
    // {typedef int_s<nbits>::ret> ret_t;};

    // max

    template<unsigned nbits, unsigned mbits>
    struct operator_max_traits<int_s<nbits>,int_s<mbits> >
    {typedef int_s<meta::max<nbits,mbits>::ret> ret_t;};

    // We don't allow (indirect) comparisons between unsigned and signed.
    // template<unsigned nbits, unsigned mbits>
    // struct operator_max_traits<int_s<nbits>,int_u<mbits> >
    // {typedef int_s<meta::max<nbits,mbits>::ret> ret_t;};

    // other traits

    template<unsigned nbits>
    struct value_traits< int_s<nbits> >
    {
      typedef typename internal::C_for_int_s<nbits>::type ret_t;
    };

    template<unsigned nbits>
    struct larger_traits< int_s<nbits> >
    {
      typedef int_s<nbits+1> ret_t;
    };

    template<unsigned nbits>
    struct signed_traits< int_s<nbits> >
    {
      typedef int_s<nbits> ret_t;
    };

    template<unsigned nbits>
    struct unsigned_traits< int_s<nbits> >
    {
      typedef int_u<nbits+1> ret_t;
    };

    template<unsigned nbits>
    struct cumul_traits< int_s<nbits> >
    {
      typedef float_p1 ret_t;  // security
    };


  } // end of internal



  template<unsigned nbits>
  class int_s :
    public rec_int_s< int_s<nbits> >
  {
  public:
    typedef typename internal::C_for_int_s<nbits>::type value_t;

    int_s(value_t val = 0) { _value = val; }
    operator value_t() const { return _value; }


    template<unsigned mbits>
    int_s(const int_s<mbits>& rhs)
    {
      meta::lesseq<mbits,nbits>::ensure();
      _value = rhs.value();
    }

    template<unsigned mbits>
    int_s<nbits>& operator=(const int_s<mbits>& rhs)
    {
      meta::lesseq<mbits,nbits>::ensure();
      _value = rhs.value();
      return *this;
    }


    template<unsigned mbits>
    int_s(const int_u<mbits>& rhs)
    {
      meta::less<mbits,nbits>::ensure();
      _value = rhs.value();
    }

    template<unsigned mbits>
    int_s<nbits>& operator=(const int_u<mbits>& rhs)
    {
      meta::less<mbits,nbits>::ensure();
      _value = rhs.value();
      return *this;
    }


    template<class T>
    int_s(const internal::any_int<T>& a)
    {
      precondition(a.value >= internal::C_for_int_s<nbits>::min());
      precondition(a.value <= internal::C_for_int_s<nbits>::max());
      _value = value_t(a.value);
    }

    template<class T>
    int_s<nbits>& operator=(const internal::any_int<T>& a)
    {
      precondition(a.value >= internal::C_for_int_s<nbits>::min());
      precondition(a.value <= internal::C_for_int_s<nbits>::max());
      _value = value_t(a.value);
      return *this;
    }

    bool operator==(const int_s<nbits>& rhs) const {
       return _value == rhs._value;
    }
    bool operator< (const int_s<nbits>& rhs) const {
      return _value < rhs._value;
    }
    int_s<nbits>& operator+=(const int_s<nbits>& rhs) {
      _value += rhs._value; return *this;
    }
    int_s<nbits>& operator-=(const int_s<nbits>& rhs) {
      _value -= rhs._value; return *this;
    }
    int_s<nbits>& operator*=(const int_s<nbits>& rhs) {
      _value *= rhs._value; return *this;
    }
    int_s<nbits>& operator/=(const int_s<nbits>& rhs) {
      _value /= rhs._value; return *this;
    }

    static const int_s<nbits> unit() { return 1; }
    static const int_s<nbits> zero() { return 0; }
    static const int_s<nbits> min()  { return internal::C_for_int_s<nbits>::min(); }
    static const int_s<nbits> max()  { return internal::C_for_int_s<nbits>::max(); }

    static string name() {
      std::ostringstream out;
      out << "int_s<" << int(nbits) << ">"<< std::ends;
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



namespace std {

  template<class _Key> struct hash;

  template<unsigned nbits>
  struct hash< oln::int_s<nbits> > {
    size_t operator()(const oln::int_s<nbits>& val) const {
      return val.value();
    }
  };

} // end of std




template<unsigned nbits>
inline
std::ostream& operator<<(std::ostream& ostr, const oln::int_s<nbits>& rhs)
{
  ostr.width(unsigned(log(double(nbits))/log(2.f)));
  ostr << rhs.value();
  return ostr;
}

inline
std::ostream& operator<<(std::ostream& ostr, const oln::int_s<8>& rhs)
{
   ostr.width(3);
   return ostr << int(rhs.value());
}


#endif // ! OLENA_VALUE_INT_S_HH
