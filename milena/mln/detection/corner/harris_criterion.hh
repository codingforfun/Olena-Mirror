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

#ifndef MLN_DETECTION_CORNER_HARRIS_CRITERION_HH
# define MLN_DETECTION_CORNER_HARRIS_CRITERION_HH


//#include <mln/io/essential.hh>
//#include <mln/data/stretch.hh>
//#include <mln/data/convert.hh>
//#include <mln/value/graylevel.hh>
# include <mln/core/alias/w_window2d_int.hh>

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
	 Computes Harris corner detection criterion on \p in_image.

	 \param image_in_ The input image.
	 \param k The k parameter in harris algorithm (use 0.04 if you
	 do not know what to do).
	 \param sigma The standard deviation of the gaussian used harris algorithm.

	 \return Harris criterion computed for evey pixel.

	 \author J. Fabrizio
      */
      template<typename I>
      mln_ch_convolve(I, w_window2d_int)
	harris_criterion(const Image<I>& in_image_, float k, float sigma);

# ifndef MLN_INCLUDE_ONLY


      template<typename I>
      mln_ch_convolve(I, w_window2d_int)
	harris_criterion(const Image<I>& in_image_, float k, float sigma)
	{
	  trace::entering("detection::corner::haris_criterion");

	  const I& in_image = exact(in_image_);
	  p_array<mln_site(I)> result;
	  typedef mln_ch_convolve(I, w_window2d_int) O;
	  O dx_image;
	  O dy_image;
	  O dxx_image;
	  O dxy_image;
	  O dyy_image;
	  O harris_criterion(in_image.domain());

	  typedef dpoint2d D;
	  w_window2d_int dx_grad;
	  dx_grad
	    .insert(-1, D(0, -1))
	    .insert(+1, D(0, +1));
	  w_window2d_int dy_grad;
	  dy_grad
	    .insert(-1, D(-1, 0))
	    .insert(+1, D(+1, 0));

	  dx_image=linear::convolve(in_image, dx_grad);
	  dy_image=linear::convolve(in_image, dy_grad);

	  dxx_image=linear::gaussian(dx_image*dx_image, sigma);
	  dxy_image=linear::gaussian(dx_image*dy_image, sigma);
	  dyy_image=linear::gaussian(dy_image*dy_image, sigma);

	  typedef mln_value(O) V;
	  mln_piter(I) p(in_image.domain());
	  for_all(p) {
	    V a = dxx_image(p);
	    V b = dyy_image(p);
	    V c = dxy_image(p);
	    harris_criterion(p)=(a*b-c*c)-k*(a+b)*(a+b);
	  }

	  //	  I output = data::stretch(mln_value(I)(), harris_criterion);
	  //	  io::pgm::save(output, "harris-criterion-result.pgm");

	  trace::exiting("detection::corner::harris_criterion");
	  return harris_criterion;
	}

# endif // ! MLN_INCLUDE_ONLY

    } //  end of namespace mln::detection::corner

  } //  end of namespace mln::detection

} // end of namespace mln

#endif // ! MLN_DETECTION_CORNER_HARRIS_CRITERION_HH
