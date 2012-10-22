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
/// \brief Check if site is a primary face.

#ifndef MLN_WORLD_K1_IS_PRIMARY_FACE_HH
# define MLN_WORLD_K1_IS_PRIMARY_FACE_HH

# include <mln/core/alias/point2d.hh>
# include <mln/world/kn/is_2_face.hh>


namespace mln
{

  namespace world
  {

    namespace k1
    {

      /// \brief Check if site is a primary face
      bool is_primary_face(const point2d& p);


      /// \overload
      bool is_primary_face(const mln::def::coord& row,
			   const mln::def::coord& col);


# ifndef MLN_INCLUDE_ONLY


      // Facade

      inline
      bool is_primary_face(const point2d& p)
      {
	return kn::is_2_face(p);
      }

      inline
      bool is_primary_face(const mln::def::coord& row,
			   const mln::def::coord& col)
      {
	return kn::is_2_face(row, col);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k1

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K1_IS_PRIMARY_FACE_HH
