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

#ifndef MLN_MATH_FLOOR_HH
# define MLN_MATH_FLOOR_HH

/*! \file
 *
 * \brief Define floor routine.
 */

# include <cmath>

namespace mln
{

  namespace math
  {

    /// \brief Return the nearest integer not greater than \p v1.
    template <typename T>
    T floor(const T& v1);


  } // end of namespace mln::math

# ifndef MLN_INCLUDE_ONLY

  /// \internal Generic implementation of the floor function.
  template <typename T>
  T floor_(const T& v1)
  {
    return std::floor(v1);
  }

  namespace math
  {

    template <typename T>
    T floor(const T& v1)
    {
      return floor_(exact(v1));
    }

  } // end of namespace mln::math

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_MATH_FLOOR_HH
