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
/// \brief Routines used in scribo::layout::smearing

#ifndef SCRIBO_LAYOUT_INTERNAL_SMEARING_DRAW_REGION_HH
# define SCRIBO_LAYOUT_INTERNAL_SMEARING_DRAW_REGION_HH

# include <mln/core/image/image2d.hh>
# include <scribo/layout/internal/smearing/bbox2d.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace smearing
      {

	using namespace mln;


	template< typename V >
	void remove_text_region(const bbox2d& bbox,
				const image2d<bool>& input,
				image2d<bool>& output,
				image2d<V>& labeled_img_orig,
				const image2d<V>& labeled_img);

	template< typename V >
	void draw_region(const bbox2d& bbox,
			 const image2d<bool>& input,
			 image2d<bool>& output,
			 image2d<V>& labeled_img);

# ifndef MLN_INCLUDE_ONLY

	template< typename V >
	void remove_text_region(const bbox2d& bbox,
				const image2d<bool>& input,
				image2d<bool>& output,
				image2d<V>& labeled_img_orig,
				const image2d<V>& labeled_img)
	{
	  const unsigned id = bbox.bbox_number();
	  box2d b(bbox.pmin(), bbox.pmax());
	  mln_piter_(box2d) p(b);

	  for_all(p)
	  {
	    const bool pixel = input(p);
	    const V label_pixel = labeled_img(p);

	    if (pixel && label_pixel == id)
	    {
	      output(p) = true;
	      labeled_img_orig(p) = 0;
	    }
	  }
	}

	template< typename V >
	void draw_region(const bbox2d& bbox,
			 const image2d<bool>& input,
			 image2d<bool>& output,
			 image2d<V>& labeled_img)
	{
	  const unsigned id = bbox.bbox_number();
	  box2d b(bbox.pmin(), bbox.pmax());
	  mln_piter_(box2d) p(b);

	  for_all(p)
	  {
	    const bool pixel = input(p);
	    const V label_pixel = labeled_img(p);

	    if (pixel && label_pixel == id)
	      output(p) = true;
	  }
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::smearing

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_SMEARING_DRAW_REGION_HH
