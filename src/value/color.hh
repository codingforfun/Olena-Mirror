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

#ifndef OLENA_VALUE_COLOR_HH
# define OLENA_VALUE_COLOR_HH

# include <iostream>
# include "value/predefs.hh"
# include "value/vec.hh"


namespace oln {

  namespace internal {

    template <unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct value_traits<color<ncomps, qbits, color_system> >
    { typedef vec<ncomps, int_u<qbits> > ret_t; };

    template <unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct real_value_traits<color<ncomps, qbits, color_system> >
      : public value_traits<vec<ncomps, int_u<qbits> > > {};

    /* FIXME: the following 4 definitions are meaningless for
       color types.  IMHO rec_value should not require them. -- adl */
    template <unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct larger_traits<color<ncomps, qbits, color_system> >
    { typedef void ret_t;};

    template <unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct signed_traits<color<ncomps, qbits, color_system> >
    { typedef void ret_t;};

    template <unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct unsigned_traits<color<ncomps, qbits, color_system> >
    { typedef void ret_t;};

    template <unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct cumul_traits<color<ncomps, qbits, color_system> >
    { typedef void ret_t;};


    // Helper struct to convert vec<N,T> to vec<N,float>,
    // taking color_system into account.
    template <unsigned n, unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct _to_float
    {
      typedef int_u<qbits> T;
      typedef vec<ncomps, T >      in_type;
      typedef vec<ncomps, float > out_type;

      static void
      doit (const in_type& in, out_type& out)
      {
	float in_range = float(T::max()) - float(T::min());
	float out_range = float(color_system<n>::upper_bound)
	  - float(color_system<n>::lower_bound);
	out[n] = ((float(in[n]) - float(T::min()))
		  * out_range / in_range
		  + float(color_system<n>::lower_bound));

	// process next componant recursively:
	_to_float<n + 1, ncomps, qbits, color_system>::doit(in, out);
      }
    };

    // stop recursion when n == ncomps.
    template <unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct _to_float<ncomps, ncomps, qbits, color_system>
    {
      typedef vec<ncomps, int_u<qbits> > in_type;
      typedef vec<ncomps, float >	out_type;

      static void
      doit (const in_type&, out_type&)
      {
      }
    };


    // Helper struct to convert vec<N,float> to vec<N,T>,
    // taking color_system into account.
    template <unsigned n, unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct _from_float
    {
      typedef int_u<qbits> T;
      typedef vec<ncomps, float >  in_type;
      typedef vec<ncomps, T >     out_type;

      static void
      doit (const in_type& in, out_type& out)
      {
	float out_range = float(T::max()) - float(T::min());
	float in_range = float(color_system<n>::upper_bound)
	  - float(color_system<n>::lower_bound);

	out[n] = cast::rbound<int_u<qbits> >
	  ((in[n] - float(color_system<n>::lower_bound))
	   * out_range / in_range
	   + float(color_system<n>::lower_bound));

	// process next componant recursively:
	_from_float<n + 1, ncomps, qbits, color_system>::doit(in, out);
      }
    };

    // stop recursion when n == ncomps.
    template <unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct _from_float<ncomps, ncomps, qbits, color_system>
    {
      typedef vec<ncomps, float >	  in_type;
      typedef vec<ncomps, int_u<qbits> > out_type;

      static void
      doit (const in_type&, out_type&)
      {
      }
    };

  } // internal



  template <unsigned ncomps, unsigned qbits, template <unsigned>
  class color_system>
  struct color
  {
    template<unsigned icomp>
    struct lower_bound { enum { ret = color_system<icomp>::lower_bound }; };
    template<unsigned icomp>
    struct upper_bound { enum { ret = color_system<icomp>::upper_bound }; };

    typedef int_u<qbits> comp_type;
    typedef vec<ncomps, comp_type > vec_type;
    typedef vec<ncomps, float > float_vec_type;

    color(): _value() {};
    color(const vec_type& vec) : _value(vec) {};
    color(const float_vec_type& vec) : _value()
    {
      internal::_from_float<0,ncomps,qbits,color_system>::doit(vec,_value);
    }

    comp_type&       operator[](unsigned i)	    { return _value[i]; }
    const comp_type& operator[](unsigned i) const { return _value[i]; }

    vec_type&       as_vec()       { return _value; }
    const vec_type& as_vec() const { return _value; }

    float_vec_type to_float() const
    {
      float_vec_type tmp;
      internal::_to_float<0,ncomps,qbits,color_system>::doit(_value, tmp);
      return tmp;
    }

    bool operator==(const color& r) const { return _value == r._value; }

    static string name() {
      std::ostringstream out;
      // FIXME: Output color_system somehow.
      out << "color<" << ncomps << "," << qbits <<  ",...>" << std::ends;
      return out.str();
    }
  protected:
    vec_type _value;
  };


  // Helper function to complete color_system (by inheritance).
  template<int lval, int uval>
  struct interval
  {
    enum {
      lower_bound = lval,
      upper_bound = uval
    };
  };

} // oln

template <unsigned ncomps, unsigned qbits, template <unsigned>
class color_system> inline
std::ostream& operator<<(std::ostream& o,
			 const oln::color<ncomps, qbits, color_system>& r)
{
  o << r.as_vec();
  return o;
}

#endif // OLENA_VALUE_COLOR_HH
