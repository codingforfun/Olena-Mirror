// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

# include <ntg/config/system.hh>

# include <mlc/cmp.hh>

# include <ntg/behaviour.hh>
# include <ntg/cast.hh>
# include <ntg/global_ops.hh>
# include <ntg/int_u.hh>
# include <ntg/optraits_vec.hh>
# include <ntg/predecls.hh>
# include <ntg/rec_value.hh>
# include <ntg/typetraits.hh>
# include <ntg/vec.hh>

# include <iostream>
# include <sstream>
# include <string>


namespace oln {

  template <unsigned ncomps, unsigned qbits, template <unsigned>
  class color_system>
  struct typetraits<color<ncomps, qbits, color_system> >
  {
    typedef color<ncomps, qbits, color_system>	self;
    typedef self				base_type;
    typedef vec<ncomps, int_u<qbits> >		storage_type;
  };
  
  namespace internal
  {

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
	float in_range = float(optraits<T>::max()) - float(optraits<T>::min());
	float out_range = float(color_system<n>::upper_bound)
	  - float(color_system<n>::lower_bound);
	out[n] = ((float(in[n]) - float(optraits<T>::min()))
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
      typedef int_u<qbits>		T;
      typedef vec<ncomps, float>	in_type;
      typedef vec<ncomps, T>		out_type;

      static void
      doit (const in_type& in, out_type& out)
      {
	float out_range = float(optraits<T>::max()) 
	  - float(optraits<T>::min());
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


  namespace type_definitions
  {

    template <unsigned ncomps, unsigned qbits, template <unsigned>
    class color_system>
    struct color : public rec_value<color<ncomps, qbits, color_system> >
    {
      template<unsigned icomp>
      struct lower_bound { enum { ret = color_system<icomp>::lower_bound }; };
      template<unsigned icomp>
      struct upper_bound { enum { ret = color_system<icomp>::upper_bound }; };

      typedef int_u<qbits> comp_type;
      typedef vec<ncomps, comp_type > vec_type;
      typedef vec<ncomps, float > float_vec_type;

      color() {};
      color(const vec_type& vec) { _value = vec; };
      color(const float_vec_type& vec)
      {
	internal::_from_float<0,ncomps,qbits,color_system>::doit(vec,_value);
      }

      color(const comp_type& c1, const comp_type& c2, const comp_type& c3)
      {
	meta::eq<ncomps, 3>::ensure();
	_value[0] = c1;
	_value[1] = c2;
	_value[2] = c3;
      }

      comp_type&       operator[](unsigned i)	    { return _value[i]; }
      const comp_type  operator[](unsigned i) const { return _value[i]; }

      vec_type&       as_vec()       { return _value; }
      const vec_type& as_vec() const { return _value; }

      float_vec_type to_float() const
      {
	float_vec_type tmp;
	internal::_to_float<0,ncomps,qbits,color_system>::doit(_value, tmp);
	return tmp;
      }

      bool operator==(const color& r) const { return _value == r._value; }

      static std::string name() {
	std::ostringstream out;
	// FIXME: Output color_system somehow.
	out << "color<" << ncomps << "," << qbits <<  ",...>" << std::ends;
	return out.str();
      }
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

  } // type_definitions

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
