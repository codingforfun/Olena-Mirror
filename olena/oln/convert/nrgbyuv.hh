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


#ifndef OLENA_CONVERT_NRGBYUV_HH
# define OLENA_CONVERT_NRGBYUV_HH

# include <oln/convert/colorconv.hh>

# include <ntg/color/nrgb.hh>
# include <ntg/color/yuv.hh>
# include <ntg/core/typetraits_builtins.hh>

/*---------------------------------------------------------------.
| The formulas used here come from ``Colour Space Conversions'', |
| IAdrian Ford and Alan Roberts; August 11,1998.                 |
`---------------------------------------------------------------*/

namespace oln {
  
  using namespace ntg;

  namespace convert {

    struct nrgb_to_yuv
      : public color_conversion<3, nrgb_traits,
				3, yuv_traits, nrgb_to_yuv>
    {
      template <unsigned qbits>
      color<3, qbits, yuv_traits>
      operator() (const color<3, qbits, nrgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[yuv_Y] =
	  + 0.299 * in[nrgb_R] + 0.587  * in[nrgb_G] + 0.114 * in[nrgb_B];
	out[yuv_U] =
	  - 0.147 * in[nrgb_R] - 0.2897 * in[nrgb_G] + 0.436 * in[nrgb_B];
	out[yuv_V] =
	  + 0.615 * in[nrgb_R] - 0.515  * in[nrgb_G] - 0.100 * in[nrgb_B];
	return out;
      }

      static std::string name() { return "nrgb_to_yuv"; }
    };

    struct yuv_to_nrgb
      : public color_conversion<3, yuv_traits,
				3, nrgb_traits, yuv_to_nrgb>
    {
      template <unsigned qbits>
      color<3, qbits, nrgb_traits>
      operator() (const color<3, qbits, yuv_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	out[nrgb_R] = in[yuv_Y]                     + 1.140 * in[yuv_V];
	out[nrgb_G] = in[yuv_Y] - 0.396 * in[yuv_U] - 0.581 * in[yuv_V];
	out[nrgb_B] = in[yuv_Y] + 2.029 * in[yuv_U];

	return out;
      }

      static std::string name() { return "yuv_to_nrgb"; }
    };

  } // convert
} // oln

#endif // OLENA_CONVERT_NRGBYUV_HH
