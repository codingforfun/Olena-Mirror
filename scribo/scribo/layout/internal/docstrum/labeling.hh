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

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_LABELING_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_LABELING_HH

# include <vector>

# include <mln/core/image/image2d.hh>

# include <scribo/layout/internal/docstrum/bbox2d.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace docstrum
      {

	using namespace mln;

	struct labeling_position
	{
	  labeling_position(const int x_, const int y_);

	  int x;
	  int y;
	};


	void
	propagate(const labeling_position& position,
		  std::vector< labeling_position >& next_position_list,
		  const image2d<bool>& input, image2d<unsigned>& labeled_image,
		  const unsigned nb_labels, bbox2d& bbox);


	void
	component_labeling(const image2d<bool>& input,
			   unsigned& nb_labels,
			   std::vector<bbox2d>& bbox_list,
			   image2d<unsigned>& labeled_image,
			   const unsigned min_size);

# ifndef MLN_INCLUDE_ONLY

	//---------------------------
	// Labeling position
	//---------------------------

	labeling_position::labeling_position(const int x_, const int y_)
	  : x(x_),
	    y(y_)
	{
	}

	//---------------------------
	// Propagate labeling
	//---------------------------

	inline
	void propagate(const labeling_position& position,
		       std::vector< labeling_position >& next_position_list,
		       const image2d<bool>& input, image2d<unsigned>& labeled_image,
		       const unsigned nb_labels, bbox2d& bbox)
	{
	  mln_trace("scribo::layout::internal::docstrum::component_labeling");

	  mln_assertion(input.is_valid());
	  mln_assertion(labeled_image.is_valid());

	  int x = position.x;
	  int y = position.y;
	  int nx;
	  int ny;
	  int ncols = input.ncols();
	  int nrows = input.nrows();

	  bool increment_xlr = false;
	  bool increment_ylr = false;
	  bool decrement_xul = false;
	  bool decrement_yul = false;

	  point2d& pmin = bbox.pmin();
	  point2d& pmax = bbox.pmax();
	  const int xul = pmin.col();
	  const int yul = pmin.row();
	  const int xlr = pmax.col();
	  const int ylr = pmax.row();

	  {
	    nx = x + 1;

	    if (nx < ncols)
	    {
	      unsigned& pixel = labeled_image.at_(y, nx);
	      if (input.at_(y, nx) && !pixel)
	      {
		if (xlr < nx)
		  increment_xlr = true;
		pixel = nb_labels;
		next_position_list.push_back(labeling_position(nx, y));
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
		if (xlr < nx)
		  increment_xlr = true;
		if (ylr < ny)
		  increment_ylr = true;
		pixel = nb_labels;
		next_position_list.push_back(labeling_position(nx, ny));
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
		if (ylr < ny)
		  increment_ylr = true;
		pixel = nb_labels;
		next_position_list.push_back(labeling_position(x, ny));
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
		if (xul > nx)
		  decrement_xul = true;
		if (ylr < ny)
		  increment_ylr = true;
		pixel = nb_labels;
		next_position_list.push_back(labeling_position(nx, ny));
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
		if (xul > nx)
		  decrement_xul = true;
		pixel = nb_labels;
		next_position_list.push_back(labeling_position(nx, y));
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
		if (xul > nx)
		  decrement_xul = true;
		if (yul > ny)
		  decrement_yul = true;
		pixel = nb_labels;
		next_position_list.push_back(labeling_position(nx, ny));
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
		if (yul > ny)
		  decrement_yul = true;
		pixel = nb_labels;
		next_position_list.push_back(labeling_position(x, ny));
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
		if (xlr < nx)
		  increment_xlr = true;
		if (yul > ny)
		  decrement_yul = true;
		pixel = nb_labels;
		next_position_list.push_back(labeling_position(nx, ny));
	      }
	    }
	  }

	  if (increment_xlr)
	    ++pmax.col();
	  if (increment_ylr)
	    ++pmax.row();
	  if (decrement_xul)
	    --pmin.col();
	  if (decrement_yul)
	    --pmin.row();
	}

	//---------------------------
	// Component labeling
	//---------------------------

	inline
	void component_labeling(const image2d<bool>& input,
				unsigned& nb_labels,
				std::vector<bbox2d>& bbox_list,
				image2d<unsigned>& labeled_image,
				const unsigned min_size)
	{
	  mln_trace("scribo::layout::internal::docstrum::component_labeling");

	  mln_assertion(input.is_valid());
	  mln_assertion(labeled_image.is_valid());

	  std::vector<labeling_position> position_list;
	  std::vector<labeling_position> next_position_list;
	  unsigned nrows = input.nrows();
	  unsigned ncols = input.ncols();

	  next_position_list.reserve(8);

	  for (unsigned i = 0; i < nrows; ++i)
	  {
	    for (unsigned j = 0; j < ncols; ++j)
	    {
	      if (input.at_(i, j) && !labeled_image.at_(i, j))
	      {
		++nb_labels;
		position_list.clear();
		position_list.push_back(labeling_position(j, i));

		labeled_image.at_(i, j) = nb_labels;

		point2d p(i, j);
		bbox2d bbox(nb_labels, p, p);

		while (!position_list.empty())
		{
		  unsigned nb_positions = position_list.size();
		  next_position_list.clear();

		  for (unsigned k = 0; k < nb_positions; ++k)
		  {
		    const labeling_position& position = position_list[k];

		    propagate(position, next_position_list, input,
			      labeled_image, nb_labels, bbox);
		  }

		  position_list.clear();
		  position_list = next_position_list;
		}

		const unsigned height = bbox.pmax().row() - bbox.pmin().row();
		const unsigned width = bbox.pmax().col() - bbox.pmin().col();

		if (height > min_size || width > min_size)
		  bbox_list.push_back(bbox);
	      }
	    }
	  }
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_LABELING_HH
