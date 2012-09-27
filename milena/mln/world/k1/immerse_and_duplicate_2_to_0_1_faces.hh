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
/// \brief Immerse a 2D image into K1 and copy 2 faces data into 0 and
/// 1 faces.

#ifndef MLN_WORLD_K1_IMMERSE_AND_DUPLICATE_2_TO_0_1_FACES_HH
# define MLN_WORLD_K1_IMMERSE_AND_DUPLICATE_2_TO_0_1_FACES_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/box.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/world/k1/immerse.hh>
# include <mln/world/k1/is_1_face_vertical.hh>
# include <mln/world/k1/is_1_face_horizontal.hh>
# include <mln/world/k1/is_0_face.hh>


namespace mln
{

  namespace world
  {

    namespace k1
    {

      /*! \brief Immerse a 2D image into K1 and copy 2 faces data into
       *  0 and 1 faces.

       Data is copied into 1 faces located at the bottom right of each
       2 faces.

	           -1 0 1 2 3
	 0 1     -1 a a a d d
       0 a d      0 a a a d d
       1 b c  ->  1 a a a d d
	          2 b b b c c
	 	  3 b b b c c

       */
      template <typename I>
      mln_concrete(I)
      immerse_and_duplicate_2_to_0_1_faces(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

      // Facade

      template <typename I>
      mln_concrete(I) immerse_and_duplicate_2_to_0_1_faces(const Image<I>& ima_)
      {
	trace::entering("mln::world::k1::immerse_and_duplicate_2_to_0_1_faces");
	mln_precondition(exact(ima_).is_valid());
	const I& ima = exact(ima_);

	mln_concrete(I) output = immerse(ima);

	mln_piter(I) p(output.domain());
	for_all(p)
	  if (is_1_face_vertical(p))
	  {
	    if (output.domain().has(p + left))
	      output(p) = output(p + left);
	    else // Handle left border
	      output(p) = output(p + right);
	  }
	  else if (is_1_face_horizontal(p))
	  {
	    if (output.domain().has(p + up))
	      output(p) = output(p + up);
	    else  // Handle top border
	      output(p) = output(p + down);
	  }
	  else if (is_0_face(p))
	  {
	    if (output.domain().has(p + up_left))
	      output(p) = output(p + up_left);
	    else if (output.domain().has(p + up_right)) // Handle left border
	      output(p) = output(p + up_right);
	    else
	      output(p) = output(p + down_left); // Handle top border
	  }

	// First top left corner pixel. (Special case)
	output(output.domain().pmin()) = output(output.domain().pmin() + down_right);

	trace::exiting("mln::world::k1::immerse_and_duplicate_2_to_0_1_faces");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k1

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K1_IMMERSE_AND_DUPLICATE_2_TO_0_1_FACES_HH
