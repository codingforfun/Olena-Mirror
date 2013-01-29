// Copyright (C) 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_OPTICAL_FLOW_LK_HH_
# define MLN_OPTICAL_FLOW_LK_HH_

/// \file
///
/// \brief Implementation of Lucas-Kanade algorithm.

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

    /// \brief Compute Lucas-Kanade algorithm
    ///
    /// @param ima1 First frame
    /// @param ima2 Second frame
    /// @param mask The mask on which we want to compute the optical flow
    /// @param winsize size of the window used
    /// @param weigths The weights inside the window
    /// @param optical_flow the result

    template <typename I>
    void
    LucasKanade (I& ima1,
		 I& ima2,
		 mln::image2d<bool>& mask,
		 unsigned winsize,
		 mln::image2d<double>& weights,
		 mln::image2d <mln::vec2d_f>& optical_flow);

# ifndef MLN_INCLUDE_ONLY

    //If true, of (optical flow) coarser but computation faster.
# define PER_BLOCK (false)

    template <typename I>
    void
    LucasKanade (I& ima1,
		 I& ima2,
		 mln::image2d<bool>& mask,
		 unsigned winsize,
		 mln::image2d<double>& weights,
		 mln::image2d <mln::vec2d_f>& optical_flow)
    {
      mln::win::rectangle2d win (winsize, winsize);

      typedef mln::image_if<I, mln::pw::value_ <mln::image2d<bool> > > II;

      II ima = (ima1 | mln::pw::value (mask));

      mln_piter (II) p (ima.domain ());

      mln_qiter_ (mln::win::rectangle2d) q (win, p);
      for_all (p)
      {

#if PER_BLOCK
	if (p.col() % winsize == winsize / 2  && p.row () % winsize == winsize / 2)
	  {
#endif
	    double A[winsize * winsize][2];
	    double B[winsize * winsize];
	    double W[winsize * winsize];
	    unsigned nb_comp = 0;

	    //filling matrices
	    for_all (q)
	    {
	      if (ima1.has(q))
		{
		  //Ex
		  A[nb_comp][0] = ima1 (q + mln::dpoint2d (0, 1)) - ima1 (q)
		    + ima2 (q + mln::dpoint2d (0, 1)) - ima2 (q);
		  //Ey
		  A[nb_comp][1] = ima1 (q + mln::dpoint2d (1, 0)) - ima1 (q)
		    + ima2 (q + mln::dpoint2d (1, 0)) - ima2 (q);

		  //Et
		  B[nb_comp] = - (ima2 (q) - ima1 (q));
		  //weights
		  W[nb_comp] = weights (q);
		  ++nb_comp;
		}
	    }

	    //Computing flow

	    //Filling the structure tensor (transpose(A) * A)
	    int struct_tensor[2][2] = {{0, 0}, {0, 0}};
	    for (unsigned i = 0; i < nb_comp; ++i)
	      {
		struct_tensor [0][0] += W[i] * A[i][0] * A[i][0];
		struct_tensor [1][0] += W[i] * A[i][0] * A[i][1];
		struct_tensor [0][1] += W[i] * A[i][0] * A[i][1];
		struct_tensor [1][1] += W[i] * A[i][1] * A[i][1];
	      }

	    //Computing the inverse of the structure tensor.
	    double struct_tensor_inv[2][2] = {{0, 0}, {0, 0}};
	    double det = struct_tensor[0][0] * struct_tensor[1][1]
	      - struct_tensor[1][0] * struct_tensor[0][1];

	    double Vx = 0;
	    double Vy = 0;
	    if (det * det > 0.000001)
	      { 
		struct_tensor_inv[0][0] = struct_tensor[1][1] / det;
		struct_tensor_inv[0][1] = -struct_tensor[0][1] / det;
		struct_tensor_inv[1][0] = -struct_tensor[1][0] / det;
		struct_tensor_inv[1][1] = struct_tensor[0][0] / det;

		int transAtimesB[2] = {0, 0};
		for (unsigned i = 0; i < nb_comp; ++i)
		  {
		    transAtimesB[0] += W[i] * A[i][0] * B[i];
		    transAtimesB[1] += W[i] * A[i][1] * B[i];
		  }

		Vx = struct_tensor_inv[0][0] * transAtimesB[0]
		  + struct_tensor_inv[0][1] * transAtimesB[1];
		Vy = struct_tensor_inv[1][0] * transAtimesB[0]
		  + struct_tensor_inv[1][1] * transAtimesB[1];
	      }
#if PER_BLOCK
	    for_all (q)
	    {
	      optical_flow (q)[0] = Vx;
	      optical_flow (q)[1] = Vy;
	    }
	  }
#else
	optical_flow (p)[0] = Vx;
	optical_flow (p)[1] = Vy;
#endif
      }


    }

# endif // ! MLN_INCLUDE_ONLY

  } //end of namespace optical_flow

} //end of namespace mln


#endif // ! MLN_OPTICAL_FLOW_LK_HH_
