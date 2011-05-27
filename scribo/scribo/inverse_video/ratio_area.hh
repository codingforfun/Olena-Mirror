// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_INVERSE_VIDEO_RATIO_AREA_HH
# define SCRIBO_INVERSE_VIDEO_RATIO_AREA_HH

/// \file
///
/// \brief	Choose beetwen two lines according to the ratio of
///		components area on the total area.

# include <scribo/core/line_set.hh>

namespace scribo
{

  namespace inverse_video
  {
    /// Choose the line where the ratio of components area is the highest
    /*!
    ** \param[in]     line		The first line.
    ** \param[in]     line_inverse	The second line
    **
    ** \return	A double : if it is close to 0, it means the second line is
    **		more likely to be the right one. As opposite, if it is closer
    **		to 1, it means the first one should be the good one.
    */
    template <typename L>
    double
    ratio_area (scribo::line_info<L>& line,
		scribo::line_info<L>& line_inverse);

# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    double
    ratio_area (scribo::line_info<L>& line,
		scribo::line_info<L>& line_inverse)
    {
      mln::trace::entering("scribo::inverse_video::ratio_area");

      double area_comps_normal = 0;
      double area_comps_inverse = 0;

      const scribo::component_set<L> comps_normal = line.holder ().components ();
      const scribo::component_set<L> comps_inverse = line_inverse.holder ().components ();

      const mln::util::array<scribo::component_id_t>& comps_id_normal = line.component_ids();
      const mln::util::array<scribo::component_id_t>& comps_id_inverse = line_inverse.component_ids();

      for (unsigned i = 0; i < comps_id_normal.nelements(); ++i)
	area_comps_normal += comps_normal(comps_id_normal[i]).bbox().nsites();

      for (unsigned i = 0; i < comps_id_inverse.nelements(); ++i)
	area_comps_inverse += comps_inverse(comps_id_inverse[i]).bbox().nsites();

      area_comps_inverse /= line_inverse.bbox().nsites();
      area_comps_normal /= line.bbox().nsites();

      mln::trace::exiting("scribo::inverse_video::ratio_area");

      return (area_comps_normal / (area_comps_normal + area_comps_inverse));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::inverse_video

} // end of namespace scribo

#endif // ! SCRIBO_INVERSE_VIDEO_RATIO_AREA_HH
