// Copyright 2001, 2002  EPITA Research and Development Laboratory
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


#ifndef OLENA_CONVERT_NRGBHSV_HH
# define OLENA_CONVERT_NRGBHSV_HH

# include <oln/convert/colorconv.hh>
# include <oln/value/nrgb.hh>
# include <oln/value/hsv.hh>

/*-----------------------------------------------------------------.
| The formulas used here come from ``Color Conversion Algorithms'' |
`-----------------------------------------------------------------*/

namespace oln {
  namespace convert {

    struct nrgb_to_hsv
      : public color_conversion<3, nrgb_traits,
				3, hsv_traits, nrgb_to_hsv>
    {
      typedef type::bottom inferior;

      template <unsigned qbits>
      color<3, qbits, hsv_traits>
      operator() (const color<3, qbits, nrgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	float max_in = std::max(in[nrgb_R], std::max(in[nrgb_B], in[nrgb_G]));
	float min_in = std::min(in[nrgb_R], std::min(in[nrgb_B], in[nrgb_G]));
	float delta = max_in - min_in;

	out[hsv_V] = max_in;

	if (max_in != 0)
	  out[hsv_S] = delta / max_in;
	else {
	  out[hsv_S] = 0;
	  out[hsv_H] = -1;
        }
	if (in[nrgb_R] == max_in)
	  out[hsv_H] = (in[nrgb_G] - in[nrgb_B]) / delta;
	else if (in[nrgb_G] == max_in)
	  out[hsv_H] = 2 + (in[nrgb_B] - in[nrgb_R]) / delta;
	else
	  out[hsv_H] = 4 + (in[nrgb_R] - in[nrgb_G]) / delta;
	out[hsv_H] *= 60;
	if (out[hsv_H] < 0)
	  out[hsv_H] += 360;

	return out;
      }

      static std::string name() { return "nrgb_to_hsv"; }
    };


    struct hsv_to_nrgb
      : public color_conversion<3, hsv_traits,
				3, nrgb_traits, hsv_to_nrgb>
    {
      typedef type::bottom inferior;


      template <unsigned qbits>
      color<3, qbits, nrgb_traits>
      operator() (const color<3, qbits, hsv_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;

	if(in[hsv_S] == 0)
	  out[nrgb_G] = out[nrgb_B] = out[nrgb_R] = in[hsv_V];

	in[hsv_H] /= 60;
	int i = (int)floor (in[hsv_H]);
	float f = in[hsv_H] - i;
	float p = in[hsv_V] * (1 - in[hsv_S]);
	float q = in[hsv_V] * (1 - in[hsv_S] * f);
	float t = in[hsv_V] * (1 - in[hsv_S] * (1 - f));

	switch (i){
	case 0:
	  out[nrgb_R] = in[hsv_V];
	  out[nrgb_G] = t;
	  out[nrgb_B] = p;
	  break;
	case 1:
	  out[nrgb_R] = q;
	  out[nrgb_G] = in[hsv_V];
	  out[nrgb_B] = p;
	  break;
	case 2:
	  out[nrgb_R] = p;
	  out[nrgb_G] = in[hsv_V];
	  out[nrgb_B] = t;
	  break;
	case 3:
	  out[nrgb_R] = p;
	  out[nrgb_G] = q;
	  out[nrgb_B] = in[hsv_V];
	  break;
	case 4:
	  out[nrgb_R] = t;
	  out[nrgb_G] = p;
	  out[nrgb_B] = in[hsv_V];
	  break;
	default:
	  out[nrgb_R] = in[hsv_V];
	  out[nrgb_G] = p;
	  out[nrgb_B] = q;
	  break;
	}

	return out;
      }

      static std::string name() { return "hsv_to_nrgb"; }
    };

  } // convert
} // oln

#endif // OLENA_CONVERT_NRGBHSV_HH
