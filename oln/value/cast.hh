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

#ifndef OLENA_VALUE_CAST_HH
# define OLENA_VALUE_CAST_HH

# include <oln/value/rec_value.hh>

namespace oln {
  namespace cast {

    template<class Tdest, class Tsrc> inline
    const Tdest force(const Tsrc& val)
    {
      Tdest tmp(static_cast<typename Tdest::real_value_t>(real_value(val)));
      return tmp;
    }

    /* Like cast::force, but with saturation.  */
    template<class Tdest, class Tsrc> inline
    const Tdest bound(const Tsrc& val)
    {
      if (val > Tsrc(Tdest::max()))
	return Tdest::max();
      if (val < Tsrc(Tdest::min()))
	return Tdest::min();
      return cast::force<Tdest>(val);
    }

    namespace internal {
      template<class Tdest, class Tsrc>
      struct _round {
	// By default we don't define any function.
	// This cast does only work on float input.
      };
      template<class Tdest, class Tsrc>
      struct _round<Tdest, rec_float<Tsrc> > {
	static const Tdest doit(const rec_float<Tsrc>& val)
	{
	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (typename Tdest::value_t) round(val.self());
	}
      };
      template<class Tdest>
      struct _round<Tdest, sfloat > {
	static const Tdest doit(const sfloat& val)
	{
	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (typename Tdest::value_t) roundf(val);
	}
      };
      template<class Tdest>
      struct _round<Tdest, double> {
	static const Tdest doit(const double& val)
	{
	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (typename Tdest::value_t) round(val);
	}
      };
      template<class Tdest>
      struct _round<Tdest, float> {
	static const Tdest doit(const float& val)
	{
	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (typename Tdest::value_t) roundf(val);
	}
      };
      template<class Tdest, class Tsrc>
      struct _round<rec_float<Tdest>, rec_float<Tsrc> > {
	static const Tdest doit(const rec_float<Tsrc>& val)
	{
	  return val.self();
	}
      };
      template<class Tsrc>
      struct _round<float, rec_float<Tsrc> > {
	static const float doit(const rec_float<Tsrc>& val)
	{
	  return val.self();
	}
      };
      template<class Tsrc>
      struct _round<double, rec_float<Tsrc> > {
	static const double doit(const rec_float<Tsrc>& val)
	{
	  return val.self();
	}
      };
      template<>
      struct _round<float, double> {
	static const float doit(const double& val)
	{
	  return val;
	}
      };
      template<>
      struct _round<double, float> {
	static const double doit(const float& val)
	{
	  return val;
	}
      };
    } // internal

    template<class Tdest, class Tsrc> inline
    const Tdest round(const Tsrc& val) {
      return internal::_round<Tdest,Tsrc>::doit(val);
    }

    /* Like cast::round, but with saturation.  */
    template<class Tdest, class Tsrc> inline
    const Tdest rbound(const Tsrc& val)
    {
      if (val > Tsrc(Tdest::max()))
	return Tdest::max();
      if (val < Tsrc(Tdest::min()))
	return Tdest::min();
      return cast::round<Tdest>(val);
    }

  } // cast
} // oln

#endif // OLENA_VALUE_CAST_HH
