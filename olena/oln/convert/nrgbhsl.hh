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


#ifndef OLENA_CONVERT_NRGBHSL_HH
# define OLENA_CONVERT_NRGBHSL_HH

# include <oln/convert/colorconv.hh>

# include <ntg/color/nrgb.hh>
# include <ntg/color/hsl.hh>
# include <ntg/core/typetraits_builtins.hh>
# include <cstdlib>

/*------------------------------------------------------------------.
| The formulas used here come from ``Color space conversion''; Paul |
| Bourke.                                                           |
`------------------------------------------------------------------*/

namespace oln {

  using namespace ntg;

  namespace convert {

    struct nrgb_to_hsl
      : public color_conversion<3, nrgb_traits,
				3, hsl_traits, nrgb_to_hsl>
    {
      template <unsigned qbits>
      color<3, qbits, hsl_traits>
      operator() (const color<3, qbits, nrgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;

	float max_in = std::max(in[nrgb_R], std::max(in[nrgb_B], in[nrgb_G]));
	float min_in = std::min(in[nrgb_R], std::min(in[nrgb_B], in[nrgb_G]));
	float diff = max_in-min_in;

	out[hsl_L] = (max_in + min_in) / 2;
	if (std::abs(diff) <= 0.0000001) {
	  out[hsl_S] = 0;
	  out[hsl_H] = -1;
	} else if (out[hsl_L] < 0.5)
	  out[hsl_S] = diff / (max_in + min_in);
	else
	  out[hsl_S] = diff / (2 - max_in - min_in);

	float r_dist = (max_in - in[nrgb_R]) / diff;
	float g_dist = (max_in - in[nrgb_G]) / diff;
	float b_dist = (max_in - in[nrgb_B]) / diff;

	if (in[nrgb_R] = max_in)
	  out[hsl_H] = b_dist-g_dist;
	else if(in[nrgb_G] = max_in)
	  out[hsl_H] = 2 + r_dist - b_dist;
	else if(in[nrgb_B] = max_in)
	  out[hsl_H] = 4 + g_dist - r_dist;
	out[hsl_H] *= 60;
	if(out[hsl_H] < 0)
	  out[hsl_H] += 360;

	return out;
      }

      static std::string name() { return "nrgb_to_hsl"; }
    };

    namespace internal {
      float RGB(float q1, float q2, float hue)
      {
	if (hue > 360)
	  hue -= 360;
	if (hue < 0)
	  hue += 360;
	if (hue < 60)
	  return q1 + (q2 - q1) * hue / 60;
	else if (hue < 180)
	  return q2;
	else if (hue < 240)
	  return q1 + (q2 - q1) * (240 - hue) / 60;
	else
	  return q1;
      }
    }

 struct hsl_to_nrgb
      : public color_conversion<3, hsl_traits,
				3, nrgb_traits, hsl_to_nrgb>
    {
      template <unsigned qbits>
      color<3, qbits, nrgb_traits>
      operator() (const color<3, qbits, hsl_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	float p2;

	if(in[hsl_L] <= 0.5)
	  p2 = in[hsl_L] * (1+in[hsl_S]);
	else
	  p2 = in[hsl_L] + in[hsl_S] - (in[hsl_L] * in[hsl_S]);

	float p1 = 2 * in[hsl_L] - p2;

	if(in[hsl_S] == 0)
	  out[nrgb_R] = out[nrgb_G] = out[nrgb_B] = in[hsl_L];
	else
	  {
	    out[nrgb_R] = internal::RGB(p1, p2, in[hsl_H] + 120);
	    out[nrgb_G] = internal::RGB(p1, p2, in[hsl_H]);
	    out[nrgb_B] = internal::RGB(p1, p2, in[hsl_H] - 120);
	  }

	return out;
      }

      static std::string name() { return "hsl_to_nrgb"; }
    };

  } // convert
} // oln

#endif // OLENA_CONVERT_NRGBHSL_HH
