// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_MATH_MEDIAN_HH
# define MLN_MATH_MEDIAN_HH

# include <mln/trait/routine/median.hh>
# include <mln/math/mean.hh>
# include <mln/metal/converts_to.hh>

/*! \file
 *
 * \brief Define median routine.
 */


namespace mln
{

  namespace math
  {

    /// Return the median of two values.
    template <typename T>
    mln_trait_routine_median(T)
    median(const T& v1, const T& v2);

    /// \overload \pre Type U must be convertible towards type T.
    template <typename T, typename U>
    mln_trait_routine_median(T)
    median(const T& v1, const U& v2);

    /// Return the median of four values.
    template <typename T>
    mln_trait_routine_median(T)
    median(const T& v1, const T& v2, const T& v3, const T& v4);

    /// \overload \pre Type T2, T3 and T4 must be convertible towards
    /// type T.
    template <typename T, typename T2, typename T3, typename T4>
    mln_trait_routine_median(T)
    median(const T& v1, const T2& v2, const T3& v3, const T4& v4);


  } // end of namespace mln::math

# ifndef MLN_INCLUDE_ONLY

  /// \internal Generic implementation of the median of four values.
  template <typename T>
  mln_trait_routine_median(T)
  median_(const T& v1, const T& v2, const T& v3, const T& v4)
  {
    if (v1 <= v2)
      if (v2 <= v3)
	if (v3 <= v4)
	  return mean_(v2, v3);
	else // v3 > v4
	  if (v4 <= v1)
	    return mean_(v1,v2);
	  else // v4 > v1
	    return mean_(v4,v2);
      else // v2 > v3
	if (v1 <= v3)
	  if (v2 <= v4)
	    return mean_(v3, v2);
	  else // v2 > v4
	    if (v4 <= v1)
	      return mean_(v1, v3);
	    else // v4 > v1
	      return mean_(v4, v3);
	else // v1 > v3
	  if (v2 <= v4)
	    return mean_(v1,v2);
	  else // v2 > v4
	    if (v4 <= v3)
	      return mean_(v3,v1);
	    else // v4 > v3
	      return mean_(v4,v1);
    else // v1 > v2
      if (v1 <= v3)
	if (v4 <= v3)
	  if (v4 <= v2)
	    return mean_(v2, v1);
	  else // v4 > v2
	    return mean_(v1, v4);
	else // v4 > v2
	  return mean_(v1, v3);
      else // v1 > v3
	if (v2 <= v3)
	  if (v4 <= v1)
	    if (v4 <= v2)
	      return mean_(v2, v3);
	    else // v4 > v2
	      return mean_(v4, v3);
	  else // v4 > v1
	    return mean_(v3, v1);
	else // v2 > v3
	  if (v4 <= v1)
	    if (v3 <= v4)
	      return mean_(v4, v2);
	    else // v3 > v4
	      return mean_(v3, v2);
	  else // v4 > v1
	    return mean_(v2, v1);
  }

  namespace math
  {

    template <typename T>
    mln_trait_routine_median(T)
    median(const T& v1, const T& v2)
    {
      return mean_(exact(v1), exact(v2));
    }

    template <typename T, typename U>
    mln_trait_routine_median(T)
    median(const T& v1, const U& v2)
    {
      mlc_converts_to(U,T)::check();
      return mean_(v1, T(v2));
    }

    template <typename T>
    mln_trait_routine_median(T)
    median(const T& v1, const T& v2, const T& v3, const T& v4)
    {
      return median_(v1, T(v2), T(v3), T(v4));
    }

    template <typename T, typename T2, typename T3, typename T4>
    mln_trait_routine_median(T)
    median(const T& v1, const T2& v2, const T3& v3, const T4& v4)
    {
      mlc_converts_to(T2,T)::check();
      mlc_converts_to(T3,T)::check();
      mlc_converts_to(T4,T)::check();
      return median_(v1, T(v2), T(v3), T(v4));
    }

  } // end of namespace mln::math

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_MATH_MEDIAN_HH
