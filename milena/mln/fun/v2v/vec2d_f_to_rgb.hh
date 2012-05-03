// Copyright (C) 2012 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_FUN_V2V_VEC2D_F_TO_RGB_HH
# define MLN_FUN_V2V_VEC2D_F_TO_RGB_HH

/// \file
///
/// \brief Convert vec2d_f values to RGB.

# include <math.h>
# include <mln/value/rgb.hh>
# include <mln/core/alias/vec2d.hh>
# include <mln/math/round.hh>

# define setvals(r, g, b, k) {color_wheel[k][0] = r;\
    color_wheel[k][1] = g;\
    color_wheel[k++][2] = b;}

namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      /// \brief Convert vec2d_f values to rgb.
      ///
      /// \ingroup modfunv2v
      //
      template <typename T_rgb>
      struct f_vec2d_f_to_rgb_ : public Function_v2v< f_vec2d_f_to_rgb_<T_rgb> >
      {
	typedef T_rgb result;

        T_rgb operator()(const mln::vec2d_f& vec) const;

      };

# ifndef MLN_INCLUDE_ONLY


      template <typename T_rgb>
      inline
      T_rgb
      f_vec2d_f_to_rgb_<T_rgb>::operator()(const mln::vec2d_f& vec) const
      {
	typedef typename T_rgb::red_t   red_t;
	typedef typename T_rgb::green_t green_t;
	typedef typename T_rgb::blue_t  blue_t;
	
	red_t r;
	green_t g;
	blue_t b;

	static math::round<red_t>   to_r;
	static math::round<green_t> to_g;
	static math::round<blue_t>  to_b;

	int RY = 15;
	int YG = 6;
	int GC = 4;
	int CB = 11;
	int BM = 13;
	int MR = 6;

	int color_wheel[55][3];

	int k = 0;
	for (int i = 0; i < RY; ++i)
	  setvals(255, 255 * i / RY, 0, k);
	for (int i = 0; i < YG; ++i)
	  setvals(255 - 255 * i / YG, 255, 0, k);
	for (int i = 0; i < GC; ++i)
	  setvals(0, 255, 255 * i / GC, k);
	for (int i = 0; i < CB; ++i)
	  setvals(0, 255 - 255 * i / CB, 255, k);
	for (int i = 0; i < BM; ++i)
	  setvals(255 * i / BM, 0, 255, k);
	for (int i = 0; i < MR; ++i)
	  setvals(255, 0, 255 - 255 * i / MR, k);

	float u = vec[0];
	float v = vec[1];

	float rad = sqrt (u * u + v * v);
	float a = atan2 (-v, -u) / 3.14159265;
	float fk = (a + 1.0) / 2.0 * (54);
	int k0 = (int)fk;
	int k1 = (k0 + 1) % 55;
	float f = fk - k0;

	//Red.
	float col0 = color_wheel[k0][0] / 255.0;
	float col1 = color_wheel[k1][0] / 255.0;
	float col = (1 - f) * col0 + f * col1;
	if (rad <= 1)
	  col = 1 - rad * (1 - col);
	else
	  col *= 0.75;

	r = to_r(255 * col);

	//Green.
	col0 = color_wheel[k0][1] / 255.0;
	col1 = color_wheel[k1][1] / 255.0;
	col = (1 - f) * col0 + f * col1;
	if (rad <= 1)
	  col = 1 - rad * (1 - col);
	else
	  col *= 0.75;

	g = to_g(255 * col);

	//Blue.
	col0 = color_wheel[k0][2] / 255.0;
	col1 = color_wheel[k1][2] / 255.0;
	col = (1 - f) * col0 + f * col1;
	if (rad <= 1)
	  col = 1 - rad * (1 - col);
	else
	  col *= 0.75;

	b = to_b(255 * col);
	
	T_rgb rgb_result (r, g, b);

	return (rgb_result);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2v

  } // end of namespace mln::fun

} // end of namespace mln

#endif // ! MLN_FUN_V2V_VEC2D_F_TO_RGB_HH
