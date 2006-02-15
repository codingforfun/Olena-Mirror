// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006  EPITA Research and Development Laboratory
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

#ifndef OLENA_LEVEL_ANISOTROPIC_DIFFUSION_HH
# define OLENA_LEVEL_ANISOTROPIC_DIFFUSION_HH

# include <oln/basics.hh>
# include <ntg/int.hh>
# include <oln/morpho/gradient_morpho.hh>

# define SQR(u) u*u
# define ABS(u) u >= 0 ? u : -u

namespace oln {

  namespace level {

    namespace impl {

      // version 2d of the 4nbhs gradient
      template <typename T>
      image2d<std::vector<double> >
      gradient_on_each_side_(const image2d<T>& input)
      {
	image2d<std::vector<double> > g(input.size());

	oln_iter_type(image2d<T>) p(input);
	point2d pt;

	for_all(p)
	  {
	    pt = p;

	    // N
	    g[p].push_back((input(pt.row() - 1, pt.col()) - input[p]) / 2);

	    // S
	    g[p].push_back((input(pt.row() + 1, pt.col()) - input[p]) / 2);

	    // E
	    g[p].push_back((input(pt.row(), pt.col() + 1) - input[p]) / 2);

	    // W
	    g[p].push_back((input(pt.row(), pt.col() - 1) - input[p]) / 2);
	  }

	return g;
      }


      template <typename T>
      double compute_capa(const image2d<T>& input)
      {
	// static version of cappa (near the variance)
	return 20;
      }


      double diffusion_coefficient(double grad, double K)
      {
	return 1 / (1 + SQR(grad / K));
      }

      // generic function of the anisotropic diffusion
      template <typename T, typename T2>
      image2d<T>
      anisotropic_diffusion_(const image2d<T>& input, const image2d<T2>& mask, unsigned nb_it, bool aniso)
      {
      	image2d<T> a_d(input.size());
      	image2d<std::vector<double> > grad(input.size());
	oln_iter_type(image2d<T>) p(input);

	bool conv = false;
	bool change = false;
	double K = 0;
	double modify = 0;
	double error = 0;
	double tmp = 0;
	unsigned it = 1;

	for_all(p)
	  {
	    a_d[p] = input[p];
	  }

	while (conv == false)
	  {
	    grad = gradient_on_each_side_(a_d);
	    K = compute_capa(a_d);
	    change = false;
	    error = 0;

	    for_all(p)
	      {
		if (mask[p] == false)
		  {
		    modify = 0;

		    std::vector<double>::const_iterator it;

		    for (it = grad[p].begin(); it != grad[p].end(); it++)
		      if (aniso == true)
			modify += (double)(*it) * diffusion_coefficient((double)*it, K);
		      else
			modify += (double)(*it);

		    if (modify != 0 and a_d[p] + 0.14 * modify >= 0 and a_d[p] + 0.14 * modify <= 255)
		      {
			tmp = a_d[p];
			a_d[p] = a_d[p] + 0.14 * modify;
			if ((double)a_d[p] != tmp)
			  change = true;
		      }
		    error += ABS(modify);
		  }
	      }

 	    std::cout << "error IT " << it << " : " << error << std::endl;

	    if ((it >= nb_it and nb_it != 0) or error < 0.1 or change == false)
	      conv = true;

	    it++;
	  };

	return a_d;
      }

    } // end of namespace oln::level::impl


    // nb_it = 0 signifies until algorithm convergence.
    template <typename T, typename T2>
    image2d<T>
    anisotropic_diffusion(const image2d<T>& input, const image2d<T2>& mask, unsigned nb_it, bool aniso = true)
    {
      return impl::anisotropic_diffusion_(input, mask, nb_it, aniso);
    }

  } // end of namespace oln::level

} // end of namespace oln

#endif // ! OLENA_LEVEL_ANISOTROPIC_DIFFUSION_HH
