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
/// \brief Look for background label in the borders of a labeled
/// image.

#ifndef MLN_WORLD_KN_INTERNAL_GET_BACKGROUND_LABEL_HH
# define MLN_WORLD_KN_INTERNAL_GET_BACKGROUND_LABEL_HH

# include <mln/core/concept/image.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      namespace internal
      {

        /// \brief Look for background label in the borders of a
        /// labeled image.
	template <typename I>
	mln_value(I)
	get_background_label(const Image<I>& lab, bool& has_bg_label);


# ifndef MLN_INCLUDE_ONLY

	template <typename I>
	mln_value(I)
	get_background_label(const Image<I>& lab_, bool& has_bg_label)
	{
	  trace::entering("mln::world::kn::internal::get_background_label");
	  mln_precondition(exact(lab_).is_valid());

	  const I& lab = exact(lab_);

	  mln_box(I) dom = lab.domain();
	  def::coord
	    min_row = dom.pmin().row(),
	    max_row = dom.pmax().row(),
	    min_col = dom.pmin().col(),
	    max_col = dom.pmax().col();

	  has_bg_label = false;
	  for (def::coord col = min_col; col <= max_col; ++col)
	  {
	    // first row
	    if (lab.at_(min_row, col) != 0)
	    {
	      has_bg_label = true;
	      return lab.at_(min_row, col);
	    }
	    // last row
	    if (lab.at_(max_row, col) != 0)
	    {
	      has_bg_label = true;
	      return lab.at_(max_row, col);
	    }
	  }
	  if (! has_bg_label)
	  {
	    for (def::coord row = min_row; row <= max_row; ++row)
	    {
	      // first col
	      if (lab.at_(row, min_col) != 0)
	      {
		has_bg_label = true;
		return lab.at_(row, min_col);
	      }
	      // last col
	      if (lab.at_(row, max_col) != 0)
	      {
		has_bg_label = true;
		return lab.at_(row, max_col);
	      }
	    }
	  }

	  trace::exiting("mln::world::kn::internal::get_background_label");
	  return 0;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::world::kn::internal

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_INTERNAL_GET_BACKGROUND_LABEL_HH
