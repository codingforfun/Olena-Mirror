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


#ifndef SCRIBO_LAYOUT_INTERNAL_VORONOI_FILTER_HH
# define SCRIBO_LAYOUT_INTERNAL_VORONOI_FILTER_HH

# include <climits>
# include <vector>
# include <map>
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

	void invalidate_edges(const std::map<util::ord_pair<unsigned>, int>& distances,
			      const std::vector<outline_bbox>& bbox_list,
			      const util::graph& rag,
			      util::graph& g,
			      const float td1,
			      const float td2,
			      const float ta);

# ifndef MLN_INCLUDE_ONLY

	inline
	void invalidate_edges(const std::map<util::ord_pair<unsigned>, int>& distances,
			      const std::vector<outline_bbox>& bbox_list,
			      const util::graph& rag,
			      util::graph& g,
			      const float td1,
			      const float td2,
			      const float ta)
	{
	  mln_edge_iter_(util::graph) e(rag);

	  for_all(e)
	  {
	    const unsigned v1 = e.v1();
	    const unsigned v2 = e.v2();

	    const outline_bbox& b1 = bbox_list[v1 - 1];
	    const outline_bbox& b2 = bbox_list[v2 - 1];

	    std::map<util::ord_pair<unsigned>, int>::const_iterator it_map =
	      distances.find(util::ord_pair<unsigned>(v1, v2));

	    if (it_map != distances.end())
	    {
	      const float d = (*it_map).second;
	      const float area1 = b1.area;
	      const float area2 = b2.area;
	      const float area_ratio = std::max(area1, area2) /
		std::min(area1, area2);
	      const bool cond1 = (d / td1) < 1;
	      const bool cond2 = (d / td2 + area_ratio / ta) < 1;

	      if (cond1 || cond2)
		g.add_edge(e.v1(), e.v2());
	    }
	  }
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::voronoi

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_VORONOI_FILTER_HH


