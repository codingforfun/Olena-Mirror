// Copyright (C) 2007, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_MATH_MIN_HH
# define MLN_MATH_MIN_HH

/*! \file
 *
 * \brief Define min routine.
 */


namespace mln
{

  namespace math
  {

    /// Return the minimum of two values.
    template <typename T>
    T min(const T& v1, const T& v2);

    /// Return the minimum of four values.
    template <typename T>
    T min(const T& v1, const T& v2, const T& v3, const T& v4);

  } // end of namespace mln::math

# ifndef MLN_INCLUDE_ONLY

  /// \internal Generic implementation of the minimum function.
  template <typename T>
  T min_(const T& v1, const T& v2)
  {
    return v1 < v2 ? v1 : v2;
  }

  /// \internal Generic implementation of the minimum function.
  template <typename T>
  T min_(const T& v1, const T& v2, const T& v3, const T& v4)
  {
    return min_(min_(v1, v2), min_(v3, v4));
  }


  namespace math
  {

    template <typename T>
    T min(const T& v1, const T& v2)
    {
      return min_(exact(v1), exact(v2));
    }

    template <typename T>
    T min(const T& v1, const T& v2, const T& v3, const T& v4)
    {
      return min_(exact(v1), exact(v2), exact(v3), exact(v4));
    }

  } // end of namespace mln::math

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_MATH_MIN_HH
