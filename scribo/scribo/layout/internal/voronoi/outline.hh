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


#ifndef SCRIBO_LAYOUT_INTERNAL_VORONOI_OUTLINE_HH
# define SCRIBO_LAYOUT_INTERNAL_VORONOI_OUTLINE_HH

#include <scribo/layout/internal/voronoi/flooding.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace voronoi
      {
	using namespace mln;


	void left_up(int& direction,
		     const image2d<bool>& input,
		     const point2d& cur_pt);

	void left_up_after(int& direction,
			   const unsigned i);

	void right_up(int& direction,
		      const image2d<bool>& input,
		      const point2d& cur_pt);

	void right_up_after(int& direction,
			    const unsigned i);

	void right_down(int& direction,
			const image2d<bool>& input,
			const point2d& cur_pt);

	void right_down_after(int& direction,
			      const unsigned i);

	void left_down(int& direction,
		       const image2d<bool>& input,
		       const point2d& cur_pt);

	void left_down_after(int& direction,
			     const unsigned i);

	void find_next_point(const image2d<bool>& input,
			     point2d& cur_pt,
			     int& direction);

	unsigned follow_outline(outline_bbox& o_b,
				const point2d& start_pt,
				const image2d<bool>& input,
				image2d<unsigned>& labeled_image,
				const unsigned sampling_interval,
				const unsigned nlabels);

# ifndef MLN_INCLUDE_ONLY

	//-------------------------
	// Offset
	//-------------------------

	static const int offset[4][8][2] =
	{
	  { { -1, 0 }, { 0, -1 }, { -1, -1 }, { 1, 0 }, { 1, -1 }, { 0, 1 }, {
	      1, 1 }, { -1, 1 } },
	  { { 0, -1 }, { 1, 0 }, { 1, -1 }, { 0, 1 }, { 1, 1 }, { -1, 0 }, {
	      -1, 1 }, { -1, -1 } },
	  { { 1, 0 }, { 0, 1 }, { 1, 1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, {
	      -1, -1 }, { 1, -1 } },
	  { { 0, 1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { -1, -1 }, { 1, 0 }, {
	      1, -1 }, { 1, 1 } }
	};

	//-------------------------
	// Left up
	//-------------------------

	inline
	void left_up(int& direction,
		     const image2d<bool>& input,
		     const point2d& cur_pt)
	{
	  const point2d p2(cur_pt.row() + offset[direction][5][1],
			   cur_pt.col() + offset[direction][5][0]);
	  const point2d p3(cur_pt.row() + offset[direction][7][1],
			   cur_pt.col() + offset[direction][7][0]);

	  if (!input(p2) && input(p3))
	  {
	    direction = 3;
	    return;
	  }
	}

	//-------------------------
	// After left up
	//-------------------------

	inline
	void left_up_after(int& direction,
			   const unsigned i)
	{
	  if (i == 3 || i == 4)
	    direction = 1;
	  else if (i == 5 || i == 6)
	    direction = 2;
	  else if (i == 7)
	    direction = 3;
	}

	//-------------------------
	// Right up
	//-------------------------

	inline
	void right_up(int& direction,
		      const image2d<bool>& input,
		      const point2d& cur_pt)
	{
	  const point2d p1(cur_pt.row() + offset[direction][0][1],
			   cur_pt.col() + offset[direction][0][0]);
	  const point2d p2(cur_pt.row() + offset[direction][5][1],
			   cur_pt.col() + offset[direction][5][0]);
	  const point2d p3(cur_pt.row() + offset[direction][7][1],
			   cur_pt.col() + offset[direction][7][0]);

	  if (!input(p2) && (input(p1) || input(p3)))
	  {
	    direction = 0;
	    return;
	  }
	}

	//-------------------------
	// After right up
	//-------------------------

	inline
	void right_up_after(int& direction,
			    const unsigned i)
	{
	  if (i == 3 || i == 4)
	    direction = 2;
	  else if (i == 5 || i == 6)
	    direction = 3;
	  else if (i == 7)
	    direction = 0;
	}

	//-------------------------
	// Right down
	//-------------------------

	inline
	void right_down(int& direction,
			const image2d<bool>& input,
			const point2d& cur_pt)
	{
	  const point2d p2(cur_pt.row() + offset[direction][5][1],
			   cur_pt.col() + offset[direction][5][0]);
	  const point2d p3(cur_pt.row() + offset[direction][7][1],
			   cur_pt.col() + offset[direction][7][0]);

	  if (!input(p2) && input(p3))
	  {
	    direction = 1;
	    return;
	  }
	}

	//-------------------------
	// After right down
	//-------------------------

	inline
	void right_down_after(int& direction,
			      const unsigned i)
	{
	  if (i == 3 || i == 4)
	    direction = 3;
	  else if (i == 5 || i == 6)
	    direction = 0;
	  else if (i == 7)
	    direction = 1;
	}

	//-------------------------
	// Left down
	//-------------------------

	inline
	void left_down(int& direction,
		       const image2d<bool>& input,
		       const point2d& cur_pt)
	{
	  const point2d p1(cur_pt.row() + offset[direction][0][1],
			   cur_pt.col() + offset[direction][0][0]);
	  const point2d p2(cur_pt.row() + offset[direction][5][1],
			   cur_pt.col() + offset[direction][5][0]);
	  const point2d p3(cur_pt.row() + offset[direction][7][1],
			   cur_pt.col() + offset[direction][7][0]);

	  if (!input(p2) && (input(p1) || input(p3)))
	  {
	    direction = 2;
	    return;
	  }
	}

	//-------------------------
	// After left down
	//-------------------------

	inline
	void left_down_after(int& direction,
			     const unsigned i)
	{
	  if (i == 3 || i == 4)
	    direction = 0;
	  else if (i == 5 || i == 6)
	    direction = 1;
	  else if (i == 7)
	    direction = 2;
	}

	//-------------------------
	// Find next point
	//-------------------------

	inline
	void find_next_point(const image2d<bool>& input,
			     point2d& cur_pt,
			     int& direction)
	{
	  unsigned i = 0;
	  point2d tmp;

	  switch (direction)
	  {
	    case 0: left_up(direction, input, cur_pt); break;
	    case 1: right_up(direction , input, cur_pt); break;
	    case 2: right_down(direction, input, cur_pt); break;
	    case 3: left_down(direction, input, cur_pt); break;
	  }

	  for (; i < 8; ++i)
	  {
	    tmp = point2d(cur_pt.row() + offset[direction][i][1],
			  cur_pt.col() + offset[direction][i][0]);

	    if (input(tmp))
	      break;
	  }

	  // Should not happen
	  if (i == 8)
	    return;

	  switch (direction)
	  {
	    case 0: left_up_after(direction, i); break;
	    case 1: right_up_after(direction , i); break;
	    case 2: right_down_after(direction, i); break;
	    case 3: left_down_after(direction, i); break;
	  }

	  cur_pt = tmp;
	}

	//-------------------------
	// Follow outline
	//-------------------------

	inline
	unsigned follow_outline(outline_bbox& o_b,
				const point2d& start_pt,
				const image2d<bool>& input,
				image2d<unsigned>& labeled_image,
				const unsigned sampling_interval,
				const unsigned nlabels)
	{
	  int direction = 0;
	  point2d cur_pt = start_pt;
	  std::vector<point2d>& points = o_b.outline_points;
	  unsigned sample_count = 0;
	  unsigned outline_count = 0;

	  // Flood the connected component
	  flood(input, labeled_image, nlabels, start_pt, o_b);

	  find_next_point(input, cur_pt, direction);
	  points.push_back(cur_pt);
	  labeled_image(cur_pt) = nlabels;
	  ++sample_count;
	  ++outline_count;

	  while (cur_pt != start_pt)
	  {
	    find_next_point(input, cur_pt, direction);
	    ++outline_count;
	    o_b.bbox.extend(cur_pt);

	    if (sample_count == 0)
	      points.push_back(cur_pt);

	    ++sample_count;

	    if (sample_count == sampling_interval)
	      sample_count = 0;
	  }

	  find_next_point(input, cur_pt, direction);

	  if (std::find(points.begin(), points.end(), cur_pt) == points.end())
	  {
	    points.push_back(cur_pt);

	    while (cur_pt != start_pt)
	    {
	      find_next_point(input, cur_pt, direction);
	      ++outline_count;
	      o_b.bbox.extend(cur_pt);

	      if (sample_count == 0)
		points.push_back(cur_pt);

	      ++sample_count;

	      if (sample_count == sampling_interval)
		sample_count = 0;
	    }
	  }

	  return outline_count;
	}


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::voronoi

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_VORONOI_OUTLINE_HH




