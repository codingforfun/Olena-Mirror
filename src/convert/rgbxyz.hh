// Copyright 2001  EPITA Research and Development Laboratory
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


#ifndef OLENA_CONVERT_RGBXYZ_HH
# define OLENA_CONVERT_RGBXYZ_HH

# include "convert/colorconv.hh"
# include "value/rgb.hh"
# include "value/xyz.hh"

/*--------------------------------------------------------------.
| The formulas used here come from ``Digital Image Processing   |
| Algorithms and Applications'', I. Pitas; Wiley-Interscience.  |
`--------------------------------------------------------------*/

namespace oln {
  namespace convert {

    struct rgb_to_xyz
      : public color_conversion<3, rgb_traits,
				3, xyz_traits, rgb_to_xyz>
    {
      typedef type::bottom inferior;

      template <unsigned qbits>
      color<3, qbits, xyz_traits>
      operator() (const color<3, qbits, rgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[xyz_X] = 0.490 * in[rgb_R] + 0.310 * in[rgb_G] + 0.200 * in[rgb_B];
	out[xyz_Y] = 0.177 * in[rgb_R] + 0.813 * in[rgb_G] + 0.011 * in[rgb_B];
	out[xyz_Z] =                     0.010 * in[rgb_G] + 0.990 * in[rgb_B];
	return out;
      }

      static std::string name() { return "rgb_to_xyz"; }
    };

    struct xyz_to_rgb
      : public color_conversion<3, xyz_traits,
				3, rgb_traits, xyz_to_rgb>
    {
      typedef type::bottom inferior;

      template <unsigned qbits>
      color<3, qbits, rgb_traits>
      operator() (const color<3, qbits, xyz_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[rgb_R] =
	    2.365 * in[xyz_X] - 0.896 * in[xyz_Y] - 0.468 * in[xyz_Z];
	out[rgb_G] =
	  - 0.515 * in[xyz_X] + 1.425 * in[xyz_Y] + 0.088 * in[xyz_Z];
	out[rgb_B] =
	    0.005 * in[xyz_X] - 0.014 * in[xyz_Y] + 1.009 * in[xyz_Z];
	return out;
      }

      static std::string name() { return "xyz_to_rgb"; }
    };

  } // convert
} // oln

#endif // OLENA_CONVERT_RGBXYZ_HH
