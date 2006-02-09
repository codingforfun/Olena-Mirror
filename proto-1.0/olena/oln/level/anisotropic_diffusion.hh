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
# define SQR(u) u*u
# define ABS(u) u > 0 ? u : -u

namespace oln {

  namespace level {

    namespace impl {

      template <typename I>
      oln_ch_concrete_type(I, std::vector<double>)
	gradient_on_each_side_(const abstract::image<I>& input)
      {
	oln_ch_concrete_type(I, std::vector<double>) g(input.size(), "gradient");
	oln_type_of(I, fwd_piter) p(input.size());
	oln_type_of(I, point) pt;

	for_all_p(p)
	  {
	    pt = p;

	    // N
	    g.at(p).push_back((double)input[oln_type_of(I, point)((coord_t)(pt.row() - 1), pt.col())].value() -
			      input[oln_type_of(I, point)((coord_t)(pt.row()), pt.col())].value());

	    // S
	    g.at(p).push_back((double)input[oln_type_of(I, point)((coord_t)(pt.row() + 1), pt.col())].value() -
			      input[oln_type_of(I, point)((coord_t)(pt.row()), pt.col())].value());

	    // E
	    g.at(p).push_back((double)input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() + 1))].value() -
			      input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col()))].value());

	    // W
	    g.at(p).push_back((double)input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() - 1))].value() -
			      input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col()))].value());
	  }

	return g;
      }


      template <typename T>
      void clean_ima(image2d<T>& ima)
      {
	for (int j = 0; j < ima.size().ncols(); j++)
	  {
	    ima(-1, j) = ima(0, j);
	    ima(ima.size().nrows(), j) = ima(ima.size().nrows() - 1, j);
	  }

	for (int j = 0; j < ima.size().nrows(); j++)
	  {
	    ima(j, -1) = ima(j, 0);
	    ima(j, ima.size().ncols()) = ima(j, ima.size().ncols() - 1);
	  }
      }


      template <typename I>
      double compute_capa(const abstract::image<I>& input)
      {
	double K = 0;
	// on admet un 255 dans l'image
	double seuil = 0.9 * 255;

	oln_type_of(I, concrete) grad = morpho::gradient_morpho(input);
	oln_type_of(I, fwd_piter) p(input.size());

	for_all_p(p)
	  {
	    if (grad[p] < (int)seuil)
	      K++;
	  }
	return K;
      }

      double diffusion_coefficient(double grad, double K)
      {
	std::cout << "----------------" << std::endl;
	std::cout << grad << std::endl;
	std::cout << K << std::endl;
	return 1 / (1 + SQR(grad / K));
      }

      template <typename I>
      oln_type_of(I, concrete)
	anisotropic_diffusion_(const abstract::image<I>& input)
      {
      	oln_type_of(I, concrete) a_d(input.size(), "gradient");
      	oln_ch_concrete_type(I, std::vector<double>) grad(input.size(), "gradient");
	oln_type_of(I, fwd_piter) p(input.size());

	bool conv = false;
	double K = 0;
	double modify = 0;

	a_d = clone(input.exact());

	while (conv == false)
	  {
	    clean_ima(a_d.exact());
	    grad = gradient_on_each_side_(a_d);
	    K = compute_capa(a_d);

	    for_all_p(p)
	      {
		modify = 0;

		typename std::vector<double>::const_iterator it;

		for (it = grad[p].value().begin(); it != grad[p].value().end(); it++)
		  modify += *it * diffusion_coefficient(ABS((double)*it), K);

		if (a_d[p] + 0.14 * modify >= 0 and a_d[p] + 0.14 * modify <= 255)
		  a_d[p] = a_d[p] + 0.14 * modify;
	      }

	    std::cout << modify << std::endl;
	    if (not modify)
	      conv = true;
	  };
	return a_d;
    }



    } // end of namespace oln::level::impl


    template <typename I>
    oln_type_of(I, concrete)
      anisotropic_diffusion(const abstract::image<I>& input)
    {
      return impl::anisotropic_diffusion_(input.exact());
    }

  } // end of namespace oln::level

} // end of namespace oln

#endif // ! OLENA_LEVEL_ANISOTROPIC_DIFFUSION_HH
