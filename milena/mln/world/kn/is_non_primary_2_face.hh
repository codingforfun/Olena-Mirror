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
/// \brief Check if site is a non-primary face.

#ifndef MLN_WORLD_KN_IS_NON_PRIMARY_2_FACE_HH
# define MLN_WORLD_KN_IS_NON_PRIMARY_2_FACE_HH

# include <mln/core/alias/point2d.hh>
# include <mln/world/kn/is_2_face.hh>
# include <mln/world/kn/is_primary_2_face.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {

      /// \brief Check if site is a non-primary face
      bool is_non_primary_2_face(const point2d& p, unsigned n);


      /// \overload
      bool is_non_primary_2_face(const mln::def::coord& row,
				 const mln::def::coord& col,
				 unsigned n);

      /// \overload
      /// Static version. \tparam n is a template parameter which
      /// allows some computation to be performed at compile time.
      template <unsigned n>
      bool is_non_primary_2_face(const mln::def::coord& row,
				 const mln::def::coord& col);

# ifndef MLN_INCLUDE_ONLY


      // Facade

      inline
      bool is_non_primary_2_face(const point2d& p, unsigned n)
      {
	return is_non_primary_2_face(p.row(), p.col(), n);
      }

      inline
      bool is_non_primary_2_face(const mln::def::coord& row,
				 const mln::def::coord& col,
				 unsigned n)
      {
	return is_2_face(row, col) && !is_primary_2_face(row, col, n);
      }


      template <unsigned n>
      bool is_non_primary_2_face(const mln::def::coord& row,
				 const mln::def::coord& col)
      {
	return is_2_face(row, col) && !is_primary_2_face<n>(row, col);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_IS_NON_PRIMARY_2_FACE_HH
