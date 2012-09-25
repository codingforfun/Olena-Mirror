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

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/fun/v2v/rgb_to_int_u.hh>

#include "process.hh"


/* FIXME: Hide OpenCV-Milena conversion operations; or better, wrap
   the `Iplimage' data structure in a Milena image so that it can be
   processed directly.  */

void process(IplImage* cv_ima)
{
  using namespace mln;

  // Convert `cv_ima' into a Milena image...
  image2d<value::rgb8> mln_ima(cv_ima->height, cv_ima->width);
  for (int row = 0; row < cv_ima->height; ++row)
    for (int col = 0; col < cv_ima->width; ++col)
      for (int chan = 0; chan < cv_ima->nChannels; ++chan)
	{
	  opt::at(mln_ima, row, col).comp(chan) =
	    (unsigned char) cv_ima->imageData[row * cv_ima->widthStep
					     + col * cv_ima->nChannels
					     + chan];
	}

  // ...do something with it...
  mln_piter_(image2d<value::rgb8>) p(mln_ima.domain());
  for_all(p)
    convert::from_to(fun::v2v::rgb_to_int_u<8>()(mln_ima(p)), mln_ima(p));

  // ...and paste `mln_ima' back into the OpenCV image `cv_ima'.
  for (int row = 0; row < cv_ima->height; ++row)
    for (int col = 0; col < cv_ima->width; ++col)
      for (int chan = 0; chan < cv_ima->nChannels; ++chan)
	{
	  cv_ima->imageData[row * cv_ima->widthStep
			   + col * cv_ima->nChannels
			   + chan] =
	    (char) opt::at(mln_ima, row, col).comp(chan);
	}
}
