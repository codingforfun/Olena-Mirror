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

#ifndef MLN_DETECTION_CORNER_HARRIS_HH
# define MLN_DETECTION_CORNER_HARRIS_HH

#include <mln/core/site_set/p_array.hh>
//#include <mln/win/rectangle2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/linear/ch_convolve.hh>
#include <mln/linear/convolve.hh>
#include <mln/linear/gaussian.hh>
#include <mln/arith/times.hh>
#include <mln/core/alias/w_window2d_int.hh>

//#include <mln/core/alias/window2d.hh>
#include <mln/detection/corner/harris_criterion.hh>

/// \file
///
/// \brief Implementation of Harris corner detection.


namespace mln
{
  namespace detection
  {
    namespace corner 
    {
      /**
	 Computes Harris corner detection on \p in_image

	 \param image_in_ The input image
	 \param k the k parameter in Harris algorithm
	 \param sigma the standard deviation of the gaussian used in
	 Harris algorithm
	 \return The list of points detected as corner

	 \author J. Fabrizio
      */
      template<typename I>
      p_array<mln_site(I)>
      harris(const Image<I>& in_image_, float k, float sigma);

      /**
	 Computes Harris corner detection on \p in_image with the
	 standard deviation of the guassian sets to 0.34

	 \param image_in_ The input image
	 \param k the k parameter in Harris algorithm
	 \return The list of points detected as corner

	 \author J. Fabrizio
      */
      template<typename I>
      p_array<mln_site(I)>
      harris(const Image<I>& in_image_, float k);

      /**
	 Computes Harris corner detection on \p in_image with the k
	 parameter sets to 0.04 and the standard deviation of the
	 guassian sets to 0.34.

	 \param image_in_ The input image
	 \return The list of points detected as corner

	 \author J. Fabrizio
      */
      template<typename I>
      p_array<mln_site(I)>
      harris(const Image<I>& in_image_);


# ifndef MLN_INCLUDE_ONLY


      namespace internal 
      {
      /**
	 \iternal Computes local max to find corner in an image

	 \param image_in_ The result of harris_criterion
	 \return The list of local min

	 \author J. Fabrizio
      */
	template<typename I>
	p_array<mln_site(I)>
	harris_local_max(const Image<I>& in_image_)
	{
	  trace::exiting("detection::corner::harris_local_max");
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
	  trace::exiting("detection::corner::harris_local_max");
	  return result;
	}

      } // end of namespace mln::detection::corner::internal

      template<typename I>
      p_array<mln_site(I)>
      harris(const Image<I>& in_image_, float k, float sigma) {
	trace::entering("detection::corner::harris");
	mln_ch_convolve(I, w_window2d_int) criterion_image;
	criterion_image = harris_criterion(in_image_, k, sigma);
	trace::exiting("detection::corner::harris");
	return internal::harris_local_max(criterion_image);
      }

      template<typename I>
      p_array<mln_site(I)>
      harris(const Image<I>& in_image_, float k) {
	return harris(in_image_, k, 0.34);
      }

      template<typename I>
      p_array<mln_site(I)>
      harris(const Image<I>& in_image_) {
	return harris(in_image_, 0.04, 0.34);
      }



# endif // ! MLN_INCLUDE_ONLY

    } //  end of namespace mln::detection::corner
  } //  end of namespace mln::detection


} // end of namespace mln

#endif // ! MLN_DETECTION_CORNER_HARRIS_HH
