// Copyright (C) 2006  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_LRDE_EFIGI_REQ_HH
# define OLENA_LRDE_EFIGI_REQ_HH

#include <iostream>
#include <cmath>
#include <map>
#include <oln/basics2d.hh>

#include <oln/lrde/efigi/misc.hh>


namespace oln
{

  namespace efigi
  {


    image2d<unsigned char>
    convert_linear_8(const image2d<float>& input,
		     float min, float max)
    {
      float coef = 256.f / (max - min);
      const int nrows = input.nrows(), ncols = input.ncols();
      image2d<unsigned char> output(nrows, ncols);
      for (int row = 0; row < nrows; ++row)
	for (int col = 0; col < ncols; ++col)
	  {
	    float dest = min + coef * input(row, col);
	    if (dest < 0.)
	      output(row, col) = 0;
	    else if (dest > 255.)
	      output(row, col) = 255;
	    else
	      output(row, col) = unsigned(dest);
	  }
      return output;
    }


    image2d<unsigned short>
    convert_linear_16(const image2d<float>& input,
		      float min, float max)
    {
      float coef = 65536.f / (max - min);
      const int nrows = input.nrows(), ncols = input.ncols();
      image2d<unsigned short> output(nrows, ncols);
      for (int row = 0; row < nrows; ++row)
	for (int col = 0; col < ncols; ++col)
	  {
	    float dest = min + coef * input(row, col);
	    if (dest < 0.)
	      output(row, col) = 0;
	    else if (dest > 65535.)
	      output(row, col) = 65535;
	    else
	      output(row, col) = unsigned(dest);
	  }
      return output;
    }


    float slog(float f, float sigma)
    {
      return f < 0 ? - std::log(1. - f / sigma) : std::log(1. + f / sigma);
    }


    image2d<unsigned char>
    convert_log(const image2d<float>& input,
		float min, float max,
		float sigma)
    {
      assert(sigma > 0);
      assert(max > min);

      float orig = slog(min, sigma);
      float L = (slog(max, sigma) - orig) / 255.9999f;

      unsigned too_low = 0, too_high = 0;
      const int nrows = input.nrows(), ncols = input.ncols();
      image2d<unsigned char> output(nrows, ncols);
      for (int row = 0; row < nrows; ++row)
	for (int col = 0; col < ncols; ++col)
	  {
	    float f = input(row, col), f_dyn = 0;
	    if (f >= max)
	      output(row, col) = 255;
	    else
	      if (f <= min)
		output(row, col) = 0;
	      else
		// min < f < max
		{
		  f_dyn = (slog(f, sigma) - orig) / L;
		  if (f_dyn < 0)
		    ++too_low;
		  if (f_dyn > 255)
		    ++too_high;
		}
	    output(row, col) = unsigned(f_dyn);
	  }    

      if (too_low || too_high)
	std::cerr << "pb in efigi::convert_log: "
		  << too_low << ' ' << too_high << std::endl;

      return output;
    }


    image2d<unsigned char>
    equalize(const image2d<float>& input)
    {
      std::vector<pix> v = efigi::sort(input);
      const int nrows = input.nrows(), ncols = input.ncols();
      image2d<unsigned char> output(nrows, ncols);

      unsigned n = unsigned((nrows * ncols) / 256.f + 1.f);
      unsigned l = 0;
      for (;;)
	{
	  for (unsigned i = 0; i < n; ++i)
	    {
	      unsigned j = l * n + i;
	      if (j >= v.size())
		return output;
	      pix& p = v[l * n + i];
	      output(p.row, p.col) = l;
	    }
	  ++l;
	}
      return output;
    }

    
    image2d<bool>
    lab2bin(const image2d<unsigned char>& input)
    {
      const int nrows = input.nrows(), ncols = input.ncols();
      image2d<bool> output(nrows, ncols);
      for (int row = 0; row < nrows; ++row)
	for (int col = 0; col < ncols; ++col)
	  output(row, col) = (input(row, col) == 255);
      return output;
    }


  } // end of namespace oln::efigi

} // end of namespace oln


#endif // ! OLENA_LRDE_EFIGI_REQ_HH
