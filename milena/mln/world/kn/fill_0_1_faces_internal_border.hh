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
/// \brief Fill 0 and 1 faces border with a value in a KN 2D image.

#ifndef MLN_WORLD_KN_FILL_0_1_FACES_INTERNAL_BORDER_HH
# define MLN_WORLD_KN_FILL_0_1_FACES_INTERNAL_BORDER_HH

# include <mln/core/alias/point2d.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {

      /*! \brief Fill 0 and 1 faces border with a value in a KN 2D image.

	\param[in,out] inout A 2D image immersed in KN.
	\param[in]     v     The border value.

	Example with \p v=1:

	. - . - .         1 1 1 1 1
	| o | o |         1 o | o 1
	. - . - .     ->  1 - . - 1
	| o | o |         1 o | o 1
	. - . - .         1 1 1 1 1

       */
      template <typename I>
      void fill_0_1_faces_internal_border(Image<I>& inout, const mln_value(I)& v);



# ifndef MLN_INCLUDE_ONLY


      // Facade


      template <typename I>
      void fill_0_1_faces_internal_border(Image<I>& inout_, const mln_value(I)& v)
      {
	trace::entering("mln::world::kn::fill_0_1_faces_internal_border");

	mln_precondition(exact(inout_).is_valid());
	I& inout = exact(inout_);

	// Horizontal borders
	for (mln::def::coord col = geom::min_col(inout); col <= geom::max_col(inout); ++col)
	{
	  inout.at_(geom::min_row(inout), col) = v;
	  inout.at_(geom::max_row(inout), col) = v;
	}

	// Vertical borders
	for (mln::def::coord row = geom::min_row(inout); row <= geom::max_row(inout); ++row)
	{
	  inout.at_(row, geom::min_col(inout)) = v;
	  inout.at_(row, geom::max_col(inout)) = v;
	}

	trace::exiting("mln::world::kn::fill_0_1_faces_internal_border");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_FILL_0_1_FACES_INTERNAL_BORDER_HH
