// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef NTG_UTILS_CAST_HH
# define NTG_UTILS_CAST_HH

# include <ntg/config/system.hh>
# include <ntg/core/macros.hh>
# include <ntg/core/predecls.hh>
# include <ntg/core/type_traits.hh>
# include <ntg/real/real_value.hh>

// FIXME: this file is completely broken, taking float_value as
// parameter does not make sense.

/*-------.
| macros |
`-------*/

# define TO_NTG_CAST(Dest)				\
  template<class T>					\
  inline ntg_##Dest##_type(T) to_##Dest##_ntg(T val)	\
  {							\
    return static_cast<ntg_##Dest##_type(T)>(val);	\
  }

namespace ntg {

  /*---------.
  | to_ntg() |
  `---------*/

  template<class T>
  inline ntg_type(T) to_ntg(T val)
  {
    return static_cast<ntg_type(T)>(val);
  }

  TO_NTG_CAST(signed)

  namespace cast {

    // FIXME: force does not seem to have any reason to live.

    /*------.
    | force |
    `------*/

    // a cast is performed instead of a constructor (unsafe one) call
    // because this last one may not be available with the good
    // signature.
    template<class Tdest, class Tsrc> inline
    const Tdest
    force(const Tsrc& val)
    {
      return ntg_unsafe_type(Tdest)(val);
    }

    /*------.
    | bound |
    `------*/

    // Like cast::force, but with saturation.
    template<class Tdest, class Tsrc> inline
    const Tdest
    bound(const Tsrc& val)
    {
      if (ntg_sup_val(Tsrc) > ntg_sup_val(Tdest))
	if (val > Tsrc(Tdest(ntg_sup_val(Tdest))))
	  return ntg_sup_val(Tdest);
      if (ntg_inf_val(Tsrc) < ntg_inf_val(Tdest))
	if (val < Tsrc(Tdest(ntg_inf_val(Tdest))))
	  return ntg_inf_val(Tdest);
      return val;
    }

    /*------.
    | round |
    `------*/

    namespace internal {
      template<class Tdest, class Tsrc>
      struct _round {
	// By default we don't define any function.
	// This cast does only work on float input.
      };

      template<class Tdest, class Tsrc>
      struct _round<Tdest, float_value<Tsrc> >
      {
	static const Tdest
	doit(const float_value<Tsrc>& val)
	{
	  // FIXME: this code seems out of date.

#if 1
	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.* complains there
	  // is no Tdest constructor taking a float argument.
	  return (ntg_storage_type(Tdest)) round(val.exact());
#endif
	  //	  return round(val.exact());
	}
      };

      template<class Tdest>
      struct _round<Tdest, float_s >
      {
	static const Tdest
	doit(const float_s& val)
	{
	  // FIXME: this code seems out of date.
#if 1
	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.* complains there
	  // is no Tdest constructor taking a float argument.
	  return (ntg_storage_type(Tdest)) roundf(val);
#endif
	  //	  return roundf(val);
	}
      };

      template<class Tdest>
      struct _round<Tdest, double>
      {
	static const Tdest
	doit(const float_d& val)
	{
	  // FIXME: this code seems out of date.
#if 0
	  // KLUDGE: Cast the rounded value to Tdest::value_t before
	  // returning it as Tdest. Otherwise g++-3.0 complains there
	  // is no Tdest constructor taking a float argument.
	  return (ntg_storage_type(Tdest)) round(val);
#endif
	  return round(val);
	}
      };

      template<class Tdest, class Tsrc>
      struct _round<float_value<Tdest>, float_value<Tsrc> >
      {
	static const Tdest
	doit(const float_value<Tsrc>& val)
	{
	  return val.exact();
	}
      };

      template<class Tsrc>
      struct _round<float_s, float_value<Tsrc> >
      {
	static float_s
	doit(const float_value<Tsrc>& val)
	{
	  return val.exact();
	}
      };

      template<class Tsrc>
      struct _round<float_d, float_value<Tsrc> >
      {
	static float_d
	doit(const float_value<Tsrc>& val)
	{
	  return val.exact();
	}
      };

      template<>
      struct _round<float_s, float_d>
      {
	static float
	doit(const double& val)
	{
	  return val;
	}
      };

      template<>
      struct _round<float_d, float_s>
      {
	static float_d
	doit(const float& val)
	{
	  return val;
	}
      };
    } // end of internal.

    template<class Tdest, class Tsrc> inline
    const Tdest round(const Tsrc& val)
    {
      return internal::_round<Tdest, Tsrc>::doit(val);
    }

    /* Like cast::round, but with saturation.  */
    template<class Tdest, class Tsrc> inline
    const Tdest rbound(const Tsrc& val)
    {
      if (val > Tsrc(Tdest(ntg_max_val(Tdest))))
	return ntg_max_val(Tdest);
      if (val < Tsrc(Tdest(ntg_min_val(Tdest))))
	return ntg_min_val(Tdest);
      return cast::round<Tdest>(val);
    }

  } // end of cast.

} // end of ntg.

#endif // NTG_UTILS_CAST_HH
