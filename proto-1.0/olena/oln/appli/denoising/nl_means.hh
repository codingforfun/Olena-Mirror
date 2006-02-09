// Copyright (C) 2001, 2002, 2004, 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef OLENA_DENOISING_NLMEANS_HH
# define OLENA_DENOISING_NLMEANS_HH

# define SQR(u) u*u
# define PI 3.141592653589793238462643

# include <cmath>
# include <oln/level/fill.hh>

namespace oln {

  namespace denoising {

    namespace impl {


      template<typename I>
      const oln_type_of(I, concrete) gaussian_mask(double variance,
						   int rows,
						   int cols)
      {
	oln_type_of(I, concrete) g_m(rows, cols, 0);

	int i, j;

	for (i = 0; i < rows; i++)
	  for (j = 0; j < cols; j++)
	    g_m(i,j) = (1 / 2 * PI * SQR(variance)) * exp(-(SQR(rows / 2 - i) + SQR(cols / 2 - j)));

	return g_m;
      }


      template<typename I>
      oln_type_of(I, concrete) nl_means_(const oln::abstract::image<I>& input,
					 int window,
					 double variance,
					 int horizon)
      {
	oln_type_of(I, concrete) nl(input.size(), "nl_means");
	oln_type_of(I, concrete) g_m;
	oln_type_of(I, fwd_piter) p(input.size());
	oln_type_of(I, point) pt1;
	oln_type_of(I, point) pt2;

	int i, j, k, l, i2, j2, k2, l2;
	double dist;
	double w;
	double Z;
	double val;


	level::fill(nl, 0);

	g_m = gaussian_mask<I>(variance, window, window);

	for (i = 0; i < input.size().ncols(); i++)
	  {
	    for (j = 0; j < input.size().nrows(); j++)
	      {
		Z = 0;
		val = 0;

		for (k = i - horizon; k < i + horizon; k++)
		  for (l = j - horizon; l < j + horizon; l++)
		    {
		      if (not (k >= 0 and k < (int)input.size().ncols() and l >= 0 and l < (int)input.size().nrows()))
			continue;

		      dist = 0;
		      w = 0;

		      for (k2 = k - window / 2, i2 = i - window / 2; k2 < (k + window / 2 + window % 2); k2++, i2++)
			for (l2 = l - window / 2, j2 = j - window / 2; l2 < (l + window / 2 + window % 2); l2++, j2++)

			  if (i2 >= 0 and i2 < input.size().ncols() && j2 >= 0 and j2 < input.size().nrows() and
			      k2 >= 0 and k2 < input.size().ncols() && l2 >= 0 and l2 < input.size().nrows())
			    {
			      dist += g_m.exact()(k2 + window / 2 - k, l2 + window / 2 - l) *
				((input.exact()(i2, j2) - input.exact()(k2, l2)) *
				 (input.exact()(i2, j2) - input.exact()(k2, l2)));
			    }

		      w = exp(-dist/(SQR(variance)));
		      val += w * input.exact()(k, l).value();
		      Z += w;
		    }

		nl(i,j) = val / Z;
	      }

	    std::cout << (int)input.size().nrows() << " / " << i + 1 << std::endl;
	  }

	return nl;
      }

    } // end of namespace oln::denoising::impl

    template<typename I>
    oln_type_of(I, concrete) nl_means(const oln::abstract::image<I>& input,
				      int window,
				      double variance,
				      int horizon)
    {
      return impl::nl_means_(input.exact(), window, variance, horizon);
    }


  } // end of namespace oln::denoising

} // end of namespace oln

#endif // OLENA_DENOISING_NLMEANS_HH
