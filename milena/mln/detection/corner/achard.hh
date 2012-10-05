// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DETECTION_CORNER_ACHARD_HH
# define MLN_DETECTION_CORNER_ACHARD_HH

#include <mln/core/site_set/p_array.hh>
//#include <mln/win/rectangle2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/linear/ch_convolve.hh>
#include <mln/linear/convolve.hh>
#include <mln/linear/gaussian.hh>
#include <mln/arith/times.hh>
#include <mln/core/alias/w_window2d_int.hh>

//#include <mln/core/alias/window2d.hh>
#include <mln/detection/corner/achard_criterion.hh>

/// \file
///
/// \brief Implementation of Achard corner detection.


namespace mln
{
  namespace detection
  {
    namespace corner 
    {
      /**
	 Computes Achard corner detection on \p in_image

	 \param image_in_ The input image
	 \return The list of points detected as corner

	 \author J. Fabrizio
      */
      template<typename I>
      p_array<mln_site(I)>
      achard(const Image<I>& in_image_);


# ifndef MLN_INCLUDE_ONLY


      namespace internal 
      {
      /**
	 \iternal Computes local max to find corner in an image

	 \param image_in_ The result of achard_criterion
	 \return The list of local min

	 \author J. Fabrizio
      */
	template<typename I>
	p_array<mln_site(I)>
	achard_local_max(const Image<I>& in_image_)
	{
	  trace::exiting("detection::corner::achard_local_max");
	  const I& in_image = exact(in_image_);
	  p_array<mln_site(I)> result;
	  mln_piter(I) p(in_image.domain());
	  for_all(p)
	  {
	    bool max = true;
	    mln_niter(neighb2d) n(c8(), p);
	    for_all(n) {
	      if (in_image(p)<=in_image(n))
		{
		  max = false;
		  break;
		}
	    }
	    if (max) {
	      result.append(p);
	    }
	  }
	  trace::exiting("detection::corner::achard_local_max");
	  return result;
	}

      } // end of namespace mln::detection::corner::internal

      template<typename I>
      p_array<mln_site(I)>
      achard(const Image<I>& in_image_) {
	trace::entering("detection::corner::achard");
	mln_ch_convolve(I, w_window2d_int) criterion_image;
	criterion_image = achard_criterion(in_image_);
	p_array<mln_site(I)> result = internal::achard_local_max(criterion_image);
	trace::exiting("detection::corner::achard");
	return result;
      }



# endif // ! MLN_INCLUDE_ONLY

    } //  end of namespace mln::detection::corner
  } //  end of namespace mln::detection


} // end of namespace mln

#endif // ! MLN_DETECTION_CORNER_ACHARD_HH
