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

#ifndef MLN_DETECTION_CORNER_ACHARD_CRITERION_HH
# define MLN_DETECTION_CORNER_ACHARD_CRITERION_HH


//#include <mln/io/essential.hh>
//#include <mln/data/stretch.hh>
//#include <mln/data/convert.hh>
//#include <mln/value/graylevel.hh>
# include <mln/core/alias/w_window2d_int.hh>

/// \file
///
/// \brief Implementation of Achard, Bigorgne and Devars corner detection.


namespace mln
{
  namespace detection
  {
    namespace corner 
    {
      /**
	 Computes Achard, Bigorgne and Devars corner detection criterion on \p in_image.

	 \param image_in_ The input image.

	 \return Achard criterion computed for evey pixel.

	 \author J. Fabrizio
      */
      template<typename I>
      mln_ch_convolve(I, w_window2d_int)
	achard_criterion(const Image<I>& in_image_);

# ifndef MLN_INCLUDE_ONLY

      template<typename I>
      mln_ch_convolve(I, w_window2d_int)
	achard_criterion(const Image<I>& in_image_)
	{
	  trace::entering("detection::corner::achard_criterion");

	  const I& in_image = exact(in_image_);
	  typedef mln_ch_convolve(I, w_window2d_int) O;

	  typedef dpoint2d D;
	  w_window2d_int dx_grad;
	  dx_grad
	    .insert(-1, D(0, -1))
	    .insert(+1, D(0, +1));
	  w_window2d_int dy_grad;
	  dy_grad
	    .insert(-1, D(-1, 0))
	    .insert(+1, D(+1, 0));

	  O dx_image=linear::convolve(in_image, dx_grad);
	  O dy_image=linear::convolve(in_image, dy_grad);

	  w_window2d_int mask_M;
	  mask_M
	    .insert(+1, D( 0, -1))
	    .insert(+1, D( 0, +1))
	    .insert(+1, D(+1, -1))
	    .insert(+1, D(+1, +1))
	    .insert(+1, D(-1, -1))
	    .insert(+1, D(-1, +1))
	    .insert(+1, D(-1,  0))
	    .insert(+1, D(+1,  0));

	  O dxxj_image=linear::convolve(dx_image*dx_image, mask_M);
	  O dyyj_image=linear::convolve(dy_image*dy_image, mask_M);
	  O dxyj_image=linear::convolve(dx_image*dy_image, mask_M);

	  O achard_criterion(in_image.domain());

	  typedef mln_value(O) V;
	  mln_piter(I) p(in_image.domain());
	  for_all(p) {
	    V ix = dx_image(p);
	    V iy = dy_image(p);
	    V ixxj = dxxj_image(p);
	    V iyyj = dyyj_image(p);
	    V ixyj = dxyj_image(p);
	    V grad_magnitude = ix*ix+iy*iy;
	    achard_criterion(p)=ix*ix*iyyj+iy*iy*ixxj-2*ix*iy*ixyj;
	    if (grad_magnitude>1) achard_criterion(p)/=grad_magnitude;
	  }

	  //	  I output = data::stretch(mln_value(I)(), achard_criterion);
	  //	  io::pgm::save(output, "achard-criterion-result.pgm");

	  trace::exiting("detection::corner::achard_criterion");
	  return achard_criterion;
	}

# endif // ! MLN_INCLUDE_ONLY

    } //  end of namespace mln::detection::corner

  } //  end of namespace mln::detection

} // end of namespace mln

#endif // ! MLN_DETECTION_CORNER_ACHARD_CRITERION_HH
