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
/// \brief Docstrum layout analysis algorithm.

#ifndef SCRIBO_LAYOUT_DOCSTRUM_HH
# define SCRIBO_LAYOUT_DOCSTRUM_HH

#include <iostream>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>
#include <mln/util/timer.hh>
#include <mln/data/convert.hh>
#include <mln/draw/box.hh>
#include <mln/literal/colors.hh>
#include <mln/core/image/image2d.hh>


#include <scribo/layout/internal/docstrum/all.hh>

using namespace mln;

namespace scribo
{

  namespace layout
  {

    using namespace mln;

    /*!
      \brief Extraction type for Docstrum algorithm.

      Defines how the components should be grouped in Docstrum
      algorithm.

      \sa scribo::docstrum
     */
    enum docstrum_extraction_type
    {
      Line,     ///< Group by lines
      Paragraph ///< Group by paragraphs.
    };


    /*!
      \brief Docstrum layout analysis algorithm.

      \param[in] ima A binary 2D document image.

      \param[in] extraction_type how the components should be grouped
      in results.

      \return An image of Paragraph or lines bounding boxes (w.r.t to
      \p extraction_type). Small elements are set to 1 and large ones
      to 2.

      This algorithm has been implemented w.r.t. the description in
      paper \cite ogorman1993pami.

      \ingroup grpalgolayout
     */
    template <typename I>
    mln_ch_value(I,value::int_u8)
    docstrum(const Image<I>& ima, docstrum_extraction_type extraction_type);


# ifndef MLN_INCLUDE_ONLY

    using namespace scribo::layout::internal::docstrum;

    template <typename I>
    mln_ch_value(I,value::int_u8)
    docstrum(const Image<I>& ima_, docstrum_extraction_type extraction_type)
    {
      mln_trace("scribo::layout::docstrum");

      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());
      mln_precondition(extraction_type == Line
		       || extraction_type == Paragraph);
      mlc_equal(mln_trait_image_kind(I),
		mln::trait::image::kind::binary)::check();

      //-------------------------
      // Component labeling
      //-------------------------

      std::vector<bbox2d> bbox_list;
      image2d<unsigned> labeled_img(ima.domain());
      data::fill(labeled_img, 0);
      unsigned nlabels = 0;

      component_labeling(ima, nlabels, bbox_list, labeled_img, 3);

      //-------------------------
      // Separation in two groups
      //-------------------------

      std::vector<bbox2d> small_comp;
      std::vector<bbox2d> big_comp;
      small_comp.reserve(nlabels / 2);
      big_comp.reserve(nlabels / 2);

      compute_comp_size(bbox_list, small_comp, big_comp, 3.0f);
      bbox_list.clear();

      //-------------------------
      // Extract lines from small components
      //-------------------------

      std::vector<line> small_lines;
      int small_within_distance = 0;
      int small_between_distance = 0;
      extract_lines(small_comp, small_lines, small_within_distance,
		    small_between_distance, math::pi / 6, math::pi / 6, 5);

      //-------------------------
      // Extract lines from big components
      //-------------------------

      std::vector<line> big_lines;
      int big_within_distance = 0;
      int big_between_distance = 0;
      extract_lines(big_comp, big_lines, big_within_distance,
		    big_between_distance, math::pi / 6, math::pi / 6, 5);

      //-------------------------
      // Extract paragraphs if requested
      //-------------------------

      std::vector<paragraph> small_paragraphs;
      std::vector<paragraph> big_paragraphs;

      if (extraction_type == Paragraph)
      {
	//-------------------------
	// Extract paragraphs from small components
	//-------------------------

	extract_paragraphs(small_lines, small_paragraphs, 1.5f *
			   small_within_distance, 1.3f *
			   small_between_distance, math::pi / 6);

	//-------------------------
	// Extract paragraphs from big components
	//-------------------------

	extract_paragraphs(big_lines, big_paragraphs, 1.5f *
			   big_within_distance, 2.0f *
			   small_between_distance, math::pi / 6);
      }


      //-------------------------
      // Saving
      //-------------------------

      mln_ch_value(I,value::int_u8) output(ima.domain());
      data::fill(output, literal::zero);

      if (extraction_type == Line)
      {
	{
	  const unsigned small_nlines = small_lines.size();

	  for (unsigned i = 0; i < small_nlines; ++i)
	  {
	    const bbox2d& b = small_lines[i].bbox();
	    box2d box(b.pmin(), b.pmax());

	    draw::box(output, box, 1u);
	  }
	}

	{
	  const unsigned big_nlines = big_lines.size();

	  for (unsigned i = 0; i < big_nlines; ++i)
	  {
	    const bbox2d& b = big_lines[i].bbox();
	    box2d box(b.pmin(), b.pmax());

	    draw::box(output, box, 2u);
	  }
	}
      }
      else if (extraction_type == Paragraph)
      {
	{
	  const unsigned small_nparas = small_paragraphs.size();

	  for (unsigned i = 0; i < small_nparas; ++i)
	  {
	    const bbox2d& b = small_paragraphs[i].bbox();
	    box2d box(b.pmin(), b.pmax());

	    draw::box(output, box, 1u);
	  }
	}

	{
	  const unsigned big_nparas = big_paragraphs.size();

	  for (unsigned i = 0; i < big_nparas; ++i)
	  {
	    const bbox2d& b = big_paragraphs[i].bbox();
	    box2d box(b.pmin(), b.pmax());

	    draw::box(output, box, 2u);
	  }
	}
      }
      else
      {
	mln_trace_warning("Invalid extraction type!");
	abort();
      }

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_DOCSTRUM_HH
