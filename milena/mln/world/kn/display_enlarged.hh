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
/// \brief Create a new image where 2 faces are enlarged for better
/// visualization.

#ifndef MLN_WORLD_KN_DISPLAY_ENLARGED_HH
# define MLN_WORLD_KN_DISPLAY_ENLARGED_HH

# include <mln/core/alias/point2d.hh>
# include <mln/world/kn/is_0_face.hh>
# include <mln/world/kn/is_1_face_vertical.hh>
# include <mln/world/kn/is_1_face_horizontal.hh>
# include <mln/world/kn/is_2_face.hh>
# include <mln/geom/all.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {

      /*! \brief Create a new image where 2 faces are enlarged for better
	visualization.

	Exemple with zoom = 3:
	\verbatim
	. - . - .                . - - - . - - - .
	| o | o |                | o o o | o o o |
	. - . - .      ->        | o o o | o o o |
	| o | o |                | o o o | o o o |
	. - . - .                . - - - . - - - .
	                         | o o o | o o o |
	                         | o o o | o o o |
	                         | o o o | o o o |
				 . - - - . - - - .
	\endverbatim

	\param[in] ima_kn A 2D image immersed in KN.
	\param[in] zoom The number of times 2 faces must be duplicated
	(must be odd).


	This function is useful for debugging to distinguish 2 faces
	from others.

      */
      template <typename I>
      mln_concrete(I)
      display_enlarged(const Image<I>& ima_kn, unsigned zoom);


# ifndef MLN_INCLUDE_ONLY


      // Facade

      template <typename I>
      mln_concrete(I)
      display_enlarged(const Image<I>& ima_kn_, unsigned zoom_)
      {
	trace::entering("mln::world::kn::display_enlarged");

	mln_precondition(exact(ima_kn_).is_valid());
	mln_assertion(zoom_ % 2);

	const I& ima_kn = exact(ima_kn_);
	int shift = zoom_ - 1;

	// No zoom: return original image.
	if (zoom_ == 1)
	  return ima_kn;

	mln_concrete(I) output(make::box2d(ima_kn.domain().pmin().row() * shift,
					   ima_kn.domain().pmin().col() * shift,
					   ima_kn.domain().pmax().row() * shift,
					   ima_kn.domain().pmax().col() * shift));

	const mln::def::coord
	  min_row = geom::min_row(ima_kn),
	  max_row = geom::max_row(ima_kn),
	  min_col = geom::min_col(ima_kn),
	  max_col = geom::max_col(ima_kn);

	for (mln::def::coord row = min_row; row <= max_row; ++row)
	{
	  for (mln::def::coord col = min_col; col <= max_col; ++col)
	  {
	    if (kn::is_0_face(row, col))
	    {
	      output.at_(row * shift, col * shift) = ima_kn.at_(row, col);
	    }
	    else if (kn::is_1_face_vertical(row, col))
	    {
	      for (mln::def::coord j = -(shift - 1); j <  (shift + 3); ++j)
	    	output.at_(row * shift + j, col * shift) = ima_kn.at_(row, col);
	    }
	    else if (kn::is_1_face_horizontal(row, col))
	    {
	      for (mln::def::coord j = -(shift - 1); j <  (shift + 3); ++j)
	    	output.at_(row * shift, col * shift  + j) = ima_kn.at_(row, col);
	    }
	    else if (kn::is_2_face(row, col))
	    {
	      for (mln::def::coord i = -(shift - 1); i <  (shift + 3); ++i)
	    	for (mln::def::coord j = -(shift - 1); j <  (shift + 3); ++j)
	    	  output.at_(row * shift + i, col * shift  + j) = ima_kn.at_(row, col);
	    }
	  }
	}

	trace::exiting("mln::world::kn::display_enlarged");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_DISPLAY_ENLARGED_HH

