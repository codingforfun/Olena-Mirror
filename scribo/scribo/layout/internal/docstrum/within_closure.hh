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

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_WITHIN_CLOSURE_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_WITHIN_CLOSURE_HH

# include <vector>

# include <scribo/layout/internal/docstrum/line.hh>
# include <scribo/layout/internal/docstrum/distances.hh>


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
	within_closure(std::vector<line>& lines,
		       const std::vector<nearestn>& clusters,
		       const int within_distance,
		       const int distance_factor,
		       const float theta_h);

# ifndef MLN_INCLUDE_ONLY

	//-------------------------
	// Within closure
	//-------------------------

	inline
	void
	within_closure(std::vector<line>& lines,
		       const std::vector<nearestn>& clusters,
		       const int within_distance,
		       const int distance_factor,
		       const float theta_h)
	{
	  mln_trace("scribo::layout::internal::docstrum::within_closure");

	  const int max_within_distance = distance_factor * within_distance;

	  std::vector<nearestn>::const_iterator it_begin = clusters.begin();
	  std::vector<nearestn>::const_iterator it_end = clusters.end();
	  std::set<unsigned> already_seen;
	  unsigned line_count = 0;

	  for (; it_begin != it_end; ++it_begin)
	  {
	    const nnpoint* n = (*it_begin).nnp();
	    const unsigned id = n->id();

	    if (already_seen.find(id) == already_seen.end())
	    {
	      const std::vector<nnpoint>& neighbors = (*it_begin).nearest_points();
	      const unsigned nelements = (*it_begin).ninserted();

	      std::vector<const nearestn*> propagation;
	      std::vector<const nearestn*> next_propagation;

	      // Initialize a new line with the current component
	      line l(n, line_count);
	      ++line_count;

	      // Firstly, we try link the current component n with its closest
	      // neighbors
	      for (unsigned i = 0; i < nelements; ++i)
	      {
		const nnpoint& nnp = neighbors[i];
		const float angle_h = nnp.angle_h;
		const int distance = nnp.distance;
		const unsigned id = nnp.id();

		if (distance <= max_within_distance &&
		    angle_h > -theta_h && angle_h < theta_h)
		{
		  if (already_seen.find(id) == already_seen.end())
		  {
		    l.add_component(&nnp);
		    already_seen.insert(id);
		    propagation.push_back(&clusters[id]);
		  }
		}
	      }

	      // Then, we propagate the linking to our neighbors
	      while (! propagation.empty())
	      {
		next_propagation.clear();
		const unsigned nelements = propagation.size();

		for (unsigned i = 0; i < nelements; ++i)
		{
		  const std::vector<nnpoint>& neighbors = propagation[i]->nearest_points();
		  const unsigned nelements = (*it_begin).ninserted();

		  for (unsigned i = 0; i < nelements; ++i)
		  {
		    const nnpoint& nnp = neighbors[i];
		    const float angle_h = nnp.angle_h;
		    const int distance = nnp.distance;
		    const unsigned id = nnp.id();

		    if (distance <= max_within_distance &&
			angle_h > -theta_h && angle_h < theta_h)
		    {
		      if (already_seen.find(id) == already_seen.end())
		      {
			l.add_component(&nnp);
			already_seen.insert(id);
			next_propagation.push_back(&clusters[id]);
		      }
		    }
		  }
		}

		propagation.clear();
		propagation = next_propagation;
	      }

	      // Compute linear regression of the centroids of the line's
	      // components
	      l.linear_regression();

	      lines.push_back(l);
	    }
	  }
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_WITHIN_CLOSURE_HH
