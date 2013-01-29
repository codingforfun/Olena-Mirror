// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_OPTICAL_FLOW_HS_HH_
# define MLN_OPTICAL_FLOW_HS_HH_

/// \file
///
/// \brief Implementation of Horn-Schunck algorithm.

# include <mln/core/alias/box2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/core/alias/vec2d.hh>
# include <mln/core/concept/image.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/pw/value.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include <cmath>

namespace mln
{

  namespace optical_flow
  {

    /// \brief Compute Hors-Schunck algorithm
    ///
    /// @param frame1 The first frame
    /// @param frame2 The second frame
    /// @param mask The mask on which we want to compute the optical flow
    /// @param optical_flow The result
    /// @param alpha_sq Numeric to balance terms
    /// @param niter Maximum number of iterations
    /// @param stop_criterion Stop the algorithm if not enough changes between
    ///        two iterations.

    template <typename I>
    void
    HornSchunck (I& frame1, I& frame2, mln::image2d<bool>& mask,
		 mln::image2d <mln::vec2d_f>& optical_flow,
		 float alpha_sq = 49, unsigned niter = 170,
		 double stop_criterion = 0.001);


#ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void
    HornSchunck (I& frame1, I& frame2, mln::image2d<bool>& mask,
		 mln::image2d <mln::vec2d_f>& optical_flow,
		 float alpha_sq, unsigned niter,
		 double stop_criterion)
    {
      mln_assertion (frame1.domain () == optical_flow.domain ());
      mln_assertion (frame2.domain () == optical_flow.domain ());

      //Used to store the value at eache given iteration.
      mln::image2d <mln::vec2d_f> tmp_of (optical_flow.domain ());
      bool stop = false;

      typedef mln::image_if<I, mln::pw::value_ <mln::image2d<bool> > > II;

      II ima = (frame1 | mln::pw::value (mask));

      mln_piter (II) p (ima.domain ());

      for (unsigned i = 0; i < niter && !stop; ++i)
	{
	  double diff_sum = 0;//Sum used to eventually stop the computation.
	  for_all (p)
	  {
	    //Computation of the average of the optical flow vector.
	    mln::vec2d_f ave;

	    ave = (tmp_of (p + mln::dpoint2d (-1, 0))
		   + tmp_of (p + mln::dpoint2d (0, 1))
		   + tmp_of (p + mln::dpoint2d (1, 0))
		   + tmp_of (p + mln::dpoint2d (0, -1))) / 6
	      + (tmp_of (p + mln::dpoint2d (-1, -1))
		 + tmp_of (p + mln::dpoint2d (-1, 1))
		 + tmp_of (p + mln::dpoint2d (1, 1))
		 + tmp_of (p + mln::dpoint2d (1, -1))) / 12;
	  

	    //Computation of the partial derivates.
	    float Ex = 0, Ey = 0, Et = 0;

	    Ex = frame1 (p + mln::dpoint2d (0, 1)) - frame1 (p)
	      + frame1 (p + mln::dpoint2d (1, 1)) - frame1 (p + mln::dpoint2d (1, 0))
	      + frame2 (p + mln::dpoint2d (0, 1)) - frame2 (p)
	      + frame2 (p + mln::dpoint2d (1, 1)) - frame2 (p + mln::dpoint2d (1, 0));
	    Ex /= 4;

	    Ey = frame1 (p + mln::dpoint2d (1, 0)) - frame1 (p)
	      + frame1 (p + mln::dpoint2d (1, 1)) - frame1 (p + mln::dpoint2d (0, 1))
	      + frame2 (p + mln::dpoint2d (1, 0)) - frame2 (p)
	      + frame2 (p + mln::dpoint2d (1, 1)) - frame2 (p + mln::dpoint2d (0, 1));
	    Ey /= 4;

	    Et = frame2 (p) - frame1 (p)
	      + frame2 (p + mln::dpoint2d (1, 0)) - frame1 (p + mln::dpoint2d (1, 0))
	      + frame2 (p + mln::dpoint2d (0, 1)) - frame1 (p + mln::dpoint2d (0, 1))
	      + frame2 (p + mln::dpoint2d (1, 1)) - frame1 (p + mln::dpoint2d (1, 1));
	    Et /= 4;


	    optical_flow (p)[0] = ave[0] - Ex * (Ex * ave[0] + Ey * ave[1] + Et)
	      / (alpha_sq + Ex * Ex + Ey * Ey);
	    optical_flow (p)[1] = ave[1] - Ey * (Ex * ave[0] + Ey * ave[1] + Et)
	      / (alpha_sq + Ex * Ex + Ey * Ey);

	    diff_sum += ((optical_flow (p)[0] - tmp_of (p)[0]) *
			 (optical_flow (p)[0] - tmp_of (p)[0])) +
	      ((optical_flow (p)[1] - tmp_of (p)[1]) *
	       (optical_flow (p)[1] - tmp_of (p)[1]));
	  }

	  for_all (p)
	    tmp_of (p) = optical_flow (p);

	  diff_sum /= (optical_flow.nrows () * optical_flow.ncols ());

	  if (diff_sum < stop_criterion * stop_criterion)
	    stop = true;
	}
    }

# endif // ! MLN_INCLUDE_ONLY

  } //end of namespace optical_flow

} //end of namespace mln


#endif // ! MLN_OPTICAL_FLOW_HS_HH_
