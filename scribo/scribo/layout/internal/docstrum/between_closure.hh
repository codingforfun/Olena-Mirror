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

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_BETWEEN_CLOSURE_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_BETWEEN_CLOSURE_HH

# include <vector>

# include <scribo/layout/internal/docstrum/line.hh>
# include <scribo/layout/internal/docstrum/paragraph.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace docstrum
      {

	using namespace mln;


	float
	compute_lines_angle(const line& l1, const line& l2);


	void
	project_point(const line& destination,
		      const point2d& p,
		      point2d& projection);


	float
	compute_parallel_distance(const line& origin,
				  const line& destination,
				  point2d& p_center);

	float
	point_to_line_distance(const float slope,
			       const float y_intercept,
			       const point2d& p_center);


	void
	between_closure(const std::vector<line>& lines,
			std::vector<paragraph>& paragraphs,
			const std::vector< std::vector<unsigned> >& clusters,
			const float within_distance,
			const float between_distance,
			const float theta);

# ifndef MLN_INCLUDE_ONLY

	//-------------------------
	// Compute angle between two lines
	//-------------------------

	inline
	float compute_lines_angle(const line& l1,
				  const line& l2)
	{
	  const point2d& l1_begin = l1.line_begin();
	  const point2d& l1_end = l1.line_end();
	  const point2d& l2_begin = l2.line_begin();
	  const point2d& l2_end = l2.line_end();

	  const float delta_y_l1 = l1_end.row() - l1_begin.row();
	  const float delta_x_l1 = l1_end.col() - l1_begin.col();
	  const float delta_y_l2 = l2_end.row() - l2_begin.row();
	  const float delta_x_l2 = l2_end.col() - l2_begin.col();

	  return (atan(delta_y_l2 / delta_x_l2) - atan(delta_y_l1 / delta_x_l1));
	}

	//-------------------------
	// Projection of a point
	//-------------------------

	inline
	void project_point(const line& destination,
			   const point2d& p,
			   point2d& projection)
	{
	  const point2d& destination_begin = destination.line_begin();
	  const point2d& destination_end = destination.line_end();

	  const int delta_x = destination_end.col() - destination_begin.col();
	  const int delta_y = destination_end.row() - destination_begin.row();
	  const float length = (delta_x * delta_x) + (delta_y * delta_y);
	  float r = (destination_begin.row() - p.row()) * (-delta_y) -
	    (destination_begin.col() - p.col()) * (delta_x);
	  r /= length;

	  projection.row() = destination_begin.row() + r * delta_y;
	  projection.col() = destination_begin.col() + r * delta_x;
	}

	//-------------------------
	// Compute parallel distance
	//-------------------------

	inline
	float compute_parallel_distance(const line& origin,
					const line& destination,
					point2d& p_center)
	{
	  mln_trace("scribo::layout::internal::docstrum::compute_parallel_distance");

	  const point2d& origin_begin = origin.line_begin();
	  const point2d& origin_end = origin.line_end();
	  const point2d& destination_begin = destination.line_begin();
	  const point2d& destination_end = destination.line_end();
	  point2d projection_begin;
	  point2d projection_end;

	  project_point(destination, origin_begin, projection_begin);
	  project_point(destination, origin_end, projection_end);

	  point2d overlap_begin;
	  point2d overlap_end;
	  float overlap_length = 0.0f;
	  // float parallel_distance_left = 0;
	  // float parallel_distance_right = 0;

	  if (projection_begin.col() > destination_end.col() ||
	      projection_end.col() < destination_begin.col())
	    return -1.0f;

	  if (projection_begin.col() < destination_begin.col())
	    overlap_begin = destination_begin;
	  else
	    overlap_begin = projection_begin;

	  if (projection_end.col() < destination_end.col())
	    overlap_end = projection_end;
	  else
	    overlap_end = destination_end;

	  p_center = point2d(overlap_begin.row() + (overlap_end.row() -
						    overlap_begin.row()) / 2,
			     overlap_begin.col() + (overlap_end.col() -
						    overlap_begin.col()) / 2);

	  // const int delta_x_left = destination_begin.col() - projection_begin.col();
	  // const int delta_y_left = destination_begin.row() - projection_begin.row();
	  // const int delta_x_right = destination_end.col() - projection_end.col();
	  // const int delta_y_right = destination_end.row() - projection_end.row();

	  // parallel_distance_left = sqrt(delta_x_left * delta_x_left +
	  // 				delta_y_left * delta_y_left);
	  // parallel_distance_right = sqrt(delta_x_right * delta_x_right +
	  // 				delta_y_right * delta_y_right);

	  const int over_delta_x = overlap_end.col() - overlap_begin.col();
	  const int over_delta_y = overlap_end.row() - overlap_begin.row();

	  overlap_length = sqrt(over_delta_y * over_delta_y + over_delta_x
				* over_delta_x);

	  return overlap_length;

	  // return std::min(parallel_distance_left, parallel_distance_right);
	}

	//-------------------------
	// Point to line distance
	//-------------------------

	inline
	float point_to_line_distance(const float slope,
				     const float y_intercept,
				     const point2d& p_center)
	{
	  return (std::fabs(-slope * p_center.col() +
			    p_center.row() - y_intercept) /
		  sqrt(slope * slope + 1));
	}

	//-------------------------
	// Between closure
	//-------------------------

	inline
	void between_closure(const std::vector<line>& lines,
			     std::vector<paragraph>& paragraphs,
			     const std::vector< std::vector<unsigned> >& clusters,
			     const float within_distance,
			     const float between_distance,
			     const float theta)
	{
	  mln_trace("scribo::layout::internal::docstrum::between_closure");

	  // FIXME: why taking this value ?
	  (void) within_distance;

	  const unsigned nelements = lines.size();
	  std::set<unsigned> already_seen;
	  unsigned paragraph_count = 0;

	  for (unsigned i = 0; i < nelements; ++i)
	  {
	    const line& l = lines[i];
	    const unsigned id = l.id();

	    if (already_seen.find(id) == already_seen.end())
	    {
	      const std::vector<unsigned>& neighbors = clusters[i];

	      std::vector<unsigned>::const_iterator it_begin = neighbors.begin();
	      std::vector<unsigned>::const_iterator it_end = neighbors.end();

	      std::vector<const line*> propagation;
	      std::vector<const line*> next_propagation;

	      // Initialize a new paragraph with the current line
	      paragraph p(&l, paragraph_count);
	      ++paragraph_count;
	      already_seen.insert(id);

	      // Firstly, we try to link the current line with its closest
	      // neighbors
	      for (; it_begin != it_end; ++it_begin)
	      {
		// We first retrieve the corresponding line
		const line& ln = lines[(*it_begin)];
		const unsigned id_ln = ln.id();

		if (already_seen.find(id_ln) == already_seen.end())
		{
		  // Compute the angle between the two lines
		  const float angle = compute_lines_angle(l, ln);

		  if (angle > -theta && angle < theta)
		  {
		    point2d p_center;
		    const float p_distance = compute_parallel_distance(l, ln, p_center);

		    if (p_distance >= 0)
		    {
		      const float distance_to_line =
			point_to_line_distance(l.slope(), l.y_intercept(), p_center);

		      if (distance_to_line <= between_distance)
		      {
			p.add_line(&ln);
			already_seen.insert(id_ln);
			propagation.push_back(&lines[id_ln]);
		      }
		    }
		  }
		}
	      }

	      // Then, we propagate the linking to our neighbors
	      while (! propagation.empty())
	      {
		next_propagation.clear();
		const unsigned prop_nelements = propagation.size();

		for (unsigned j = 0; j < prop_nelements; ++j)
		{
		  const std::vector<unsigned>& neighbors =
		    clusters[propagation[j]->id()];
		  const line& ln1 = lines[propagation[j]->id()];

		  std::vector<unsigned>::const_iterator it_begin = neighbors.begin();
		  std::vector<unsigned>::const_iterator it_end = neighbors.end();

		  // Firstly, we try to link the current line with its closest
		  // neighbors
		  for (; it_begin != it_end; ++it_begin)
		  {
		    // We first retrieve the corresponding line
		    const line& ln = lines[(*it_begin)];
		    const unsigned id_ln = ln.id();

		    if (already_seen.find(id_ln) == already_seen.end())
		    {
		      // Compute the angle between the two lines
		      const float angle = compute_lines_angle(ln1, ln);

		      if (angle > -theta && angle < theta)
		      {
			point2d p_center;
			const float p_distance = compute_parallel_distance(ln1, ln, p_center);

			if (p_distance >= 0)
			{
			  const float distance_to_line = point_to_line_distance(ln1.slope(),
										ln1.y_intercept(), p_center);

			  if (distance_to_line <= between_distance)
			  {
			    p.add_line(&ln);
			    already_seen.insert(id_ln);
			    next_propagation.push_back(&lines[id_ln]);
			  }
			}
		      }
		    }
		  }
		}

		propagation.clear();
		propagation = next_propagation;
	      }

	      paragraphs.push_back(p);
	    }
	  }
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_BETWEEN_CLOSURE_HH
