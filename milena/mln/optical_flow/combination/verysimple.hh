// Copyright (C) 2013 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_OPTICAL_FLOW_COMBINATION_VERYSIMPLE_HH_
# define MLN_OPTICAL_FLOW_COMBINATION_VERYSIMPLE_HH_

/// \file
///
/// \brief The simpliest combination.

# include <mln/value/int_u8.hh>
# include <mln/core/alias/vec2d.hh>
# include <mln/core/concept/image.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/pw/value.hh>
# include <mln/core/image/dmorph/image_if.hh>

namespace mln
{

  namespace optical_flow
  {

    namespace combination
    {

      /// \brief The simpliest combination
      ///
      /// @param optical_flowHS The optical flow from the global algorithm
      /// @param optical_flowLK The optical flow from the local algorithm
      /// @param mask The mask of the trust map

      void
      verysimpleCombination (mln::image2d<mln::vec2d_f>& optical_flowHS,
			     mln::image2d<mln::vec2d_f>& optical_flowLK,
			     mln::image2d<bool>& mask);

# ifndef MLN_INCLUDE_ONLY
      void
      verysimpleCombination (mln::image2d<mln::vec2d_f>& optical_flowHS,
			     mln::image2d<mln::vec2d_f>& optical_flowLK,
			     mln::image2d<bool>& mask)
      {
	mln_piter_ (mln::image2d<mln::vec2d_f>) p2 (optical_flowHS.domain());

	for_all(p2)
	{
	  if (mask(p2) == false)
	    optical_flowLK(p2) = optical_flowHS(p2);
	}
      }

# endif // ! MLN_INCLUDE_ONLY

    } //end of namespace combination

  } //end of namespace optical_flow

} //end of namespace mln

#endif // ! MLN_OPTICAL_FLOW_COMBINATION_VERYSIMPLE_HH_
