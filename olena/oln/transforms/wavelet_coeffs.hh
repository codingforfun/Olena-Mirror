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

#ifndef OLENA_TRANSFORMS_WAVELET_COEFFS_HH
# define OLENA_TRANSFORMS_WAVELET_COEFFS_HH

# include <ntg/basics.hh>
# include <ntg/float.hh>
# include <oln/transforms/dwt.hh>

namespace oln {

  namespace transforms {

    //
    // Haar wavelet coefficients
    //
    //////////////////////////////////////

    Wavelet_coeffs_definition(haar, ntg::float_d, 2)
    {
      const ntg::float_d sqrt_2 = sqrt(2);

      Wavelet_coeffs_begin
        sqrt_2 * 1.0 / 2.0,
        sqrt_2 * 1.0 / 2.0
      Wavelet_coeffs_end
    };


    //
    // Daubechies wavelet coefficients
    //
    //////////////////////////////////////

    Wavelet_coeffs_definition(daub4, ntg::float_d, 4)
    {
      const ntg::float_d sqrt_3 = sqrt(3);
      const ntg::float_d denom = 4 * sqrt(2);

      Wavelet_coeffs_begin
        (1.0 + sqrt_3)/denom,
        (3.0 + sqrt_3)/denom,
        (3.0 - sqrt_3)/denom,
        (1.0 - sqrt_3)/denom
      Wavelet_coeffs_end
    };

    Wavelet_coeffs_definition(daub6, ntg::float_d, 6)
    {
      Wavelet_coeffs_begin
	0.332670552950,
	0.806891509311,
	0.459877502118,
	-0.135011020010,
	-0.085441273882,
	0.035226291882
      Wavelet_coeffs_end
    };

    Wavelet_coeffs_definition(daub8, ntg::float_d, 8)
    {
      Wavelet_coeffs_begin
	0.230377813309,
	0.714846570553,
	0.630880767930,
	-0.027983769417,
	-0.187034811719,
	0.030841381836,
	0.032883011667,
	-0.010597401785
      Wavelet_coeffs_end
    };

    Wavelet_coeffs_definition(daub10, ntg::float_d, 10)
    {
      Wavelet_coeffs_begin
	0.160102397974,
	0.603829269797,
	0.724308528438,
	0.138428145901,
	-0.242294887066,
	-0.032244869585,
	0.077571493840,
	-0.006241490213,
	-0.012580751999,
	0.003335725285
      Wavelet_coeffs_end
    };

    Wavelet_coeffs_definition(daub12, ntg::float_d, 12)
    {
      Wavelet_coeffs_begin
	0.111540743350,
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

    Wavelet_coeffs_definition(daub20, ntg::float_d, 20)
    {
      Wavelet_coeffs_begin
	0.026670057901,
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


    //
    // Coifman wavelet coefficients
    //
    //////////////////////////////////////

    Wavelet_coeffs_definition(coiflet2, ntg::float_d, 6)
    {
      const ntg::float_d sqrt_2 = sqrt(2);
      const ntg::float_d sqrt_15 = sqrt(15);

      Wavelet_coeffs_begin
	sqrt_2 * (sqrt_15 - 3) / 32.0,
	sqrt_2 * (1 - sqrt_15) / 32.0,
	sqrt_2 * (6 - 2 * sqrt_15) / 32.0,
	sqrt_2 * (2 * sqrt_15 + 6) / 32.0,
	sqrt_2 * (sqrt_15 + 13) / 32.0,
	sqrt_2 * (9 - sqrt_15) / 32.0
      Wavelet_coeffs_end
    };

    Wavelet_coeffs_definition(coiflet4, ntg::float_d, 12)
    {
      Wavelet_coeffs_begin
	0.0011945726958388,
	-0.01284557955324,
	0.024804330519353,
	0.050023519962135,
	-0.15535722285996,
	-0.071638282295294,
	0.57046500145033,
	0.75033630585287,
	0.28061165190244,
	-0.0074103835186718,
	-0.014611552521451,
	-0.0013587990591632
      Wavelet_coeffs_end
    };

    Wavelet_coeffs_definition(coiflet6, ntg::float_d, 18)
    {
      Wavelet_coeffs_begin
	-0.0016918510194918,
	-0.00348787621998426,
	 0.019191160680044,
	 0.021671094636352,
	-0.098507213321468,
	-0.056997424478478,
	 0.45678712217269,
	 0.78931940900416,
	 0.38055713085151,
	-0.070438748794943,
	-0.056514193868065,
	 0.036409962612716,
	 0.0087601307091635,
	-0.011194759273835,
	-0.0019213354141368,
	 0.0020413809772660,
	 0.00044583039753204,
	-0.00021625727664696
      Wavelet_coeffs_end
    };

  } // end of namespace transforms

} // end of namespace oln

#endif // ! OLENA_TRANSFORMS_WAVELET_COEFFS_HH
