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

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_EXTRACT_PARAGRAPHS_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_EXTRACT_PARAGRAPHS_HH

# include <scribo/layout/internal/docstrum/line.hh>
# include <scribo/layout/internal/docstrum/paragraph.hh>
# include <scribo/layout/internal/docstrum/kd_tree.hh>
# include <scribo/layout/internal/docstrum/between_closure.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace docstrum
      {

	using namespace mln;

	void
	extract_paragraphs(const std::vector<line>& lines,
			   std::vector<paragraph>& paragraphs,
			   const float within_distance,
			   const float between_distance,
			   const float theta);

# ifndef MLN_INCLUDE_ONLY

	//-------------------------
	// Extract paragraphs
	//-------------------------

	inline
	void extract_paragraphs(const std::vector<line>& lines,
				std::vector<paragraph>& paragraphs,
				const float within_distance,
				const float between_distance,
				const float theta)
	{
	  mln_trace("scribo::layout::internal::docstrum::extract_paragraphs");

	  // Build a KD tree for nearest line neighbor search
	  const unsigned nelements = lines.size();
	  kd_tree kd(nelements);

	  for (unsigned i = 0; i < nelements; ++i)
	  {
	    // Insert the bbox in the KD-tree
	    const bbox2d* b = &(lines[i].bbox());
	    kd.insert(b);
	  }

	  // Compute the nearest neighbors for each line
	  std::vector< std::vector<unsigned> > knn;
	  knn.reserve(nelements);

	  for (unsigned i = 0; i < nelements; ++i)
	  {
	    std::vector<unsigned> neighbors;
	    const line& l = lines[i];

	    kd.find_nnearest_within_range(neighbors, 3.0f * between_distance,
					  &l.bbox(), l.slope(), l.y_intercept());

	    knn.push_back(neighbors);
	  }

	  // Transitive closure on between-line nearest neighbors pairings
	  // using parallel and perpendicular distances
	  between_closure(lines, paragraphs, knn, within_distance,
			  between_distance, theta);
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_EXTRACT_PARAGRAPHS_HH
