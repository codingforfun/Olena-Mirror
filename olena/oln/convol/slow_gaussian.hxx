// Copyright (C) 2004  EPITA Research and Development Laboratory
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


#ifndef SLOW_GAUSSIAN_HXX
# define SLOW_GAUSSIAN_HXX
# include <oln/core/w_window1d.hh>
# include <oln/core/w_window2d.hh>
# include <oln/core/w_window3d.hh>

namespace oln {
  namespace convol {
    namespace slow {
      namespace internal {
	/* FIXME: The hierarchy w_window is a REAL mess.
	   a w_window1d is not an abstract::w_window,
	   and w_windownd does not supprot concrete_type, etc
	*/
	template<class T>
	inline T
	normalise(const T &in)
	{
	  T w = in;
	  ntg::float_d sum = 0.;
	  for (unsigned i = 0; i < in.card(); ++i)
	    sum += in.w(i);
	  sum = 1. / sum;
	  assert(finite(sum));
	  for (unsigned i = 0; i < in.card(); ++i)
	    w.set(w.dp(i), in.w(i) * sum);
	  return w;
	}

      }// End namespace internal

      //! Kernel of a 1D Gaussian.
      template<>
      struct gaussian_kernel<1>
      {
	typedef oln::w_window1d<ntg::float_d> ret;
	enum {dim = 1};

	//! Return a discrete Gaussian kernel, that is equal to zero
	// at a distance of radius_factor * sigma.
	static w_window1d<ntg::float_d>
	kernel(ntg::float_d sigma,
	       ntg::float_d radius_factor)
	{
	  precondition(sigma > 0.);
	  precondition(radius_factor >= 0.);
	  return internal::normalise(kernel_values(sigma, radius_factor));
	}

	//! Return values of the Gaussian kernel at discrete points.
	//
	// Note: the integral is not equal to 1 (discrete grid and 0 outside
	// radius_factor * sigma). You should use kernel(sigma, radius_factor).
	static w_window1d<ntg::float_d>
	kernel_values(ntg::float_d sigma,
		      ntg::float_d radius_factor)
	{
	  precondition(sigma > 0.);
	  precondition(radius_factor >= 0.);

	  w_window1d<ntg::float_d> w;

	  const int size = int(sigma * radius_factor);
	  const ntg::float_d inv_sigma_sqrt_2pi
	    = 1. / (sigma * sqrt(M_PI * 2.));
	  for (int x = -size; x <= +size; ++x)
	    w.add(x, inv_sigma_sqrt_2pi * exp(-x * x /(2 * sigma * sigma)));
	  return w;
	}
      };

      //! Kernel of a 2D Gaussian.
      template<>
      struct gaussian_kernel<2>
      {
	typedef w_window2d<ntg::float_d> ret;
	enum {dim = 2};

	//! Return a discrete Gaussian kernel, that is equal to zero at a
	// distance of radius_factor * sigma.
	static w_window2d<ntg::float_d>
	kernel(ntg::float_d sigma,
	       ntg::float_d radius_factor)
	{
	  precondition(sigma > 0.);
	  precondition(radius_factor >= 0.);

	  return internal::normalise(kernel_values(sigma, radius_factor));
	}

	//! Return values of the Gaussian kernel at discrete points.
	//
	// Note: the integral is not equal to 1 (discrete grid and 0 outside
	// radius_factor * sigma). You should use kernel(sigma, radius_factor).
	static w_window2d<ntg::float_d>
	kernel_values(ntg::float_d sigma,
	       ntg::float_d radius_factor)
	{
	  precondition(sigma > 0.);
	  precondition(radius_factor >= 0.);
	  w_window2d<ntg::float_d> w;

	  const int size = int(sigma * radius_factor);
	  const ntg::float_d inv_sigma_sigma_pi_2 = 1. / (sigma * sigma *
							  M_PI * 2.);

	  for (int x = -size; x <= +size; ++x)
	    for (int y = -size; y <= +size; ++y)
	      if (x * x + y * y <= size * size)
		w.add(x, y, inv_sigma_sigma_pi_2
		      *  exp(- (x * x +	y * y)
			     / ( 2. * sigma * sigma)
			     )
		    );
	  return w;
	}
      };

      //! Kernel of a 3D Gaussian.
      template<>
      struct gaussian_kernel<3>
      {
	typedef w_window3d<ntg::float_d> ret;
	enum {dim = 3};

	//! Return a discrete Gaussian kernel, that is equal to zero at a
	// distance of radius_factor * sigma.
	static w_window3d<ntg::float_d>
	kernel(ntg::float_d sigma,
		    ntg::float_d radius_factor)
	{
	  precondition(sigma > 0.);
	  precondition(radius_factor >= 0.);

	  return internal::normalise(kernel_values(sigma, radius_factor));
	}


	//! Return values of the Gaussian kernel at discrete points.
	//
	// Note: the integral is not equal to 1 (discrete grid and 0 outside
	// radius_factor * sigma). You should use kernel(sigma, radius_factor).
	static w_window3d<ntg::float_d>
	kernel_values(ntg::float_d sigma,
	       ntg::float_d radius_factor)
	{
	  precondition(sigma > 0.);
	  precondition(radius_factor >= 0.);

	  w_window3d<ntg::float_d> w;

	  const int size = int(sigma * radius_factor);
	  const ntg::float_d k = 1. / (sigma * sigma * sigma *
				       sqrt((M_PI * 2.) * (M_PI * 2.) *
					    (M_PI * 2.)));

	  for (int x = -size; x <= +size; ++x)
	    for (int y = -size; y <= +size; ++y)
	      for (int z = -size; z <= +size; ++z)
		if (x * x + y * y + z * z <= size)
		  w.add(x, y, z,  k *
			exp(-(x * x + y * y + z * z)
			    ) / (2. * sigma * sigma));
	  return w;
	}
      };

    }//End namespace slow
  }// End namespace convol
}// End namespace oln
#endif // end SLOW_GAUSSIAN_HXX
