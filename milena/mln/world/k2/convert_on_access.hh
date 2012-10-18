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

/// \file
///
/// \brief Fill 1 faces in a K2 2D image using its 2 faces.

#ifndef MLN_WORLD_K2_CONVERT_ON_ACCESS_HH
# define MLN_WORLD_K2_CONVERT_ON_ACCESS_HH

# include <mln/core/alias/point2d.hh>
# include <mln/world/k2/is_non_primary_face_vertical.hh>
# include <mln/world/k2/is_non_primary_face_horizontal.hh>
# include <mln/world/k2/convert_on_access.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/intsub.hh>

namespace mln
{

  namespace world
  {

    namespace k2
    {

      using namespace mln::value;

      template <unsigned n>
      void convert_on_access(const intsub<n>& from, intsub<n/2>& to);

      template <unsigned n>
      void convert_on_access(const int_u8& from, intsub<n>& to);

      template <unsigned n>
      void convert_on_access(const int_u8& from, interval<intsub<n> >& to);

      template <unsigned n>
      void convert_on_access(const interval<intsub<n> >& from, int& to);

      template <unsigned n>
      void convert_on_access(const interval<intsub<n> >& from, value::int_u8& to);

      template <typename T, typename U>
      void convert_on_access(const T& from, U& to);


#ifndef MLN_INCLUDE_ONLY

      template <unsigned n>
      void convert_on_access(const intsub<n/2>& from, intsub<n>& to)
      {
	to = from.to_int();
      }

      template <unsigned n>
      void convert_on_access(const int_u8& from, intsub<n>& to)
      {
	to = from.to_equiv();
      }

      template <unsigned n>
      void convert_on_access(const int_u8& from, interval<intsub<n> >& to)
      {
	to = intsub<n>(from.to_interop());
      }

      template <unsigned n>
      void convert_on_access(const interval<intsub<n> >& from, int& to)
      {
	mln_precondition(from.is_degenerated());
	to = from.first();
      }

      template <unsigned n>
      void convert_on_access(const interval<intsub<n> >& from, value::int_u8& to)
      {
	mln_precondition(from.is_degenerated());
	to = from.first();
      }

      template <unsigned n>
      void convert_on_access(const interval<intsub<n> >& from, intsub<n/2>& to)
      {
	mln_precondition(from.is_degenerated());
	to = from.first().to_interop();
      }

      template <typename T, typename U>
      void convert_on_access(const T& from, U& to)
      {
	to = static_cast<U>(from);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k2

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K2_CONVERT_ON_ACCESS_HH
