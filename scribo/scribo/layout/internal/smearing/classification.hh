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

#ifndef SCRIBO_LAYOUT_INTERNAL_SMEARING_CLASSIFICATION_HH
# define SCRIBO_LAYOUT_INTERNAL_SMEARING_CLASSIFICATION_HH

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


	template<typename V>
	void classify_component(const bbox2d& bbox,
				V label,
				image2d<V>& labeled_img);

	template<typename V>
	void compute_region_info(const image2d<bool>& input,
				 const image2d<V>& labeled_img,
				 const bbox2d& bbox,
				 float& brl,
				 float& tc);


# ifndef MLN_INCLUDE_ONLY

	//---------------------------
	// Classify component
	//---------------------------

	template<typename V>
	inline
	void classify_component(const bbox2d& bbox,
				V label,
				image2d<V>& labeled_img)
	{
	  const unsigned id = bbox.bbox_number();
	  const point2d& pmin = bbox.pmin();
	  const point2d& pmax = bbox.pmax();
	  box2d b(pmin, pmax);
	  mln_piter_(box2d) p(b);

	  for_all(p)
	  {
	    V& pixel = labeled_img(p);
	    if (pixel == id)
	      pixel = label;
	  }
	}

	//---------------------------
	// Compute region information
	//---------------------------

	template<typename V>
	inline
	void compute_region_info(const image2d<bool>& input,
				 const image2d<V>& labeled_img,
				 const bbox2d& bbox,
				 float& brl,
				 float& tc)
	{
	  const unsigned id = bbox.bbox_number();
	  const point2d& pmin = bbox.pmin();
	  const point2d& pmax = bbox.pmax();

	  for (int i = pmin.row(); i <= pmax.row(); ++i)
	  {
	    for (int j = pmin.col(); j <= pmax.col(); ++j)
	    {
	      const unsigned label_pixel = labeled_img.at_(i, j);

	      if (label_pixel == id)
	      {
		const bool pixel = input.at_(i, j);

		if (pixel)
		{
		  if (j > 0 && input.at_(i, j - 1) == 0)
		    ++tc;

		  int k = j + 1;
		  for (; input.at_(i, k); ++k);
		  brl += (k - j);

		  j = k;
		}
	      }
	    }
	  }

	  if (tc == 0.0f)
	    tc = 1.0f;
	}


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::smearing

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_SMEARING_CLASSIFICATION_HH
