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


#ifndef OLENA_CONVERT_NRGBHSV_HH
# define OLENA_CONVERT_NRGBHSV_HH

# include <oln/basics.hh>
# include <oln/convert/abstract/colorconv.hh>

# include <ntg/basics.hh>
# include <ntg/color/nrgb.hh>
# include <ntg/color/hsv.hh>

# include <mlc/contract.hh>

# include <sstream>

/*-----------------------------------------------------------------.
| The formulas used here come from ``Color Conversion Algorithms'' |
`-----------------------------------------------------------------*/

namespace oln {

  using namespace ntg;

  namespace convert {

    template<unsigned inbits, unsigned outbits>
    struct f_nrgb_to_hsv
      : public abstract::color_conversion<3, inbits, nrgb_traits,
					  3, outbits, hsv_traits, f_nrgb_to_hsv<inbits, outbits> >
    {
      color<3, outbits, hsv_traits>
      doit(const color<3, inbits, nrgb_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;
	float max_in = std::max(in[nrgb_R], std::max(in[nrgb_B], in[nrgb_G]));
	float min_in = std::min(in[nrgb_R], std::min(in[nrgb_B], in[nrgb_G]));
	float delta = max_in - min_in;


	out[hsv_V] = max_in;

	if (max_in != 0)
	  out[hsv_S] = delta / max_in;
	else 
	  out[hsv_S] = 0;

	if (out[hsv_S] == 0)
	  out[hsv_H] = -1;   // undefined
	else { 
	  if (in[nrgb_R] == max_in)
	    out[hsv_H] = (in[nrgb_G] - in[nrgb_B]) / delta;
	  else if (in[nrgb_G] == max_in)
	    out[hsv_H] = 2 + (in[nrgb_B] - in[nrgb_R]) / delta;
	  else
	    out[hsv_H] = 4 + (in[nrgb_R] - in[nrgb_G]) / delta;
	  out[hsv_H] *= 60;
	  if (out[hsv_H] < 0)
	    out[hsv_H] += 360;
	}

	return out;
      }

      static std::string name()
      { 
	std::ostringstream s;
	s << "f_nrgb_to_hsv<" << inbits << ", " << outbits << '>'; 
	s.str();
      }
    };

    template <unsigned inbits, unsigned outbits>
    color<3, outbits, hsv_traits>
    nrgb_to_hsv(const color<3, inbits, nrgb_traits>& v)
    {
      f_nrgb_to_hsv<inbits, outbits> f;
      return f(v);
    }

    template<unsigned inbits, unsigned outbits>
    struct f_hsv_to_nrgb
      : public abstract::color_conversion<3, inbits, hsv_traits,
				3, outbits, nrgb_traits, f_hsv_to_nrgb<inbits, outbits> >
    {
      color<3, outbits, nrgb_traits>
      doit(const color<3, inbits, hsv_traits>& v) const
      {
	vec<3, float> in = v.to_float();
	vec<3, float> out;

	if(in[hsv_S] == 0)
	  out[nrgb_G] = out[nrgb_B] = out[nrgb_R] = in[hsv_V];
	else
	  {
	    in[hsv_H] /= 60;
	    int i = (int)floor (in[hsv_H]);
	    float f = in[hsv_H] - i;
	    float p = in[hsv_V] * (1 - in[hsv_S]);
	    float q = in[hsv_V] * (1 - in[hsv_S] * f);
	    float t = in[hsv_V] * (1 - in[hsv_S] * (1 - f));

	    switch (i){
	    case 0:
	    case 6:
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
	  }
	return out;
      }

      static std::string name() 
      { 
	std::ostringstream s;
	s << "f_hsv_to_nrgb<" << inbits << ", " << outbits << '>'; 
	s.str();
      }
    };

    template <unsigned inbits, unsigned outbits>
    color<3, outbits, nrgb_traits>
    hsv_to_nrgb(const color<3, inbits, hsv_traits>& v)
    {
      f_hsv_to_nrgb<inbits, outbits> f;
      return f(v);
    }

  } // convert
} // oln

#endif // OLENA_CONVERT_NRGBHSV_HH
