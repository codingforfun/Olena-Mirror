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

# include <oln/types/to_oln.hh>
# include <oln/types/rec_value.hh>
# include <oln/types/optraits.hh>
# include <oln/types/typetraits.hh>
# include <oln/types/to_oln.hh>

# include <string>
# include <sstream>

# define ABS(X) ((X) >= 0 ? (X) : (std::abs(X)))

//
//  Behaviours for data_types int_u, int_s, etc ...
// 
//  <WARNING>
//  Don't forget behaviour are checked on assignements and contruction of
//  types, and use comparison, so create only vars with unsafe bahaviour
//  in comparison operators
//
/////////////////////////////////////////////////////////////////////////

// FIXME : there is maybe simpler a way to write that, but i want it to 
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
	  precondition(OLN_TYPE(T, ret) > lhs);
	else
	  precondition(OLN_TYPE(T, ret) <= lhs);
	return ret;
      }

      template <class T1, class T2>
      static T check_minus_equal (T1 lhs, T2 rhs)
      {	
	T ret = lhs - rhs;
	if (rhs > 0)
	  precondition(OLN_TYPE(T, ret) <= lhs);
	else
	  precondition(OLN_TYPE(T, ret) > lhs);
	return ret;
      }

      template <class T1, class T2>
      static T check_times_equal (T1 lhs, T2 rhs)
      {		
	// FIXME: add checks
	return lhs * rhs;
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

      // FIXME: Calculate real values

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

      // FIXME: insert apply
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
