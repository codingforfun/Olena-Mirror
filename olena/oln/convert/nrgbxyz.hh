// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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


#ifndef OLENA_CONVERT_NRGBXYZ_HH
# define OLENA_CONVERT_NRGBXYZ_HH

# include <oln/convert/colorconv.hh>

# include <ntg/nrgb.hh>
# include <ntg/xyz.hh>
# include <ntg/typetraits_builtins.hh>

/*--------------------------------------------------------------.
| The formulas used here come from ``Digital Image Processing   |
| Algorithms and Applications'', I. Pitas; Wiley-Interscience.  |
`--------------------------------------------------------------*/

namespace oln {

  using namespace ntg;

  namespace convert {

    struct nrgb_to_xyz
      : public color_conversion<3, nrgb_traits,
				3, xyz_traits, nrgb_to_xyz>
    {
      template <unsigned qbits>
      color<3, qbits, xyz_traits>
      operator() (const color<3, qbits, nrgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[xyz_X] =
	  0.607 * in[nrgb_R] + 0.174 * in[nrgb_G] + 0.201 * in[nrgb_B];
	out[xyz_Y] =
	  0.299 * in[nrgb_R] + 0.587 * in[nrgb_G] + 0.114 * in[nrgb_B];
	out[xyz_Z] =
	                       0.066 * in[nrgb_G] + 1.117 * in[nrgb_B];
	return out;
      }

      static std::string name() { return "nrgb_to_xyz"; }
    };

    struct xyz_to_nrgb
      : public color_conversion<3, xyz_traits,
				3, nrgb_traits, xyz_to_nrgb>
    {
      template <unsigned qbits>
      color<3, qbits, nrgb_traits>
      operator() (const color<3, qbits, xyz_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[nrgb_R] =
	    1.910 * in[xyz_X] - 0.534 * in[xyz_Y] - 0.289 * in[xyz_Z];
	out[nrgb_G] =
	  - 0.984 * in[xyz_X] + 1.998 * in[xyz_Y] - 0.027 * in[xyz_Z];
	out[nrgb_B] =
	    0.058 * in[xyz_X] - 0.118 * in[xyz_Y] + 0.897 * in[xyz_Z];
	return out;
      }

      static std::string name() { return "xyz_to_nrgb"; }
    };

  } // convert
} // oln

#endif // OLENA_CONVERT_NRGBXYZ_HH
