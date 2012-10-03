// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DETECTION_CORNER_MORAVEC_CRITERION_HH
# define MLN_DETECTION_CORNER_MORAVEC_CRITERION_HH

#include <mln/extension/adjust_duplicate.hh>
//#include <mln/metal/is.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>

/// \file
///
/// \brief Implementation of Moravec corner detection.


namespace mln
{
  namespace detection
  {
    namespace corner 
    {
      /**
	 Computes the criterion necessary to Moravec corner detection. 

	 \param in_image_ the orginal image

	 \param win_ The window 

	 \param nbh_ All possible positions of the window 

	 \return Scores computed by Moravec criterion

	 \author J. Fabrizio
      */
      template<typename I, typename W, typename N>
      mln_ch_value(I, unsigned short)
	moravec_criterion(const Image<I>& in_image_, const Window<W>& win_, const Neighborhood<N>& nbh_);

# ifndef MLN_INCLUDE_ONLY

      template<typename I, typename W, typename N>
      mln_ch_value(I, unsigned short)
	moravec_criterion(const Image<I>& in_image_, const Window<W>& win_, const Neighborhood<N>& nbh_)
      {
	trace::entering("detection::corner::moravec_criterion");

	//mlc_is(W, mln::win::rectangle2d)::check();
	//mlc_is(nbh, win::rectangle2d)::check();

	const I& in_image = exact(in_image_);
	const W& win = exact(win_);
	const N& nbh = exact(nbh_);

	mln_precondition(in_image.is_valid());
	// ...

	mln_ch_value(I, unsigned short) out_image(in_image.domain());

	extension::adjust_duplicate(in_image, mln::geom::delta(win)+mln::geom::delta(nbh));

	mln_piter(I) p(in_image.domain());
	mln_qiter(W) q(win, p);
	mln_niter(N) n(nbh, p);
	mln_qiter(W) q2(win, n);
	for_all(p)
	{
	  unsigned short result=mln_max( unsigned short );
	  for_all(n)
	  {
	    unsigned short r=0;
	    for_all_2(q, q2)
	      r+=(in_image(q)-in_image(q2))*(in_image(q)-in_image(q2));
	    if (r<result) result = r;
	  }
	  out_image(p) = result;
	}

	trace::exiting("detection::corner::moravec_criterion");
	return out_image;
      }

# endif // ! MLN_INCLUDE_ONLY

    } //  end of namespace mln::detection::corner
  } //  end of namespace mln::detetection


} // end of namespace mln

#endif // ! MLN_DETECTION_CORNER_MORAVEC_CRITERION_HH
