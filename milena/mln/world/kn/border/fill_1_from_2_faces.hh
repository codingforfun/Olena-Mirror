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
/// \brief Duplicate 2 faces in KN 2D images borders.

#ifndef MLN_WORLD_KN_BORDER_FILL_1_FROM_2_FACES_HH
# define MLN_WORLD_KN_BORDER_FILL_1_FROM_2_FACES_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/world/kn/border/fill_1_from_aux_2_faces.hh>



namespace mln
{

  namespace world
  {

    namespace kn
    {

      namespace border
      {

      /*! \brief Compute 1 faces in Kn 2D images borders.

	\param[in,out] inout A 2D image immersed in KN.
	\param[in,out] f A function computing 1-faces values from 2
	2-faces values.

       */
      template <typename I, typename F>
      void fill_1_from_2_faces(const Image<I>& inout,
			       const Function_vv2v<F>& f);



# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename F>
      void fill_1_from_2_faces(const Image<I>& inout,
			       const Function_vv2v<F>& f)
      {
	trace::entering("mln::world::kn::fill_1_from_2_faces");

	mln_precondition(exact(inout).is_valid());
	mln_precondition(exact(inout).border() >= 1);

	border::fill_1_from_aux_2_faces(inout, inout, f);

	trace::exiting("mln::world::kn::fill_1_from_2_faces");
      }


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::world::kn::border

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_BORDER_FILL_1_FROM_2_FACES_HH
