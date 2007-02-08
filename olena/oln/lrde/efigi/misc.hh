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

#ifndef OLENA_LRDE_EFIGI_MISC_HH
# define OLENA_LRDE_EFIGI_MISC_HH

#include <cmath>
#include <cstdlib>

#include <vector>
#include <algorithm>
#include <fstream>

#include <oln/basics2d.hh>


namespace oln
{

  namespace efigi
  {

 
    void min_max(const image2d<float>& ima, float& min, float& max)
    {
      min = 1.0E30;
      max = -1.0E30;
      const int nrows = ima.nrows(), ncols = ima.ncols();
      for (int row = 0; row < nrows; ++row)
	for (int col = 0; col < ncols; ++col)
	  {
	    if (ima(row, col) < min)
	      min = ima(row, col);
	    if (ima(row, col) > max)
	      max = ima(row, col);
	  }
    }



    image2d<float> add_float_noise(const image2d<unsigned char>& input)
    {
      image2d<float> output(input.nrows(), input.ncols());
      for (int row = 0; row < input.nrows(); ++row)
	for (int col = 0; col < input.ncols(); ++col)
	  output(row, col) = input(row, col) + float(rand() / RAND_MAX);
      return output;
    }



    bool diff(const image2d<float>& ima1, const image2d<float>& ima2)
    {
      const float err = 1E-5;
      assert(ima1.nrows() == ima2.nrows() && ima1.ncols() == ima2.ncols());
      for (int row = 0; row < ima1.nrows(); ++row)
	for (int col = 0; col < ima1.ncols(); ++col)
	  {
	    float sim =
	      std::min(ima1(row, col), ima2(row, col))
	      / std::max(ima1(row, col), ima2(row, col));
	    if (sim < 1 - err)
	      return true;
	  }
      return false;
    }



    std::vector<unsigned> compute_histo(const image2d<unsigned char>& input)
    {
      std::vector<unsigned> h(256, 0);
      const int nrows = input.nrows(), ncols = input.ncols();
      for (int row = 0; row < nrows; ++row)
	for (int col = 0; col < ncols; ++col)
	  ++h[input(row, col)];
      return h;
    }

    std::vector<unsigned> compute_histo(const image2d<unsigned short>& input)
    {
      std::vector<unsigned> h(65536, 0);
      const int nrows = input.nrows(), ncols = input.ncols();
      for (int row = 0; row < nrows; ++row)
	for (int col = 0; col < ncols; ++col)
	  ++h[input(row, col)];
      return h;
    }

    void print_histo(std::vector<unsigned>& h,
		     std::ofstream& file)
    {
      for (unsigned i = 0; i < h.size(); ++i)
	file << i << ' ' << h[i] << std::endl;
    }


    struct pix
    {
      pix() {}
      pix(float val, int row, int col) : val(val), row(row), col(col) {}
      bool operator<(const pix& rhs) const {
	if (val < rhs.val)
	  return true;
	if (val > rhs.val)
	  return false;
	return row < rhs.row or (row == rhs.row and col < rhs.col);
      }
      float val;
      int row, col;
    };


    std::vector<pix>
    sort(const image2d<float>& input)
    {
      std::vector<pix> v;

      const int nrows = input.nrows(), ncols = input.ncols();
      v.reserve(nrows * ncols);
      for (int row = 0; row < nrows; ++row)
	for (int col = 0; col < ncols; ++col)
	  v.push_back(pix(input(row, col), row, col));
      std::sort(v.begin(), v.end());
      return v;
    }


    // Crop a 2-D image.
    template <typename T>
    image2d<T>
    crop(const image2d<T>& input, coord x, coord y, coord dx, coord dy)
    {
      assertion (dx > 0);
      assertion (dy > 0);
      assertion(ima.hold(point2d(x, y)));
      assertion(ima.hold(point2d(x + dx, y + dy)));

      image2d<float> output(dx, dy);
      for (coord row = 0; row < dx; ++row)
	for (coord col = 0; col < dy; ++col)
	  output(row, col) = input(x + row, y + col);
      return output;
    }


  } // end of namespace oln::efigi

} // end of namespace oln


#endif // ! OLENA_LRDE_EFIGI_MISC_HH
