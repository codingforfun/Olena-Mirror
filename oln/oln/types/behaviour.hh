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

#ifndef OLENA_VALUE_BEHAVIOUR_HH
# define OLENA_VALUE_BEHAVIOUR_HH

# include <oln/core/contract.hh>
# include <oln/meta/basics.hh>
# include <oln/meta/type.hh>

# include <oln/types/rec_value.hh>
# include <oln/types/optraits.hh>
# include <oln/types/optraits_scalar.hh>
# include <oln/types/typetraits.hh>
# include <oln/types/to_oln.hh>

# include <oln/types/debug.hh>

# include <string>
# include <sstream>

//
//  Behaviours for data_types int_u, int_s, etc ...
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

namespace oln
{

  // unsafe : no check performed

  struct unsafe
  {
    template <class T>
    struct get
    {
      typedef typename typetraits<T>::storage_type storage_type;

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
      typedef typename typetraits<T>::storage_type storage_type;

      template <class T1, class T2>
      static T check_plus_equal (T1 lhs, T2 rhs)
      {	
	T ret = lhs + rhs;
	if (rhs > 0)
	  postcondition(TO_OLN_CAST(T, ret) > lhs);
	else
	  postcondition(TO_OLN_CAST(T, ret) <= lhs);
	return ret;
      }

      template <class T1, class T2>
      static T check_minus_equal (T1 lhs, T2 rhs)
      {	
	T ret = lhs - rhs;
	if (rhs > 0)
	  postcondition(TO_OLN_CAST(T, ret) < lhs);
	else
	  postcondition(TO_OLN_CAST(T, ret) >= lhs);
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
	typedef typename internal::to_oln<P>::ret p_oln_type;

	precondition(static_cast<p_oln_type>(p) <= optraits<T>::max());
	precondition(static_cast<p_oln_type>(p) >= optraits<T>::min());

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
      typedef typename typetraits<T>::storage_type storage_type;

      template <class T1, class T2>
      static T check_plus_equal (T1 lhs, T2 rhs)
      {	
	T ret = lhs + rhs;
	if (rhs > 0)
	  {
	    if (TO_OLN_CAST(T, ret) <= lhs)
	      ret = optraits<T>::max();
	  }
	else if (TO_OLN_CAST(T, ret) > lhs)
	  ret = optraits<T>::min();
	return ret;
      }

      template <class T1, class T2>
      static T check_minus_equal (T1 lhs, T2 rhs)
      {	
	T ret = lhs - rhs;
	if (rhs > 0)
	  {
	    if (TO_OLN_CAST(T, ret) > lhs)
	      ret = optraits<T>::min();
	  }
	else if (rhs != 0 && TO_OLN_CAST(T, ret) <= lhs)
	  ret = optraits<T>::max();
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
	      ret = optraits<T>::max();
	    else
	      ret = optraits<T>::min();
	  }
	return ret;	
      }

      template <class T1, class T2>
      static T check_div_equal (T1 lhs, T2 rhs)
      {	return lhs / rhs; }

      template <class P>
      static storage_type apply (const P& p)
      {
	typedef typename internal::to_oln<P>::ret p_oln_type;

	if (static_cast<p_oln_type>(p) > optraits<T>::max())
	  return optraits<T>::max();
	
	if (static_cast<p_oln_type>(p) < optraits<T>::min())
	  return optraits<T>::min();
	
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
      typedef typename typetraits<T>::storage_type storage_type;

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
	typedef typename meta::if_<is_a(optraits<P>, optraits_float)::ret,
	  cycle_fmod,
	  cycle_mod>::ret_t cycle_op;
	
	typename internal::to_oln<P>::ret
	  tmp = cycle_op::exec(std::abs(SIGNED_CAST(P, rhs)),
			       optraits<T>::max() - optraits<T>::min());
	
	if (rhs < 0) tmp = -tmp;
	
	if (tmp < optraits<T>::min())
	  return optraits<T>::max() - optraits<T>::min() + tmp;
	else if (tmp >= optraits<T>::max())
	  return optraits<T>::min() - optraits<T>::max() + tmp;
	
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

  }
    
} // end of namespace oln

#endif // ndef OLENA_VALUE_BEHAVIOUR_HH
