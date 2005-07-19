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

#ifndef NTG_REAL_BEHAVIOUR2_HH
# define NTG_REAL_BEHAVIOUR2_HH

# include <ntg/real/behavior.hh>
# include <ntg/core/internal/global_ops.hh>


// Method definitions.
// ###################


namespace ntg
{

  // unsafe::get<T>::

  template <class T>
  template <class T1, class T2>
  T unsafe::get<T>::check_plus (T1 lhs, T2 rhs)
  {
    return lhs + rhs;
  }

  template <class T>
  template <class T1, class T2>
  T unsafe::get<T>::check_minus (T1 lhs, T2 rhs)
  {
    return lhs - rhs;
  }

  template <class T>
  template <class T1, class T2>
  T unsafe::get<T>::check_times (T1 lhs, T2 rhs)
  {
    return lhs * rhs;
  }

  template <class T>
  template <class T1, class T2>
  T unsafe::get<T>::check_div (T1 lhs, T2 rhs)
  {
    return lhs / rhs;
  }

  template <class T>
  template <class P>
  ntgi_storage_type(T)
  unsafe::get<T>::check (const P& p)
  {
    return ntgi_storage_type(T)(p);
  }

  // force::get<T>::

  template<class T>
  template <class T1, class T2>
  T force::get<T>::check_plus (T1 lhs, T2 rhs)
  {
    return cast::force<T>(lhs + rhs);
  }

  template<class T>
  template <class T1, class T2>
  T force::get<T>::check_minus (T1 lhs, T2 rhs)
  {
    return cast::force<T>(lhs - rhs);
  }
  
  template<class T>
  template <class T1, class T2>
  T force::get<T>::check_times (T1 lhs, T2 rhs)
  {
    return cast::force<T>(lhs * rhs);
  }
  
  template<class T>
  template <class T1, class T2>
  T force::get<T>::check_div (T1 lhs, T2 rhs)
  {
    return cast::force<T>(lhs / rhs);
  }
  
  template<class T>
  template <class P>
  ntgi_storage_type(T)
  force::get<T>::check (const P& p)
  {
    return cast::force<T>(p);
  }

  // strict::get<T>::

  template<class T>
  template <class T1, class T2>
  T strict::get<T>::check_plus (T1 lhs, T2 rhs)
  {
    T ret = lhs + rhs;
    if (rhs > 0)
      ntg_assert(ntg_cast(ret) > lhs);
    else
      ntg_assert(ntg_cast(ret) <= lhs);
    return ret;
  }

  template<class T>
  template <class T1, class T2>
  T strict::get<T>::check_minus (T1 lhs, T2 rhs)
  {
    T ret = lhs - rhs;
    if (rhs > 0)
      ntg_assert(ntg_cast(ret) < lhs);
    else
      ntg_assert(ntg_cast(ret) >= lhs);
    return ret;
  }

  template<class T>
  template <class T1, class T2>
  T strict::get<T>::check_times (T1 lhs, T2 rhs)
  {
    T ret = lhs * rhs;
    if (rhs != 0)
      ntg_assert((ret / rhs) == lhs);
    return ret;
  }

  template<class T>
  template <class T1, class T2>
  T strict::get<T>::check_div (T1 lhs, T2 rhs)
  {
    return lhs / rhs;
  }
  
  template<class T>
  template <class P>
  ntgi_storage_type(T)
  strict::get<T>::check (const P& p)
  {
    ntg_assert(ntg_cast(p) <= ntg_max_val(T));
    ntg_assert(ntg_cast(p) >= ntg_min_val(T));
    return static_cast<ntgi_storage_type(T)>(p);
  }

  // saturate::get<T>::

  template<class T>
  template <class T1, class T2>
  T saturate::get<T>::check_plus (T1 lhs, T2 rhs)
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
  
  template<class T>
  template <class T1, class T2>
  T saturate::get<T>::check_minus (T1 lhs, T2 rhs)
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

  template<class T>
  template <class T1, class T2>
  T saturate::get<T>::check_times (T1 lhs, T2 rhs)
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
  
  template<class T>
  template <class T1, class T2>
  T saturate::get<T>::check_div (T1 lhs, T2 rhs)
  {
    return lhs / rhs;
  }

  template<class T>
  template <class P>
  ntgi_storage_type(T)
    saturate::get<T>::check (const P& p)
  {
    if (ntg_cast(p) > ntg_max_val(T))
      return ntg_max_val(T);
    if (ntg_cast(p) < ntg_min_val(T))
      return ntg_min_val(T);
    return static_cast<ntgi_storage_type(T)>(p);
  }

  // cycle_behavior::get<T>::

  template<class T>
  template <class T1, class T2>
  T cycle_behavior::get<T>::check_plus (T1 lhs, T2 rhs)
  {
    return lhs + rhs;
  }

  template<class T>
  template <class T1, class T2>
  T cycle_behavior::get<T>::check_minus (T1 lhs, T2 rhs)
  {
    return lhs - rhs;
  }

  template<class T>
  template <class T1, class T2>
  T cycle_behavior::get<T>::check_times (T1 lhs, T2 rhs)
  {
    return lhs * rhs;
  }

  template<class T>
  template <class T1, class T2>
  T cycle_behavior::get<T>::check_div (T1 lhs, T2 rhs)
  {
    return lhs / rhs;
  }

  template<class T>
  struct cycle_behavior::get<T>::cycle_fmod
  {
    static double
    exec(double lhs, double rhs)
    { return fmod(lhs, rhs); }
  };

  template<class T>
  struct cycle_behavior::get<T>::cycle_mod
  {
    template <class T1, class T2>
    static T1
    exec(const T1& lhs, const T2& rhs)
    { return lhs % rhs; }
  };

  template<class T>
  template <class P>
  ntgi_storage_type(T)
  cycle_behavior::get<T>::check (const P& rhs)
  {
    typedef typename mlc::if_< ntg_is_a(P, decimal),
      cycle_fmod,
      cycle_mod>::ret cycle_op;
    
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


} // end of ntg.


#endif // !NTG_REAL_BEHAVIOUR2_HH
