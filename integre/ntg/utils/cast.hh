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

# include <ntg/core/macros.hh>
# include <ntg/core/internal/optraits_builtins.hh>
# include <ntg/core/type_traits.hh>
# include <ntg/core/internal/typetraits_builtins.hh>
# include <ntg/core/value.hh>

// FIXME: this file is completely broken, taking float_value as
// parameter does not make sense.

# define TO_NTG_CAST(Dest)				\
  template<class T>					\
  inline ntg_##Dest##_type(T) to_##Dest##_ntg(T val)	\
  {							\
    return static_cast<ntg_##Dest##_type(T)>(val);	\
  }

namespace ntg {

  template<class T>
  inline ntg_type(T) to_ntg(T val)
  {
    return static_cast<ntg_type(T)>(val);
  }
  
  TO_NTG_CAST(signed);

  namespace cast {    

    template<class Tdest, class Tsrc> inline
    const Tdest force(const Tsrc& val)
    {
      Tdest tmp(static_cast<ntg_storage_type(Tdest)>(val));
      return tmp;
    }

    /* Like cast::force, but with saturation.  */
    template<class Tdest, class Tsrc> inline
    const Tdest bound(const Tsrc& val)
    {
      if (ntg_max_val(Tsrc) > ntg_max_val(Tdest))
	if (val > Tsrc(ntg_max_val(Tdest)))
	  return ntg_max_val(Tdest);
      if (ntg_min_val(Tsrc) < ntg_min_val(Tdest))
	if (val < Tsrc(ntg_min_val(Tdest)))
	  return ntg_min_val(Tdest);
      return cast::force<Tdest>(val);
    }

    namespace internal {
      template<class Tdest, class Tsrc>
      struct _round {
	// By default we don't define any function.
	// This cast does only work on float input.
      };
      template<class Tdest, class Tsrc>
      struct _round<Tdest, float_value<Tsrc> > {
	static const Tdest doit(const float_value<Tsrc>& val)
	{
	  // FIXME: update comments

	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (ntg_storage_type(Tdest)) round(val.self());
	}
      };
      template<class Tdest>
      struct _round<Tdest, float_s > {
	static const Tdest doit(const float_s& val)
	{
	  // FIXME: update comments

	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (ntg_storage_type(Tdest)) roundf(val);
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
	  return (ntg_storage_type(Tdest)) round(val);
	}
      };
# if 0 // useless as float_s == float alias
      template<class Tdest>
      struct _round<Tdest, float> {
	static const Tdest doit(const float& val)
	{
	  // FIXME: update comments

	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (ntg_storage_type(Tdest)) roundf(val);
	}
      };
# endif
      template<class Tdest, class Tsrc>
      struct _round<float_value<Tdest>, float_value<Tsrc> > {
	static const Tdest doit(const float_value<Tsrc>& val)
	{
	  return val.self();
	}
      };
      template<class Tsrc>
      struct _round<float, float_value<Tsrc> > {
	static float doit(const float_value<Tsrc>& val)
	{
	  return val.self();
	}
      };
      template<class Tsrc>
      struct _round<double, float_value<Tsrc> > {
	static double doit(const float_value<Tsrc>& val)
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
      if (val > Tsrc(ntg_max_val(Tdest)))
	return ntg_max_val(Tdest);
      if (val < Tsrc(ntg_min_val(Tdest)))
	return ntg_min_val(Tdest);
      return cast::round<Tdest>(val);
    }

  } // end of cast.

} // end of ntg.

#endif // NTG_CAST_HH
