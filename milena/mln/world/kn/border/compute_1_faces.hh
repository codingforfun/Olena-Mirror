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

#ifndef MLN_WORLD_KN_BORDER_COMPUTE_1_FACES_HH
# define MLN_WORLD_KN_BORDER_COMPUTE_1_FACES_HH

# include <mln/core/concept/image.hh>
# include <mln/world/kn/is_2_face.hh>
# include <mln/world/kn/is_1_face_horizontal.hh>
# include <mln/world/kn/is_1_face_vertical.hh>
# include <mln/geom/min_row.hh>
# include <mln/geom/min_col.hh>
# include <mln/geom/max_row.hh>
# include <mln/geom/max_col.hh>


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
      void compute_1_faces(Image<I>& inout, const Function_vv2v<F>& f);



# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename F>
      void compute_1_faces(Image<I>& inout_, const Function_vv2v<F>& f_)
      {
	trace::entering("mln::world::kn::compute_1_faces");

	mln_precondition(exact(inout_).is_valid());
	mln_precondition(exact(inout_).border() >= 1);
	I& inout = exact(inout_);
	const F& f = exact(f_);

	def::coord
	  min_row_1 = geom::min_row(inout) - 1,
	  max_row_1 = geom::max_row(inout) + 1,
	  min_col_1 = geom::min_col(inout) - 1,
	  max_col_1 = geom::max_col(inout) + 1;

	// Fill horizontal border
	for (def::coord col = min_col_1; col <= max_col_1; ++col)
	  if (is_1_face_vertical(min_row_1, col))
	  {
	    // First row
	    inout.at_(min_row_1, col) = f(inout.at_(min_row_1, col - 1),
					  inout.at_(min_row_1, col + 1));
	    // Last row
	    inout.at_(max_row_1, col) = f(inout.at_(max_row_1, col - 1),
					  inout.at_(max_row_1, col + 1));
	  }

	// Fill vertical border
	for (def::coord row = min_row_1; row <= max_row_1; ++row)
	  if (is_1_face_horizontal(row, min_col_1))
	  {
	    // First col
	    inout.at_(row, min_col_1) = f(inout.at_(row - 1, min_col_1),
					  inout.at_(row + 1, min_col_1));
	    // Last col
	    inout.at_(row, max_col_1) = f(inout.at_(row - 1, max_col_1),
					  inout.at_(row + 1, max_col_1));
	  }

	trace::exiting("mln::world::kn::compute_1_faces");
      }


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::world::kn::border

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_BORDER_COMPUTE_1_FACES_HH
