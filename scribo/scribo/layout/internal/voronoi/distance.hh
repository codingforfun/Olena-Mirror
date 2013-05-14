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
/// \brief Routines used in Voronoi algorithm.


#ifndef SCRIBO_LAYOUT_INTERNAL_VORONOI_DISTANCE_HH
# define SCRIBO_LAYOUT_INTERNAL_VORONOI_DISTANCE_HH


# include <climits>
# include <map>
# include <vector>
# include <mln/core/alias/point2d.hh>
# include <mln/util/ord_pair.hh>
# include <mln/util/graph.hh>
# include <scribo/layout/internal/voronoi/flooding.hh>

namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace voronoi
      {

	using namespace mln;
	using namespace internal::voronoi;


	int euclidean_distance(const point2d& p1,
			       const point2d& p2);

	int components_min_distance(const outline_bbox& o_b1,
				    const outline_bbox& o_b2);

	void compute_adjacency_distances(const std::vector<outline_bbox>& bbox_list,
					 util::graph& rag,
					 std::map<util::ord_pair<unsigned>,
						  int>& distances,
					 int& max_distance);

# ifndef MLN_INCLUDE_ONLY

	inline
	int euclidean_distance(const point2d& p1,
			       const point2d& p2)
	{
	  const int delta_x = p1.col() - p2.col();
	  const int delta_y = p1.row() - p2.row();

	  return (delta_x * delta_x + delta_y * delta_y);
	}

	inline
	int components_min_distance(const outline_bbox& o_b1,
				    const outline_bbox& o_b2)
	{
	  const std::vector<point2d>& points_b1 = o_b1.outline_points;
	  const std::vector<point2d>& points_b2 = o_b2.outline_points;
	  const unsigned nelements_b1 = points_b1.size();
	  const unsigned nelements_b2 = points_b2.size();
	  int min_distance = INT_MAX;

	  for (unsigned i = 0; i < nelements_b1; ++i)
	  {
	    const point2d& p1 = points_b1[i];

	    for (unsigned j = 0; j < nelements_b2; ++j)
	    {
	      const point2d& p2 = points_b2[j];
	      const int distance = euclidean_distance(p1, p2);

	      if (distance < min_distance)
		min_distance = distance;
	    }
	  }

	  return min_distance;
	}

	inline
	void compute_adjacency_distances(const std::vector<outline_bbox>& bbox_list,
					 util::graph& rag,
					 std::map<util::ord_pair<unsigned>,
						  int>& distances,
					 int& max_distance)
	{
	  std::map<unsigned, std::vector<unsigned> > neighbors_seen;
	  mln_edge_iter_(util::graph) e(rag);

	  for_all(e)
	  {
	    const unsigned v1 = e.v1();
	    const unsigned v2 = e.v2();

	    const std::vector<unsigned>& n = neighbors_seen[v1];

	    if (std::find(n.begin(), n.end(), v2) != n.end())
	      continue;

	    neighbors_seen[v1].push_back(v2);
	    neighbors_seen[v2].push_back(v1);

	    int min_distance = components_min_distance(bbox_list[v1 - 1],
						       bbox_list[v2 - 1]);

	    min_distance = round(sqrt(min_distance));

	    if (min_distance > max_distance)
	      max_distance = min_distance;

	    distances[util::ord_pair<unsigned>(v1, v2)] = min_distance;
	  }
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::voronoi

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_VORONOI_DISTANCE_HH

