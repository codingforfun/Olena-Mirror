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
/// \brief Dilate 1-faces values to 0-faces.

/// FIXME: to be optimized. We could stop looking in neighborhod
/// values once the first True is found.

#ifndef MLN_WORLD_KN_DILATE_0_FROM_1_FACES_HH
# define MLN_WORLD_KN_DILATE_0_FROM_1_FACES_HH

# include <mln/core/concept/image.hh>
# include <mln/accu/logic/lor.hh>
# include <mln/world/kn/fill_0_from_1_faces.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      /// \brief Dilate 1-faces values to 0-faces.
      template <typename I>
      void dilate_0_from_1_faces(Image<I>& inout);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      void dilate_0_from_1_faces(Image<I>& inout_)
      {
	trace::entering("mln::world::kn::dilate_0_from_1_faces");

	mln_precondition(exact(inout_).is_valid());
	I& inout = exact(inout_);

	fill_0_from_1_faces(inout, accu::logic::lor());

	trace::exiting("mln::world::kn::dilate_0_from_1_faces");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_DILATE_0_FROM_1_FACES_HH
