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


#ifndef OLENA_CONVERT_RGBNRGB_HH
# define OLENA_CONVERT_RGBNRGB_HH

# include <oln/convert/colorconv.hh>

# include <ntg/color/rgb.hh>
# include <ntg/color/nrgb.hh>
# include <ntg/basics.hh>

/*--------------------------------------------------------------.
| The formulas used here come from ``Digital Image Processing   |
| Algorithms and Applications'', I. Pitas; Wiley-Interscience.  |
`--------------------------------------------------------------*/

namespace oln {
  
  using namespace ntg;

  namespace convert {

    struct rgb_to_nrgb
      : public color_conversion<3, rgb_traits,
				3, nrgb_traits, rgb_to_nrgb>
    {
      template <unsigned qbits>
      color<3, qbits, nrgb_traits>
      operator() (const color<3, qbits, rgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[nrgb_R] =
	  + 0.8417270*in[rgb_R] + 0.1560987*in[rgb_G] + 0.0906747*in[rgb_B];
	out[nrgb_G] =
	  - 0.1290152*in[rgb_R] + 1.3189264*in[rgb_G] - 0.2031832*in[rgb_B];
	out[nrgb_B] =
	  + 0.0074943*in[rgb_R] - 0.0688480*in[rgb_G] + 0.8972327*in[rgb_B];
	return out;
      }

      static std::string name() { return "rgb_to_nrgb"; }
    };

    struct nrgb_to_rgb
      : public color_conversion<3, nrgb_traits,
				3, rgb_traits, nrgb_to_rgb>
    {
      template <unsigned qbits>
      color<3, qbits, rgb_traits>
      operator() (const color<3, qbits, nrgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[rgb_R] =
	  + 1.167 * in[nrgb_R] - 0.146 * in[nrgb_G] - 0.151 * in[nrgb_B];
	out[rgb_G] =
	  + 0.114 * in[nrgb_R] + 0.753 * in[nrgb_G] + 0.159 * in[nrgb_B];
	out[rgb_B] =
	  - 0.001 * in[nrgb_R] + 0.059 * in[nrgb_G] + 1.128 * in[nrgb_B];
	return out;
      }

      static std::string name() { return "nrgb_to_rgb"; }
    };

  } // convert
} // oln

#endif // OLENA_CONVERT_RGBNRGB_HH
