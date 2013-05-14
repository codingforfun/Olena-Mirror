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


#ifndef SCRIBO_LAYOUT_INTERNAL_VORONOI_FLOODING_HH
# define SCRIBO_LAYOUT_INTERNAL_VORONOI_FLOODING_HH

# include <climits>
# include <vector>
# include <map>
# include <mln/core/image/image2d.hh>
# include <mln/util/ord_pair.hh>
# include <mln/util/graph.hh>
# include <scribo/layout/internal/voronoi/bbox2d.hh>

namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace voronoi
      {
	using namespace mln;


	struct outline_bbox
	{
	  outline_bbox();

	  bbox2d bbox;
	  unsigned area;
	  std::vector<point2d> outline_points;
	};

	void propagate(const point2d& p,
		       std::vector<point2d>& next_position_list,
		       const image2d<bool>& input,
		       image2d<unsigned>& labeled_image,
		       const unsigned nlabels,
		       outline_bbox& o_b);

	void flood(const image2d<bool>& input,
		   image2d<unsigned>& labeled_image,
		   const unsigned nlabels,
		   const point2d& start_pt,
		   outline_bbox& o_b);


# ifndef MLN_INCLUDE_ONLY

	//---------------------------
	// Outline bbox
	//---------------------------

	outline_bbox::outline_bbox()
	  : area(0)
	{
	}

	//---------------------------
	// Propagate
	//---------------------------

	inline
	void propagate(const point2d& p,
		       std::vector<point2d>& next_position_list,
		       const image2d<bool>& input,
		       image2d<unsigned>& labeled_image,
		       const unsigned nlabels,
		       outline_bbox& o_b)
	{
	  int x = p.col();
	  int y = p.row();
	  int nx;
	  int ny;
	  int ncols = input.ncols();
	  int nrows = input.nrows();

	  {
	    nx = x + 1;

	    if (nx < ncols)
	    {
	      unsigned& pixel = labeled_image.at_(y, nx);
	      if (input.at_(y, nx) && !pixel)
	      {
		pixel = nlabels;
		next_position_list.push_back(point2d(y, nx));
		++o_b.area;
	      }
	    }
	  }

	  {
	    nx = x + 1;
	    ny = y + 1;

	    if (nx < ncols && ny < nrows)
	    {
	      unsigned& pixel = labeled_image.at_(ny, nx);
	      if (input.at_(ny, nx) && !pixel)
	      {
		pixel = nlabels;
		next_position_list.push_back(point2d(ny, nx));
		++o_b.area;
	      }
	    }
	  }

	  {
	    ny = y + 1;

	    if (ny < nrows)
	    {
	      unsigned& pixel = labeled_image.at_(ny, x);
	      if (input.at_(ny, x) && !pixel)
	      {
		pixel = nlabels;
		next_position_list.push_back(point2d(ny, x));
		++o_b.area;
	      }
	    }
	  }

	  {
	    nx = x - 1;
	    ny = y + 1;

	    if (nx >= 0 && ny < nrows)
	    {
	      unsigned& pixel = labeled_image.at_(ny, nx);
	      if (input.at_(ny, nx) && !pixel)
	      {
		pixel = nlabels;
		next_position_list.push_back(point2d(ny, nx));
		++o_b.area;
	      }
	    }
	  }

	  {
	    nx = x - 1;

	    if (nx >= 0)
	    {
	      unsigned& pixel = labeled_image.at_(y, nx);
	      if (input.at_(y, nx) && !pixel)
	      {
		pixel = nlabels;
		next_position_list.push_back(point2d(y, nx));
		++o_b.area;
	      }
	    }
	  }

	  {
	    nx = x - 1;
	    ny = y - 1;

	    if (nx >= 0 && ny >= 0)
	    {
	      unsigned& pixel = labeled_image.at_(ny, nx);
	      if (input.at_(ny, nx) && !pixel)
	      {
		pixel = nlabels;
		next_position_list.push_back(point2d(ny, nx));
		++o_b.area;
	      }
	    }
	  }

	  {
	    ny = y - 1;

	    if (ny >= 0)
	    {
	      unsigned& pixel = labeled_image.at_(ny, x);
	      if (input.at_(ny, x) && !pixel)
	      {
		pixel = nlabels;
		next_position_list.push_back(point2d(ny, x));
		++o_b.area;
	      }
	    }
	  }

	  {
	    nx = x + 1;
	    ny = y - 1;

	    if (nx < ncols && ny >= 0)
	    {
	      unsigned& pixel = labeled_image.at_(ny, nx);
	      if (input.at_(ny, nx) && !pixel)
	      {
		pixel = nlabels;
		next_position_list.push_back(point2d(ny, nx));
		++o_b.area;
	      }
	    }
	  }
	}

	//---------------------------
	// Flood
	//---------------------------

	inline
	void flood(const image2d<bool>& input,
		   image2d<unsigned>& labeled_image,
		   const unsigned nlabels,
		   const point2d& start_pt,
		   outline_bbox& o_b)
	{
	  std::vector<point2d> position_list;
	  std::vector<point2d> next_position_list;

	  next_position_list.reserve(8);
	  ++o_b.area;

	  position_list.push_back(start_pt);

	  while (! position_list.empty())
	  {
	    next_position_list.clear();
	    const unsigned nelements = position_list.size();

	    for (unsigned i = 0; i < nelements; ++i)
	    {
	      const point2d& p = position_list[i];

	      // Propagate the labeling
	      propagate(p, next_position_list, input, labeled_image, nlabels, o_b);
	    }

	    position_list.clear();
	    position_list = next_position_list;
	  }
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::voronoi

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_VORONOI_FLOODING_HH


