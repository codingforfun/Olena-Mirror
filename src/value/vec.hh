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

#ifndef OLENA_VALUE_VEC_HH
# define OLENA_VALUE_VEC_HH

# include "value/debug.hh"
# include "value/rec_value.hh"
# include "value/predefs.hh"
# include "value/built_in.hh"
# include "core/point.hh"
# include "core/dpoint.hh"
# include "meta/array.hh"
# include "meta/cmp.hh"

namespace oln {


  // predecl
  template <unsigned N, class T> class vec;


  namespace internal {


    template <unsigned N,class T1,class T2>
    struct operator_plus_traits<vec<N,T1>,vec<N,T2> >
    {
      typedef vec<N, typename oln::internal::deduced_from_traits
      < oln::internal::operator_plus_traits, T1, T2 >::ret_t> ret_t;
    };

    template <unsigned N,class T1,class T2>
    struct operator_minus_traits<vec<N,T1>,vec<N,T2> >
    {
      typedef vec<N, typename oln::internal::deduced_from_traits
      <oln::internal::operator_minus_traits, T1, T2 >::ret_t> ret_t;
    };

    template <unsigned N,class T1,class T2>
    struct operator_times_traits<vec<N,T1>,T2 >
    {
      typedef vec<N, typename oln::internal::deduced_from_traits
      <oln::internal::operator_times_traits, T1, T2 >::ret_t> ret_t;
    };

    template <unsigned N,class T1,class T2>
    struct operator_div_traits<vec<N,T1>,T2 >
    {
      typedef vec<N, typename oln::internal::deduced_from_traits
      <oln::internal::operator_div_traits, T1, T2 >::ret_t> ret_t;
    };


    // other traits
    template<unsigned N,class T>
    struct value_traits<vec<N,T> > { typedef T ret_t[N]; };

    template<unsigned N,class T>
    struct real_value_traits<vec<N,T> >
    {
      typedef typename real_value_traits<T>::ret_t ret_t[N];
    };

    /* FIXME: the following 5 definitions are meaningless for
       vec.  IMHO rec_value should not require them. -- adl */

    template<unsigned N,class T>
    struct larger_traits<vec<N,T> >
    { typedef void ret_t;};

    template<unsigned N,class T>
    struct slarger_traits<vec<N,T> >
    { typedef void ret_t;};

    template<unsigned N,class T>
    struct signed_traits<vec<N,T> >
    { typedef void ret_t;};

    template<unsigned N,class T>
    struct unsigned_traits< vec<N,T> >
    { typedef void ret_t;};


    template<unsigned N,class T>
    struct cumul_traits<vec<N,T> >
    { typedef void ret_t;};


    template<unsigned N,class T>
    struct default_less<vec<N,T> >
    {
      bool operator()(const vec<N, T>& l,
                      const vec<N, T>& r) const
      {
        for (unsigned i = 0; i < N; ++i)
          if (l[i] < r[i])
            return true;
          else if (l[i] > r[i])
            return false;
        return false;
      }
    };

  }


  template <unsigned N, class T>
  class vec : public rec_value<vec<N,T> >
  {
  public :
    typedef T value_t[N];

    vec()
    {
      _fill (internal::zero_value_traits<T>::zero());
    }

    template<int ncols, class T2>
    vec(const oln::meta::array2d< oln::meta::array2d_info< 1, ncols >, T2 >& arr)
    {
      meta::eq< ncols, N >::ensure();
      for (unsigned i = 0; i < N; ++i)
	_value[i] = arr[i];
    }

    template<class U>
    vec(const vec<N, U>& v)
    {
      for (unsigned i = 0; i < N; ++i)
	_value[i] = v[i];
    }

    template<class U>
    vec<N, T>& operator=(const vec<N, U>& v)
    {
      for (unsigned i = 0; i < N; ++i)
	_value[i] = v[i];
      return *this;
    }

    // accessor
    T &	     operator[](unsigned i)	  { return _value[i]; }
    const T& operator[](unsigned i) const { return _value[i]; }

    // operators
    bool operator==(const vec<N,T>& rhs) const
    {
      for (unsigned i = 0; i < N; ++i)
	if (_value[i] != rhs[i])
	  return false;
      return true;
    }

    vec<N,T>& operator+=(const vec<N,T>& rhs) {
      for (unsigned i = 0; i < N; ++i)
	_value[i] += rhs[i];
      return *this;
    }

    vec<N,T>& operator-=(const vec<N,T>& rhs) {
      for (unsigned i = 0; i < N; ++i)
	_value[i] -= rhs[i];
      return *this;
    }

    template <class U>
    vec<N,T>& operator*=(const U& rhs) {
      for (unsigned i = 0; i < N; ++i)
	_value[i] *= rhs;
      return *this;
    }

    template <class U>
    vec<N,T>& operator/=(const U& rhs) {
      for (unsigned i = 0; i < N; ++i)
	_value[i] /= rhs;
      return *this;
    }

    static const vec<N,T> zero() { return vec(); }
    // There is no unit() for vec<>.
    ~vec() {}

    static string name() {
      std::ostringstream out;
      out << "vec<" << int(N) << ","
	  << internal::debug_type<T>::name() << ">" << std::ends;
      return out.str();
    }

  protected:
    vec& _fill(T t)
    {
      for (unsigned i = 0; i < N; ++i)
	_value[i] = t;
      return *this;
    }
  };

  /*
  point2d::operator vec<2,internal::oln_for_C<coord>::ret>() const
  {
    point2d::vec_t tmp;
    tmp[0] = this->row();
    tmp[1] = this->col();
    return tmp;
  }

  dpoint2d::operator vec<2,internal::oln_for_C<coord>::ret>() const
  {
    dpoint2d::vec_t tmp;
    tmp[0] = this->row();
    tmp[1] = this->col();
    return tmp;
  }
  */

} // end of oln



template<unsigned N,class T> inline
std::ostream& operator<<(std::ostream& ostr, const oln::vec<N,T>& rhs)
{
  ostr << "[";
  for (unsigned i = 0; i < N; ++i)
    ostr << rhs[i] << ((i < N-1) ? "," : "]");
  return ostr;
}



#endif // ! OLENA_VALUE_VEC_HH
