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


#ifndef SCRIBO_LAYOUT_INTERNAL_VORONOI_LABELING_HH
# define SCRIBO_LAYOUT_INTERNAL_VORONOI_LABELING_HH


#include <mln/core/image/image2d.hh>
#include <scribo/layout/internal/voronoi/bbox2d.hh>
#include <scribo/layout/internal/voronoi/outline.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace voronoi
      {
	using namespace mln;


	void erase_component(image2d<bool>& input,
			     image2d<unsigned>& labeled_img,
			     const bbox2d& bbox,
			     const unsigned value);

	void component_labeling(image2d<bool>& input,
				unsigned& nlabels,
				std::vector<outline_bbox>& bbox_list,
				image2d<unsigned>& labeled_image,
				const unsigned sampling_interval,
				const unsigned minimum_outline_length,
				const unsigned maximum_width,
				const unsigned maximum_height);

# ifndef MLN_INCLUDE_ONLY

	//---------------------------
	// Erase component
	//---------------------------

	inline
	void erase_component(image2d<bool>& input,
			     image2d<unsigned>& labeled_img,
			     const bbox2d& bbox,
			     const unsigned value)
	{
	  box2d b(bbox.pmin(), bbox.pmax());
	  mln_piter_(box2d) p(b);

	  for_all(p)
	  {
	    if (labeled_img(p) == value)
	    {
	      input(p) = false;
	      labeled_img(p) = 0;
	    }
	  }
	}

	//---------------------------
	// Component labeling
	//---------------------------

	inline
	void component_labeling(image2d<bool>& input,
				unsigned& nlabels,
				std::vector<outline_bbox>& bbox_list,
				image2d<unsigned>& labeled_image,
				const unsigned sampling_interval,
				const unsigned minimum_outline_length,
				const unsigned maximum_width,
				const unsigned maximum_height)
	{
	  const int nrows = input.nrows();
	  const int ncols = input.ncols();

	  for (int i = 0; i < nrows; ++i)
	  {
	    for (int j = 0; j < ncols; ++j)
	    {
	      if (input.at_(i, j))
	      {
		if (!labeled_image.at_(i, j))
		{
		  ++nlabels;
		  labeled_image.at_(i, j) = nlabels;

		  point2d p(i, j);
		  outline_bbox o_b;
		  o_b.bbox = bbox2d(nlabels, p, p);

		  const unsigned outline_length =
		    follow_outline(o_b, p, input, labeled_image,
				   sampling_interval, nlabels);

		  const bbox2d& b = o_b.bbox;
		  const unsigned width = b.pmax().col() - b.pmin().col();
		  const unsigned height = b.pmax().row() - b.pmin().row();

		  if (outline_length > minimum_outline_length &&
		      width < maximum_width &&
		      height < maximum_height)
		    bbox_list.push_back(o_b);
		  else
		  {
		    erase_component(input, labeled_image, o_b.bbox, nlabels);
		    --nlabels;
		  }
		}
		else
		  for (; input.at_(i, j); ++j);
	      }
	    }
	  }
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::voronoi

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_VORONOI_LABELING_HH



