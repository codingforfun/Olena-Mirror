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

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_COMPONENT_SIZE_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_COMPONENT_SIZE_HH

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

	void
	compute_comp_size(const std::vector<bbox2d>& bbox_list,
			  std::vector<bbox2d>& small_comp,
			  std::vector<bbox2d>& big_comp,
			  const float max_size_factor);

# ifndef MLN_INCLUDE_ONLY

	//-------------------------
	// Compute components size
	//-------------------------

	inline
	void
	compute_comp_size(const std::vector<bbox2d>& bbox_list,
			  std::vector<bbox2d>& small_comp,
			  std::vector<bbox2d>& big_comp,
			  const float max_size_factor)
	{
	  mln_trace("scribo::layout::internal::docstrum::compute_comp_size");

	  std::vector<bbox2d>::const_iterator it_begin = bbox_list.begin();
	  std::vector<bbox2d>::const_iterator it_end = bbox_list.end();
	  std::map<int, int> size_hist;
	  std::vector<int> smoothed_size_hist;
	  std::vector<int> sizes;
	  sizes.reserve(bbox_list.size());
	  int max_value = 0;
	  int comp_size = 0;
	  int window = 1;

	  // Compute the histogram and find the dominant characters size
	  for (; it_begin != it_end; ++it_begin)
	  {
	    const bbox2d& box = (*it_begin);
	    const unsigned height = box.pmax().row() - box.pmin().row();
	    const unsigned width = box.pmax().col() - box.pmin().col();
	    const int size = round(sqrt(width * height));

	    ++size_hist[size];

	    if (size > max_value)
	      max_value = size;

	    sizes.push_back(size);
	  }

	  const int nelements = max_value + 1;
	  smoothed_size_hist.resize(nelements);

	  const int factor = 2 * window + 1;
	  max_value = 0;

	  // Smoothed distribution
	  for (int i = 0; i < nelements; ++i)
	  {
	    int count = 0;

	    for (int j = i - window; j <= i + window; ++j)
	    {
	      if (j < 0 || j >= nelements)
		continue;

	      if (size_hist.find(j) != size_hist.end())
		count += size_hist[j];
	    }

	    smoothed_size_hist[i] = count / factor;

	    if (smoothed_size_hist[i] > max_value)
	    {
	      comp_size = i;
	      max_value = smoothed_size_hist[i];
	    }
	  }

	  // std::map<unsigned, unsigned> size_hist;
	  // unsigned max_value = 0;
	  // unsigned comp_size = 0;
	  // std::vector<unsigned> sizes;
	  // sizes.reserve(bbox_list.size());

	  // // Compute the histogram and find the dominant characters size

	  // for (; it_begin != it_end; ++it_begin)
	  // {
	  //   const bbox2d& box = (*it_begin);
	  //   const unsigned height = box.pmax().row() - box.pmin().row();
	  //   const unsigned width = box.pmax().col() - box.pmin().col();
	  //   const unsigned size = sqrt(width * height);
	  //   const unsigned hist_value = ++size_hist[size];

	  //   sizes.push_back(size);

	  //   if (hist_value > max_value)
	  //   {
	  //     comp_size = size;
	  //     max_value = hist_value;
	  //   }
	  // }

	  // Separate components into two classes
	  it_begin = bbox_list.begin();
	  std::vector<int>::iterator it_size_begin = sizes.begin();
	  const float dominant_size = comp_size * max_size_factor;

	  for (; it_begin != it_end; ++it_begin, ++it_size_begin)
	  {
	    const bbox2d& box = (*it_begin);
	    const float size = (*it_size_begin);

	    if (size < dominant_size)
	      small_comp.push_back(box);
	    else
	      big_comp.push_back(box);
	  }
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_COMPONENT_SIZE_HH
