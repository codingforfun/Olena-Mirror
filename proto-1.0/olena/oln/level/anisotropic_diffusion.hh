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
# include <oln/utils/clone.hh>
# include <oln/morpho/gradient_morpho.hh>
# include <oln/utils/clean_boundaries.hh>

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
	image2d<std::vector<double> > g(input.size(), "gradient");
	fwd_piter2d p(input.size());
	point2d pt;

	for_all_p(p)
	  {
	    pt = p;

	    // N
	    g.at(p).push_back((input(pt.row() - 1, pt.col()) - input[p]) / 2);

	    // S
	    g.at(p).push_back((input(pt.row() + 1, pt.col()) - input[p]) / 2);

	    // E
	    g.at(p).push_back((input(pt.row(), pt.col() + 1) - input[p]) / 2);

	    // W
	    g.at(p).push_back((input(pt.row(), pt.col() - 1) - input[p]) / 2);
	  }

	return g;
      }


      template <typename I>
      double compute_capa(const abstract::image<I>& input)
      {
	// static version of cappa (near the variance)
	return 6;


	// 4 nbhs version of gradient

	/*
        double K = 0;
	double seuil = 0;

        oln_type_of(I, concrete) grad(input.size());
        oln_type_of(I, fwd_piter) p(input.size());
        oln_type_of(I, point) pt;

	for_all_p(p)
	  {
	    pt = p;

	    grad[p] = (double) ((ABS((double) (input[oln_type_of(I, point)(pt.row() - 1, pt.col())].value()))) +
				(ABS((double) (input[oln_type_of(I, point)(pt.row() + 1, pt.col())].value()))) +
				(ABS((double) (input[oln_type_of(I, point)(pt.row(), pt.col() + 1)].value()))) +
				(ABS((double) (input[oln_type_of(I, point)(pt.row(), pt.col() - 1)].value()))) +
				(ABS((double) (- 4 * input[oln_type_of(I, point)((coord_t)(pt.row()), pt.col())].value())))) / 8;

	    seuil = grad[p] > seuil ? grad[p] : seuil;
	  }

	seuil *= 0.9;

	for_all_p(p)
          {
	    //            if (grad[p] < (int)seuil)
	    if (grad[p] > (int)seuil)
              K++;
          }
	std::cout << K << std::endl;
	return K;
	*/


	// morphologic version of gradient

	/*
	double K = 0;
	double seuil = 0.9 * 255;

	oln_type_of(I, concrete) grad = morpho::gradient_morpho(input);
	oln_type_of(I, fwd_piter) p(input.size());

	for_all_p(p)
	  {
	    if (grad[p] < (int)seuil)
	      //	    if (grad[p] > (int)seuil)
	      K++;
	  }
	return K;
	*/
      }


      double diffusion_coefficient(double grad, double K)
      {
	return 1 / (1 + SQR(grad / K));
      }

      // generic function of the anisotropic diffusion
      template <typename I>
      oln_type_of(I, concrete)
	anisotropic_diffusion_(const abstract::image<I>& input, unsigned nb_it)
      {
      	oln_type_of(I, concrete) a_d(input.size(), "gradient");
      	oln_ch_concrete_type(I, std::vector<double>) grad(input.size(), "gradient");
	oln_type_of(I, fwd_piter) p(input.size());

	bool conv = false;
	bool change = false;
	double K = 0;
	double modify = 0;
	double error = 0;
	double tmp = 0;
	unsigned it = 1;

	a_d = clone(input.exact());

	while (conv == false)
	  {
	    utils::clean_boundaries(a_d.exact());
	    grad = gradient_on_each_side_(a_d.exact());
	    K = compute_capa(a_d);
	    change = false;
	    error = 0;

	    for_all_p(p)
	      {
		modify = 0;

		typename std::vector<double>::const_iterator it;

		for (it = grad.at(p).begin(); it != grad.at(p).end(); it++)
		  modify += (double)(*it) * diffusion_coefficient((double)*it, K);

		if (modify != 0 and a_d[p] + 0.14 * modify >= 0 and a_d[p] + 0.14 * modify <= 255)
		  {
		    tmp = a_d[p];
		    a_d[p] = a_d[p] + 0.14 * modify;
		    if ((double)a_d[p] != tmp)
		      change = true;
		  }

		error += ABS(modify);
	      }

 	    std::cout << "error " << error << std::endl;

	    if ((it >= nb_it and nb_it != 0) or error < 0.1 or change == false)
	      conv = true;

	    it++;
	  };

	return a_d;
      }

    } // end of namespace oln::level::impl


    // nb_it = 0 signifies until algorithm convergence.
    template <typename I>
    oln_type_of(I, concrete)
      anisotropic_diffusion(const abstract::image<I>& input, unsigned nb_it = 0)
    {
      return impl::anisotropic_diffusion_(input.exact(), nb_it);
    }

  } // end of namespace oln::level

} // end of namespace oln

#endif // ! OLENA_LEVEL_ANISOTROPIC_DIFFUSION_HH
