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


#ifndef OLENA_CONVERT_NRGBHSI_HH
# define OLENA_CONVERT_NRGBHSI_HH

# include <oln/convert/colorconv.hh>

# include <ntg/color/nrgb.hh>
# include <ntg/color/hsi.hh>
# include <ntg/basics.hh>

# include <cassert>

/*--------------------------------------------------------------.
| The formulas used here come from ``Digital Image Processing   |
| Algorithms and Applications'', I. Pitas; Wiley-Interscience.  |
`--------------------------------------------------------------*/

namespace oln {

  using namespace ntg;

  namespace convert {

    static const float sqrt3_3   = sqrt(3) / 3;
    static const float inv_sqrt6 = 1 / sqrt(6);
    static const float inv_sqrt2 = 1 / sqrt(2);

    struct nrgb_to_hsi
      : public color_conversion<3, nrgb_traits,
				3, hsi_traits, nrgb_to_hsi>
    {
      template <unsigned qbits>
      color<3, qbits, hsi_traits>
      operator() (const color<3, qbits, nrgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[hsi_I] =
	  sqrt3_3 * in[nrgb_R] + sqrt3_3 * in[nrgb_G] + sqrt3_3 * in[nrgb_B];
	const float v1 = inv_sqrt2 * in[nrgb_G] - inv_sqrt2 * in[nrgb_B];
	const float v2 = 2 * inv_sqrt6 * in[nrgb_R] - inv_sqrt6 * in[nrgb_G]
	  - inv_sqrt6 * in[nrgb_B];
	out[hsi_H] = atan2(v2, v1) / M_PI * 180.0;
	if (out[hsi_H] < 0)
	  out[hsi_H] += 360.0;
	assert(out[hsi_H] >= 0);
	out[hsi_S] = sqrt(v1 * v1 + v2 * v2);
	return out;
      }

      static std::string name() { return "nrgb_to_hsi"; }
    };

    struct hsi_to_nrgb
      : public color_conversion<3, hsi_traits,
				3, nrgb_traits, hsi_to_nrgb>
    {
      template <unsigned qbits>
      color<3, qbits, nrgb_traits>
      operator() (const color<3, qbits, hsi_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	const float h = in[hsi_H] / 180.0 * M_PI;
	const float v1 = in[hsi_S] * cos(h);
	const float v2 = in[hsi_S] * sin(h);
	out[nrgb_R] = sqrt3_3 * in[hsi_I] + 2 * inv_sqrt6 * v2;
	out[nrgb_G] = sqrt3_3 * in[hsi_I] + inv_sqrt2 * v1 - inv_sqrt6 * v2;
	out[nrgb_B] = sqrt3_3 * in[hsi_I] - inv_sqrt2 * v1 - inv_sqrt6 * v2;
	return out;
      }

      static std::string name() { return "hsi_to_nrgb"; }
    };

  } // convert
} // oln

#endif // OLENA_CONVERT_NRGBHSI_HH
