// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef NTG_CAST_HH
# define NTG_CAST_HH

# include <ntg/core/value.hh>
# include <ntg/core/typetraits.hh>
# include <ntg/core/typetraits_builtins.hh>
# include <ntg/core/optraits.hh>
# include <ntg/core/optraits_builtins.hh>

namespace ntg 
{

  namespace cast 
  {

    template<class Tdest, class Tsrc> inline
    const Tdest force(const Tsrc& val)
    {
      Tdest tmp(static_cast<typename typetraits<Tdest>::storage_type>(val));
      return tmp;
    }

    /* Like cast::force, but with saturation.  */
    template<class Tdest, class Tsrc> inline
    const Tdest bound(const Tsrc& val)
    {
      if (optraits<Tsrc>::max() > optraits<Tdest>::max())
	if (val > Tsrc(optraits<Tdest>::max()))
	  return optraits<Tdest>::max();
      if (optraits<Tsrc>::min() < optraits<Tdest>::min())
	if (val < Tsrc(optraits<Tdest>::min()))
	  return optraits<Tdest>::min();
      return cast::force<Tdest>(val);
    }

    namespace internal {
      template<class Tdest, class Tsrc>
      struct _round {
	// By default we don't define any function.
	// This cast does only work on float input.
      };
      template<class Tdest, class Tsrc>
      struct _round<Tdest, decimal<Tsrc> > {
	static const Tdest doit(const decimal<Tsrc>& val)
	{
	  // FIXME: update comments

	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (typename typetraits<Tdest>::storage_type) round(val.self());
	}
      };
      template<class Tdest>
      struct _round<Tdest, sfloat > {
	static const Tdest doit(const sfloat& val)
	{
	  // FIXME: update comments

	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (typename typetraits<Tdest>::storage_type) roundf(val);
	}
      };
      template<class Tdest>
      struct _round<Tdest, double> {
	static const Tdest doit(const double& val)
	{
	  // FIXME: update comments

	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (typename typetraits<Tdest>::storage_type) round(val);
	}
      };
# if 0 // useless as sfloat == float alias
      template<class Tdest>
      struct _round<Tdest, float> {
	static const Tdest doit(const float& val)
	{
	  // FIXME: update comments

	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (typename typetraits<Tdest>::storage_type) roundf(val);
	}
      };
# endif
      template<class Tdest, class Tsrc>
      struct _round<decimal<Tdest>, decimal<Tsrc> > {
	static const Tdest doit(const decimal<Tsrc>& val)
	{
	  return val.self();
	}
      };
      template<class Tsrc>
      struct _round<float, decimal<Tsrc> > {
	static float doit(const decimal<Tsrc>& val)
	{
	  return val.self();
	}
      };
      template<class Tsrc>
      struct _round<double, decimal<Tsrc> > {
	static double doit(const decimal<Tsrc>& val)
	{
	  return val.self();
	}
      };
      template<>
      struct _round<float, double> {
	static float doit(const double& val)
	{
	  return val;
	}
      };
      template<>
      struct _round<double, float> {
	static double doit(const float& val)
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
      if (val > Tsrc(optraits<Tdest>::max()))
	return optraits<Tdest>::max();
      if (val < Tsrc(optraits<Tdest>::min()))
	return optraits<Tdest>::min();
      return cast::round<Tdest>(val);
    }

  } // end of cast
} // end of ntg

#endif // NTG_CAST_HH
