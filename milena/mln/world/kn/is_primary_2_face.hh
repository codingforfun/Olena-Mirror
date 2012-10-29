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
/// \brief Check if site is a primary face in an image immersed in Kn.

#ifndef MLN_WORLD_KN_IS_PRIMARY_2_FACE_HH
# define MLN_WORLD_KN_IS_PRIMARY_2_FACE_HH

# include <cmath>
# include <mln/core/alias/point2d.hh>
# include <mln/metal/math/pow.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      /// \brief Check if site is a primary face in an image immersed
      /// in K\p n.
      bool is_primary_2_face(const point2d& p,
			     unsigned n);


      /// \overload
      bool is_primary_2_face(const mln::def::coord& row,
			     const mln::def::coord& col,
			     unsigned n);

      /// \overload
      /// Static version. \tparam n is a template parameter which
      /// allows some computation to be performed at compile time.
      template <unsigned n>
      bool is_primary_2_face(const mln::def::coord& row,
			     const mln::def::coord& col);

# ifndef MLN_INCLUDE_ONLY


      inline
      bool is_primary_2_face(const point2d& p, unsigned n)
      {
	return is_primary_2_face(p.row(), p.col(), n);
      }

      inline
      bool is_primary_2_face(const mln::def::coord& row,
			     const mln::def::coord& col,
			     unsigned n)
      {
	long shift = std::pow(2u,n);
	return (row % shift) == 0
	  && (col % shift) == 0;
      }

      template <unsigned n>
      bool is_primary_2_face(const mln::def::coord& row,
			     const mln::def::coord& col)
      {
	return (row % mlc_pow_int(2, n)) == 0
	  && (col % mlc_pow_int(2, n)) == 0;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_IS_PRIMARY_2_FACE_HH
