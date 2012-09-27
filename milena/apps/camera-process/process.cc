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

#include <mln/core/image/image2d.hh>
#include <mln/opt/at.hh>

#include <mln/core/image/vmorph/fun_image.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>
#include <mln/literal/black.hh>
#include <mln/literal/white.hh>

#include <mln/math/min.hh>

#include <mln/pw/all.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/morpho/dilation.hh>
#include <mln/morpho/gradient.hh>
#include <mln/morpho/opening/area.hh>

#include "process.hh"


// FIXME: Move this into the library.
/// Functor returning the minimal component of an mln::value::rgb<n> value.
template <unsigned n>
struct min_rgb_comp : mln::Function_v2v< min_rgb_comp<n> >
{
  typedef mln::value::int_u<n> result;

  result operator()(const mln::value::rgb<n>& c) const
  {
    return mln::math::min(mln::math::min(c.red(), c.green()), c.blue());
  }
};


/* FIXME: Hide OpenCV-Milena conversion operations; or better, wrap
   the `Iplimage' data structure in a Milena image so that it can be
   processed directly.  */

void process(IplImage* cv_ima)
{
  using namespace mln;
  using mln::value::int_u8;
  using mln::value::rgb8;

  char* data = cv_ima->imageData;
  int height = cv_ima->height;
  int width = cv_ima->width;
  int widthstep = cv_ima->widthStep;
  int nchannels = cv_ima->nChannels;

  // Convert `cv_ima' into a Milena image.
  image2d<rgb8> mln_ima(height, width);
  for (int row = 0; row < height; ++row)
    for (int col = 0; col < width; ++col)
      for (int chan = 0; chan < nchannels; ++chan)
	opt::at(mln_ima, row, col).comp(chan) =
	  (unsigned char) data[row * widthstep + col * nchannels + chan];

  // Detect white objects having a minimal area.
  image2d<bool> bin;
  initialize(bin, mln_ima);
  data::fill(bin, false);
  int_u8 threshold = 160;
  data::fill((bin | (pw::value(min_rgb_comp<8>() << mln_ima)
		     >= pw::cst(threshold))).rw(),
	     true);
  // For a lack of a better characterization.
  unsigned area = 15000;
  image2d<bool> objects = morpho::opening::area(bin, c4(), area);
  // Find contours.
  image2d<bool> outline = morpho::gradient_external(objects, win_c4p());
  // Enlarge them.
  image2d<bool> contours = morpho::dilation(outline, win_c4p());

  // Alter the original OpenCV image `cv_ima'.
  for (int row = 0; row < height; ++row)
    for (int col = 0; col < width; ++col)
	if (char(opt::at(contours, row, col)))
	  {
	    /* Paint the pixel in red.

	       In the initial test environment, the frame was encoded
	       as a BGR image.  */
	    int pos = row * widthstep + col * nchannels;
	    data[pos + 0] = mln_min(int_u8);  // Channel 0: Blue.
	    data[pos + 1] = mln_min(int_u8);  // Channel 1: Green.
	    data[pos + 2] = mln_max(int_u8);  // Channel 2: Red.
	  }
}
