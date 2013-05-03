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

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_EXTRACT_LINES_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_EXTRACT_LINES_HH

# include <scribo/layout/internal/docstrum/bbox2d.hh>
# include <scribo/layout/internal/docstrum/line.hh>
# include <scribo/layout/internal/docstrum/kd_tree.hh>
# include <scribo/layout/internal/docstrum/nearest_neighbors.hh>
# include <scribo/layout/internal/docstrum/within_closure.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace docstrum
      {

	using namespace mln;


	void extract_lines(std::vector<bbox2d>& comp,
			   std::vector<line>& lines,
			   int& within_distance,
			   int& between_distance,
			   const float theta_h,
			   const float theta_v,
			   const int k);

# ifndef MLN_INCLUDE_ONLY

	//-------------------------
	// Extract lines
	//-------------------------

	inline
	void
	extract_lines(std::vector<bbox2d>& comp,
		      std::vector<line>& lines,
		      int& within_distance,
		      int& between_distance,
		      const float theta_h,
		      const float theta_v,
		      const int k)
	{
	  mln_trace("scribo::layout::internal::docstrum::extract_lines");

	  // Build a KD tree for the nearest neighbors search
	  const unsigned nelements = comp.size();
	  kd_tree kd(nelements);

	  for (unsigned i = 0; i < nelements; ++i)
	  {
	    // Reset the bbox id of each element of small_comp for indexing
	    comp[i].set_bbox_number(i);

	    // Insert the bbox in the KD-tree
	    const bbox2d* b = &(comp[i]);
	    kd.insert(b);
	  }

	  // Compute the K nearest neighbors
	  std::vector<nearestn> knn;
	  knn.reserve(nelements);

	  for (unsigned i = 0; i < nelements; ++i)
	  {
	    const bbox2d* b = &(comp[i]);
	    const point2d b_center = b->pcenter();
	    nearestn n(k, b);

	    kd.find_nnearest(b, n);
	    n.compute_angles(b_center);
	    n.root_distances();
	    knn.push_back(n);
	  }

	  // Compute within line and between line mean distances
	  within_distance = within_line_distance(knn, theta_h);
	  between_distance = between_line_distance(knn, theta_v);

	  // Transitive closure on within-line nearest neighbor pairings
	  within_closure(lines, knn, within_distance, 3, theta_h);
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_EXTRACT_LINES_HH
