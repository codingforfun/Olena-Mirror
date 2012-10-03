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

#ifndef MLN_VALUE_IOTA_HH
# define MLN_VALUE_IOTA_HH

/// \file
///
/// Define a functor returning the smallest next value in a given
/// type.

namespace mln
{

  namespace value
  {

    /// Define a functor returning the smallest next value in a
    /// given type.
    template <typename T>
    struct iota;

    /// Specialization of \ref mln::value::internal::iota for unsigned.
    template <>
    struct iota<unsigned>
    {
      static unsigned value();
    };

    /// Specialization of \ref mln::value::internal::iota for int.
    template <>
    struct iota<int>
    {
      static int value();
    };


# ifndef MLN_INCLUDE_ONLY

    unsigned
    iota<unsigned>::value()
    {
      return 1u;
    }

    int
    iota<int>::value()
    {
      return 1;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

#endif // MLN_VALUE_IOTA_HH
