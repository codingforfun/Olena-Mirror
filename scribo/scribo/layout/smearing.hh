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
/// \brief Smearing layout analysis algorithm.

#ifndef SCRIBO_LAYOUT_SMEARING_HH
# define SCRIBO_LAYOUT_SMEARING_HH

# include <mln/core/image/image2d.hh>
# include <mln/util/couple.hh>

# include <scribo/layout/internal/smearing/crla.hh>
# include <scribo/layout/internal/smearing/labeling.hh>
# include <scribo/layout/internal/smearing/classification.hh>
# include <scribo/layout/internal/smearing/draw_region.hh>


namespace scribo
{

  namespace layout
  {

    using namespace mln;

    /*! \brief The Smearing layout analysis algorithm.  This algorithm
      is an implementation according to paper \cite ogorman1993pami.

      \param[in] A binary image.

      \return A couple of binary images. The first one contains the
      text components and the second one the non-text components.

      \ingroup grpalgolayout
     */
    template <typename I>
    util::couple<mln_concrete(I), mln_concrete(I)>
    smearing(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    util::couple<mln_concrete(I), mln_concrete(I)>
    smearing(const Image<I>& input_)
    {
      mln_trace("scribo::layout::smearing");

      using namespace internal::smearing;

      typedef mln_value(I) V;
      mlc_is(V,bool)::check();

      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      //-------------------------
      // Initialization
      //-------------------------

      const box2d& domain = input.domain();
      image2d<bool> output_text(domain);
      image2d<bool> output_non_text(domain);
      data::fill(output_text, false);
      data::fill(output_non_text, false);


      //-------------------------
      // Component labeling
      //-------------------------

      std::vector<bbox2d> bbox_list;
      image2d<unsigned> labeled_img(domain);
      data::fill(labeled_img, 0);
      unsigned nlabels = 0;

      internal::smearing::component_labeling(input, nlabels,
					     bbox_list, labeled_img, true);

      //-------------------------
      // Classification
      //-------------------------

      std::vector<bbox2d>::iterator it_begin = bbox_list.begin();
      std::vector<bbox2d>::iterator it_end = bbox_list.end();

      for (; it_begin != it_end; ++it_begin)
      {
	const bbox2d& bbox = (*it_begin);
	const unsigned height = bbox.pmax().row() - bbox.pmin().row();
	unsigned c = 0;

	if (height < 120)
	  c = 1;
	else if (height >= 120 && height < 360)
	  c = 2;
	else
	  c = 3;

	internal::smearing::classify_component(bbox, c, labeled_img);
      }

      //-------------------------
      // First pass
      //-------------------------

      {
	image2d<unsigned> output_CRLA(domain);
	data::fill(output_CRLA, 0);
	const unsigned bound = 1;

	internal::smearing::CRLA(labeled_img, output_CRLA, 360, 15, 360, bound);

	image2d<unsigned> f_labeled_img(domain);
	data::fill(f_labeled_img, 0);
	nlabels = 0;
	bbox_list.clear();

	internal::smearing::component_labeling(output_CRLA, nlabels,
					       bbox_list, f_labeled_img,
					       bound);

	it_begin = bbox_list.begin();
	it_end = bbox_list.end();

	for (; it_begin != it_end; ++it_begin)
	{
	  float brl = 0.0f;
	  float tc = 0.0f;
	  const bbox2d& bbox = (*it_begin);
	  const unsigned width = bbox.pmax().col() - bbox.pmin().col();

	  internal::smearing::compute_region_info(input, f_labeled_img,
						  bbox, brl, tc);

	  const float mbrl = brl / tc;
	  const float mtc = tc / width;

	  if (mbrl >= 0.4f && mbrl <= 15.0f && mtc >= 1.0f && mtc <= 3.8f)
	    internal::smearing::remove_text_region(bbox, input, output_text,
						   labeled_img, f_labeled_img);
	}
      }

      //-------------------------
      // Second pass
      //-------------------------

      {
	image2d<unsigned> output_CRLA(domain);
	data::fill(output_CRLA, 0);
	const unsigned bound = 2;

	internal::smearing::CRLA(labeled_img, output_CRLA, 360, 60, 360, bound);

	image2d<unsigned> s_labeled_img(domain);
	data::fill(s_labeled_img, 0);
	nlabels = 0;
	bbox_list.clear();

	internal::smearing::component_labeling(output_CRLA, nlabels,
					       bbox_list, s_labeled_img,
					       bound);

	it_begin = bbox_list.begin();
	it_end = bbox_list.end();

	for (; it_begin != it_end; ++it_begin)
	{
	  float brl = 0.0f;
	  float tc = 0.0f;
	  const bbox2d& bbox = (*it_begin);
	  const unsigned width = bbox.pmax().col() - bbox.pmin().col();

	  internal::smearing::compute_region_info(input, s_labeled_img,
						  bbox, brl, tc);

	  const float mbrl = brl / tc;
	  const float mtc = tc / width;

	  if (mbrl >= 2.2f && mbrl <= 45.0f && mtc >= 1.2f && mtc <= 9.0f)
	    internal::smearing::draw_region(bbox, input, output_text,
					    s_labeled_img);
	  else
	    internal::smearing::draw_region(bbox, input, output_non_text,
					    s_labeled_img);
	}
      }

      return mln::make::couple(output_text, output_non_text);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_SMEARING_HH
