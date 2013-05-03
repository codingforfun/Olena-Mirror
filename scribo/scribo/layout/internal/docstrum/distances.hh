// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_DISTANCES_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_DISTANCES_HH

# include <vector>

# include <mln/core/image/image2d.hh>

# include <scribo/layout/internal/docstrum/nearest_neighbors.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace docstrum
      {

	using namespace mln;


	int
	within_line_distance(const std::vector<nearestn>& clusters,
			     const float theta_h);


	int
	between_line_distance(const std::vector<nearestn>& clusters,
			      const float theta_v);


# ifndef MLN_INCLUDE_ONLY

	//-------------------------
	// Within-line distance
	//-------------------------

	inline
	int
	within_line_distance(const std::vector<nearestn>& clusters,
			     const float theta_h)
	{
	  mln_trace("scribo::layout::internal::docstrum::within_line_distance");

	  std::vector<nearestn>::const_iterator it_begin = clusters.begin();
	  std::vector<nearestn>::const_iterator it_end = clusters.end();
	  std::map<int, int> distance_hist;
	  int max_value = 0;
	  int mean_distance = 0;

	  for (; it_begin != it_end; ++it_begin)
	  {
	    const std::vector<nnpoint>& neighbors =
	      (*it_begin).nearest_points();
	    const unsigned nelements = (*it_begin).ninserted();

	    for (unsigned i = 0; i < nelements; ++i)
	    {
	      const nnpoint& nnp = neighbors[i];
	      const float angle_h = nnp.angle_h;

	      if (angle_h > -theta_h && angle_h < theta_h)
	      {
		const int distance = nnp.distance;
		const int value = ++distance_hist[distance];

		if (value > max_value)
		{
		  max_value = value;
		  mean_distance = distance;
		}
	      }
	    }
	  }

	  return mean_distance;
	}



	//-------------------------
	// Between-line distance
	//-------------------------

	inline
	int
	between_line_distance(const std::vector<nearestn>& clusters,
			      const float theta_v)
	{
	  mln_trace("scribo::layout::internal::docstrum::between_line_distance");

	  std::vector<nearestn>::const_iterator it_begin = clusters.begin();
	  std::vector<nearestn>::const_iterator it_end = clusters.end();
	  std::map<int, int> distance_hist;
	  int max_value = 0;
	  int mean_distance = 0;

	  for (; it_begin != it_end; ++it_begin)
	  {
	    const std::vector<nnpoint>& neighbors = (*it_begin).nearest_points();
	    const unsigned nelements = (*it_begin).ninserted();

	    for (unsigned i = 0; i < nelements; ++i)
	    {
	      const nnpoint& nnp = neighbors[i];
	      const float angle_v = nnp.angle_v;

	      if (angle_v > -theta_v && angle_v < theta_v)
	      {
		const int distance = nnp.distance;
		const int value = ++distance_hist[distance];

		if (value > max_value)
		{
		  max_value = value;
		  mean_distance = distance;
		}
	      }
	    }
	  }

	  return mean_distance;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_DISTANCES_HH
