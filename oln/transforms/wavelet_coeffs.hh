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

#ifndef OLENA_TRANSFORMS_WAVELET_COEFFS_HH
# define OLENA_TRANSFORMS_WAVELET_COEFFS_HH

# include <oln/transforms/dwt.hh>

namespace oln {

  namespace transforms {

    // FIXME: see with Astrid for the "x()" problem.

    Wavelet_coeffs_definition(haar, dfloat, 2)
    {
      const dfloat sqrt_2 = sqrt(2);

      Wavelet_coeffs_begin
        x(1 / sqrt_2),
        1 / sqrt_2
      Wavelet_coeffs_end
    };

    Wavelet_coeffs_definition(daub4, dfloat, 4)
    {
      const dfloat sqrt_3 = sqrt(3);
      const dfloat denom = 4 * sqrt(2);

      Wavelet_coeffs_begin
        x((1 + sqrt_3)/denom),
        (3 + sqrt_3)/denom,
        (3 - sqrt_3)/denom,
        (1 - sqrt_3)/denom
      Wavelet_coeffs_end
    };

    Wavelet_coeffs_definition(daub6, dfloat, 6)
    {
      const dfloat sqrt_2 = sqrt(2);

      Wavelet_coeffs_begin
        x(0.235234f * sqrt_2),
        0.570558f * sqrt_2,
        0.325183f * sqrt_2,
        -0.095467f * sqrt_2,
        -0.060416f * sqrt_2,
        0.024909f * sqrt_2
      Wavelet_coeffs_end
    };

    Wavelet_coeffs_definition(daub12, dfloat, 12)
    {
      Wavelet_coeffs_begin
	x(0.111540743350),
	0.494623890398,
	0.751133908021,
	0.315250351709,
	-0.226264693965,
	-0.129766867567,
	0.097501605587,
	0.027522865530,
	-0.031582039318,
	0.000553842201,
	0.004777257511,
	-0.001077301085
      Wavelet_coeffs_end
    };

    Wavelet_coeffs_definition(daub20, dfloat, 20)
    {
      Wavelet_coeffs_begin
	x(0.026670057901),
	0.188176800078,
	0.527201188932,
	0.688459039454,
	0.281172343661,
	-0.249846424327,
	-0.195946274377,
	0.127369340336,
	0.093057364604,
	-0.071394147166,
	-0.029457536822,
	0.033212674059,
	0.003606553567,
	-0.010733175483,
	0.001395351747,
	0.001992405295,
	-0.000685856695,
	-0.000116466855,
	0.000093588670,
	-0.000013264203
      Wavelet_coeffs_end
    };

  } // end of namespace transforms

} // end of namespace oln

#endif // ! OLENA_TRANSFORMS_WAVELET_COEFFS_HH
