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

#ifndef NTG_BEHAVIOUR_HH
# define NTG_BEHAVIOUR_HH

# include <mlc/contract.hh>
# include <mlc/bool.hh>
# include <mlc/is_a.hh>

# include <ntg/core/macros.hh>
# include <ntg/core/internal/macros.hh>
# include <ntg/core/value.hh>
# include <ntg/core/type_traits.hh>
# include <ntg/real/optraits_scalar.hh>
# include <ntg/real/real_value.hh>
# include <ntg/core/type_traits.hh>

# include <ntg/utils/debug.hh>

# include <string>
# include <sstream>

//
//  Behaviors for data_types int_u, int_s, etc ...
//
//  <WARNING>
//  Don't forget behaviour are checked on assignements and contruction of
//  types, and use comparison, so create only vars with unsafe bahaviour
//  in comparison operators
//
/////////////////////////////////////////////////////////////////////////

// FIXME : there is maybe simpler a way to write that, but we want it to
// be compatible with icc, so the behaviours must stay class, not
// template <class>

namespace ntg
{

  // unsafe : no check performed

  struct unsafe
  {
    template <class T>
    struct get
    {
      typedef ntgi_storage_type(T) storage_type;

      template <class T1, class T2>
      static T check_plus_equal (T1 lhs, T2 rhs)
      {	return lhs + rhs; }

      template <class T1, class T2>
      static T check_minus_equal (T1 lhs, T2 rhs)
      {	return lhs - rhs; }

      template <class T1, class T2>
      static T check_times_equal (T1 lhs, T2 rhs)
      {	return lhs * rhs; }

      template <class T1, class T2>
      static T check_div_equal (T1 lhs, T2 rhs)
      {	return lhs / rhs; }

      template <class P>
      static storage_type apply (const P& p)
      { return static_cast<storage_type>(p); }
    };

    // debug
    static std::string name() { return "unsafe"; }
  };


  // strict

  struct strict
  {
    template <class T>
    struct get
    {
      typedef ntgi_storage_type(T) storage_type;    

      template <class T1, class T2>
      static T check_plus_equal (T1 lhs, T2 rhs)
      {
	T ret = lhs + rhs;
	if (rhs > 0)
	  postcondition(ntg_cast(ret) > lhs);
	else
	  postcondition(ntg_cast(ret) <= lhs);
	return ret;
      }

      template <class T1, class T2>
      static T check_minus_equal (T1 lhs, T2 rhs)
      {
	T ret = lhs - rhs;
	if (rhs > 0)
	  postcondition(ntg_cast(ret) < lhs);
	else
	  postcondition(ntg_cast(ret) >= lhs);
	return ret;
      }

      // FIXME: this check is very slow ! find another solution
      template <class T1, class T2>
      static T check_times_equal (T1 lhs, T2 rhs)
      {
	T ret = lhs * rhs;
	if (rhs != 0)
	  postcondition ((ret / rhs) == lhs);
	return ret;
      }

      template <class T1, class T2>
      static T check_div_equal (T1 lhs, T2 rhs)
      {	return lhs / rhs; }

      template <class P>
      static storage_type apply (const P& p)
      {
	precondition(ntg_cast(p) <= ntg_max_val(T));
	precondition(ntg_cast(p) >= ntg_min_val(T));

	return static_cast<storage_type>(p);
      }
    };

    // debug
    static std::string name() { return "strict"; }
  };


  // saturate

  struct saturate
  {
    template <class T>
    struct get
    {
      typedef ntgi_storage_type(T) storage_type;

      template <class T1, class T2>
      static T check_plus_equal (T1 lhs, T2 rhs)
      {
	T ret = lhs + rhs;
	if (rhs > 0)
	  {
	    if (ntg_cast(ret) <= lhs)
	      ret = ntg_max_val(T);
	  }
	else if (ntg_cast(ret) > lhs)
	  ret = ntg_min_val(T);
	return ret;
      }

      template <class T1, class T2>
      static T check_minus_equal (T1 lhs, T2 rhs)
      {
	T ret = lhs - rhs;
	if (rhs > 0)
	  {
	    if (ntg_cast(ret) > lhs)
	      ret = ntg_min_val(T);
	  }
	else if (rhs != 0 && ntg_cast(ret) <= lhs)
	  ret = ntg_max_val(T);
	return ret;
      }

      // FIXME: this check is very slow ! find another solution ...
      template <class T1, class T2>
      static T check_times_equal (T1 lhs, T2 rhs)
      {
	T ret = lhs * rhs;
	if ((ret / rhs) != lhs)
	  {
	    // if lhs and rhs signs are equal, we wanted to grow
	    if ((lhs > 0 && rhs > 0) || (-lhs > 0 && -rhs > 0))
	      ret = ntg_max_val(T);
	    else
	      ret = ntg_min_val(T);
	  }
	return ret;
      }

      template <class T1, class T2>
      static T check_div_equal (T1 lhs, T2 rhs)
      {	return lhs / rhs; }

      template <class P>
      static storage_type apply (const P& p)
      {
	if (ntg_cast(p) > ntg_max_val(T))
	  return ntg_max_val(T);

	if (ntg_cast(p) < ntg_min_val(T))
	  return ntg_min_val(T);

	return static_cast<storage_type>(p);
      }
    };

    // debug
    static std::string name() { return "saturate"; }
  };

  //
  // This behaviour is not really useful, but implement cycle<> internal
  // calculus.
  //
  struct cycle_behaviour
  {
    template <class T>
    struct get
    {
      typedef ntgi_storage_type(T) storage_type;

      // FIXME: calculate real values

      template <class T1, class T2>
      static T check_plus_equal (T1 lhs, T2 rhs)
      {	return lhs + rhs; }

      template <class T1, class T2>
      static T check_minus_equal (T1 lhs, T2 rhs)
      {	return lhs - rhs; }

      template <class T1, class T2>
      static T check_times_equal (T1 lhs, T2 rhs)
      {	return lhs * rhs; }

      template <class T1, class T2>
      static T check_div_equal (T1 lhs, T2 rhs)
      {	return lhs / rhs; }

      // Assignation check

      struct cycle_fmod
      {
	static double exec (double lhs, double rhs)
	{ return fmod(lhs, rhs); }
      };

      struct cycle_mod
      {
	template <class T1, class T2>
	static T1 exec (const T1& lhs, const T2& rhs)
	{ return lhs % rhs; }
      };

      // FIXME: optimize ?
      template <class P>
      static storage_type apply (const P& rhs)
      {
	typedef typename mlc::if_<ntg_is_a(P, decimal)::ret,
	  cycle_fmod,
	  cycle_mod>::ret_t cycle_op;

	ntg_type(P) tmp = cycle_op::exec(std::abs(ntg_signed_cast(rhs)),
					 ntg_max_val(T) - ntg_min_val(T));

	if (rhs < 0) tmp = -tmp;

	if (tmp < ntg_min_val(T))
	  return ntg_max_val(T) - ntg_min_val(T) + tmp;
	else if (tmp >= ntg_max_val(T))
	  return ntg_min_val(T) - ntg_max_val(T) + tmp;

	return tmp;
      }
    };

    // debug
    static std::string name() { return "cycle_behaviour"; }
  };

  namespace internal {

    template <class B1, class B2>
    struct deduce_op_behaviour
    { typedef strict ret; };

    template <class B>
    struct deduce_op_behaviour<B, B>
    { typedef B ret; };

  } // end of internal.

} // end of ntg.

#endif // ndef NTG_BEHAVIOUR_HH
