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

#ifndef NTG_CORE_BEHAVIOUR_HH
# define NTG_CORE_BEHAVIOUR_HH

/*
  Behaviors for data_types int_u, int_s, etc ...
  
  <WARNING> Don't forget that behaviors are checked on assignements
  and contruction of types, and use comparison, so create only vars
  with unsafe behaviors in comparison operators implementation.
*/

# include <mlc/bool.hh>
# include <mlc/contract.hh>
# include <mlc/is_a.hh>

# include <ntg/core/contract.hh>
# include <ntg/core/macros.hh>
# include <ntg/core/type_traits.hh>
# include <ntg/core/value.hh>
# include <ntg/core/internal/macros.hh>
# include <ntg/real/optraits_scalar.hh>
# include <ntg/real/real_value.hh>
# include <ntg/utils/debug.hh>

# include <string>
# include <sstream>


// FIXME: there is maybe simpler a way to write that, but we want it
// to be compatible with icc, so the behaviors must stay classes, not
// meta classes.

namespace ntg
{
  
  /*
    Behaviors work by callbacks. When an operator has some side effect
    to apply, it calls the corresponding behavior functions to wrap
    the new value to be assigned.

    Example in the constructor of int_u taking an unsigned in parameter:
    real_data_value = behavior_type<get<int_u> >::check(value_to_assign);
  */

  /*-------.
  | unsafe |
  `-------*/
  //! No check performed. 
  /*!
    Same behavior as the underlying builtin type.
  */
  struct unsafe
  {
    template <class T>
    struct get
    {
      typedef ntgi_storage_type(T) storage_type;

      template <class T1, class T2>
      static T
      check_plus_equal (T1 lhs, T2 rhs)
      {	return lhs + rhs; }

      template <class T1, class T2>
      static T
      check_minus_equal (T1 lhs, T2 rhs)
      {	return lhs - rhs; }

      template <class T1, class T2>
      static T
      check_times_equal (T1 lhs, T2 rhs)
      {	return lhs * rhs; }

      template <class T1, class T2>
      static T
      check_div_equal (T1 lhs, T2 rhs)
      {	return lhs / rhs; }

      template <class P>
      static storage_type
      check (const P& p)
      { return static_cast<storage_type>(p); }
    };

    static std::string
    name()
    { return "unsafe"; }
  };

  /*-------.
  | strict |
  `-------*/
  //! Strict checking, abort in there is a problem.
  struct strict
  {
    template <class T>
    struct get
    {
      typedef ntgi_storage_type(T) storage_type;

      // FIXME: These checks are not always useful. We can determine
      // in advance the types which can raise overflow
      // (eg. int_u<32>), the tests should be performed only in those
      // cases!

      // FIXME: check that conditions leading to empty code when
      // -DNDEBUG is defined does not have any runtime cost.

      template <class T1, class T2>
      static T
      check_plus_equal (T1 lhs, T2 rhs)
      {
	T ret = lhs + rhs;
	if (rhs > 0)
	  ntg_assert(ntg_cast(ret) > lhs);
	else
	  ntg_assert(ntg_cast(ret) <= lhs);
	return ret;
      }

      template <class T1, class T2>
      static T
      check_minus_equal (T1 lhs, T2 rhs)
      {
	T ret = lhs - rhs;
	if (rhs > 0)
	  ntg_assert(ntg_cast(ret) < lhs);
	else
	  ntg_assert(ntg_cast(ret) >= lhs);
	return ret;
      }

      // FIXME: this check is very slow! Find another solution.
      template <class T1, class T2>
      static T
      check_times_equal (T1 lhs, T2 rhs)
      {
	T ret = lhs * rhs;
	if (rhs != 0)
	  ntg_assert((ret / rhs) == lhs);
	return ret;
      }

      template <class T1, class T2>
      static T
      check_div_equal (T1 lhs, T2 rhs)
      {	return lhs / rhs; }

      template <class P>
      static storage_type
      check (const P& p)
      {
	ntg_assert(ntg_cast(p) <= ntg_max_val(T));
	ntg_assert(ntg_cast(p) >= ntg_min_val(T));

	return static_cast<storage_type>(p);
      }
    };

    static std::string
    name()
    { return "strict"; }
  };

  /*---------.
  | saturate |
  `---------*/
  //! Bound values to the nearest limit when an overflow occurs.
  struct saturate
  {
    template <class T>
    struct get
    {
      typedef ntgi_storage_type(T) storage_type;

      template <class T1, class T2>
      static T
      check_plus_equal (T1 lhs, T2 rhs)
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
      static T
      check_minus_equal (T1 lhs, T2 rhs)
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
      static T
      check_times_equal (T1 lhs, T2 rhs)
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
      static T
      check_div_equal (T1 lhs, T2 rhs)
      {	return lhs / rhs; }

      template <class P>
      static storage_type
      check (const P& p)
      {
	if (ntg_cast(p) > ntg_max_val(T))
	  return ntg_max_val(T);

	if (ntg_cast(p) < ntg_min_val(T))
	  return ntg_min_val(T);

	return static_cast<storage_type>(p);
      }
    };

    static std::string
    name()
    { return "saturate"; }
  };

  /*---------------.
  | cycle_behavior |
  `---------------*/
  //! Apply a modulus when an overflow occurs.
  /*! 
    This behavior is not really useful, but implement cycle<>
    internal calculus. You should note that a range<int_u, ...,
    cycle_behavior> is different than cycle<int_u, ...>. Refer to the
    documentation for more details.
  */
  struct cycle_behavior
  {
    template <class T>
    struct get
    {
      typedef ntgi_storage_type(T) storage_type;

      // FIXME: calculate real values!

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

      // float modulus
      struct cycle_fmod
      {
	static double
	exec(double lhs, double rhs)
	{ return fmod(lhs, rhs); }
      };

      // integer modulus
      struct cycle_mod
      {
	template <class T1, class T2>
	static T1 
	exec(const T1& lhs, const T2& rhs)
	{ return lhs % rhs; }
      };

      // FIXME: optimize!
      template <class P>
      static storage_type
      check (const P& rhs)
      {
	typedef typename mlc::if_<ntg_is_a(P, decimal)::ret,
	  cycle_fmod,
	  cycle_mod>::ret_t cycle_op;

	ntg_type(P) tmp = cycle_op::exec(std::abs(ntg_signed_cast(rhs)),
					 ntg_max_val(T) - ntg_min_val(T));

	if (to_ntg(rhs) < 0)
	  tmp = -tmp;

	if (tmp < ntg_min_val(T))
	  return ntg_max_val(T) - ntg_min_val(T) + tmp;
	else if (tmp >= ntg_max_val(T))
	  return ntg_min_val(T) - ntg_max_val(T) + tmp;

	return tmp;
      }
    };

    static std::string
    name() { return "cycle_behavior"; }
  };

  namespace internal {

    /*-------------------.
    | deduce_op_behavior |
    `-------------------*/

    //! Determine the resulting behavior of an operator return type.
    /*! 
      The algorithm is quite simple and arbitrary, is the two
      behaviors are identicals, then use it for the return type. Else
      use a strict behavior.
    */
    template <class B1, class B2>
    struct deduce_op_behavior
    { typedef strict ret; };

    template <class B>
    struct deduce_op_behavior<B, B>
    { typedef B ret; };

  } // end of internal.

} // end of ntg.

#endif // !NTG_CORE_BEHAVIOUR_HH
