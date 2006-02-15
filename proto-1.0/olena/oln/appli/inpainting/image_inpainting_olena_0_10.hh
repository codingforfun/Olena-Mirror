// Copyright (C) 2001-2006  EPITA Research and Development Laboratory
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

#ifndef OLENA_IMAGE_INPAINTING_HH
# define OLENA_IMAGE_INPAINTING_HH

# include <mlc/is_a.hh>
# include <oln/basics2d.hh>
# include <ntg/all.hh>
# include <oln/convol/laplacian.hh>
# include <oln/morpho/gradient_morpho.hh>
# include <oln/level/anisotropic_diffusion.hh>
# include <ntg/int.hh>
# include <vector>

# define get_data(STR, ITER, ATTR) STR.at(ITER)
# define set_data(STR, ITER, ATTR, VAL) STR[ITER].push_back(VAL)

# define SQUARE(u) (u)*(u)

namespace oln {

  namespace inpainting {


    // Fwd decl of the image inpainting computation

    template <typename T, typename T2>
    image2d<ntg::int_u8>
      image_inpainting(const image2d<T>& input,
		       const image2d<T2>& mask,
		       unsigned nb_it_max = 0);


    namespace impl {


      template <typename T, typename T2>
      image2d<std::vector<double> >
	vect_laplacian_(const image2d<T>& lpl,
			const image2d<T2>& mb)
      {
	image2d<std::vector<double> > v_lpl(lpl.size());
	oln_iter_type(image2d<T>) p(lpl);
	point2d pt;

	for_all(p)
	  {
	    if (mb[p] == false)
	      {
		pt = p;
		set_data(v_lpl, p, &std::vector<double>::push_back,
			 lpl((pt.row() + 1), pt.col()) -
			 lpl((pt.row() - 1), pt.col()));

		set_data(v_lpl, p, &std::vector<double>::push_back,
			 lpl(pt.row(), (pt.col() + 1)) -
			 lpl(pt.row(), (pt.col() - 1)));
	      }
	  }
	return v_lpl;
      }


      template <typename T, typename T2>
      image2d<std::vector<double> >
	compute_direction_(const image2d<T>& input,
			   const image2d<T2>& mb)
      {
	image2d<std::vector<double> > n(input.size());
	oln_iter_type(image2d<T>) p(input);
	point2d pt;
	double Iyn, Ixn;

	for_all(p)
	  {
	    if (mb[p] == false)
	      {
		pt = p;

		Iyn = -input(pt.row(), (pt.col() + 1)) +
		  input(pt.row(), (pt.col() - 1));

		Ixn = input((pt.row() + 1), pt.col()) -
		  input((pt.row() - 1), pt.col());

		set_data(n, p, &std::vector<double>::push_back, Iyn);
		set_data(n, p, &std::vector<double>::push_back, Ixn);
	      }
	  }
	return n;
      }

      // do the inner product between lpl and n
      template <typename T, typename T2, typename T3>
      image2d<double>
	compute_b_(const image2d<T>& v_lpl,
		   const image2d<T2>& n,
		   const image2d<T3>& mb)
      {
	image2d<double> b(v_lpl.size());
	oln_iter_type(image2d<T>) p(v_lpl);
	double norme = 0;

	for_all(p)
	  {
	    if (mb[p] == false)
	      {
		norme = sqrt(SQUARE(n[p].front()) + SQUARE(n[p].back()));

		if (norme == 0)
		  b[p] = (double) n[p].front() * v_lpl[p].front() + n[p].back() * v_lpl[p].back();
		else
		  b[p] = ((double) n[p].front() * v_lpl[p].front() + n[p].back() * v_lpl[p].back()) / norme;
	      }
	  }
	return b;
      }

      double min(double a, double b)
      {
	return a < b ? a : b;
      }


      double max(double a, double b)
      {
	return a > b ? a : b;
      }

      template <typename T, typename T2, typename T3>
      image2d<T>
	compute_grad_(const image2d<T>& input,
		      const image2d<T2>& b,
		      const image2d<T3>& mb)
      {
	image2d<T> g(input.size());
	oln_iter_type(image2d<T>) p(input);
	point2d pt;

	for_all(p)
	  {
	    if (mb[p] == false)
	      {
		pt = p;
		g[p] = 0;

		if (b[p] > 0)
		  {
		    g[p] = (sqrt(SQUARE(min((input[p] -
					     input((pt.row() - 1), pt.col())), 0)) +

				 SQUARE(max((input((pt.row() + 1), pt.col()) -
					     input[p]), 0)) +

				 SQUARE(min((input[p] -
					     input(pt.row(), (pt.col() - 1))), 0)) +

				 SQUARE(max((input(pt.row(), (pt.col() + 1)) -
					     input[p]), 0))));

		    if (g[p] > 10)
		      g[p] = 10;
		  }

		if (b[p] < 0)
		  {
		    g[p] = (sqrt(SQUARE(max((input[p] -
					     input((pt.row() - 1), pt.col())), 0)) +

				 SQUARE(min((input((pt.row() + 1), pt.col()) -
					     input[p]), 0)) +

				 SQUARE(max((input[p] -
					     input(pt.row(), (pt.col() - 1))), 0)) +

				 SQUARE(min((input(pt.row(), (pt.col() + 1)) -
					     input[p]), 0))));

		    if (g[p] > 10)
		      g[p] = 10;

		  }
	      }
	  }
	return g;
      }


      template <typename T, typename T2>
      image2d<ntg::int_u8>
	image_inpainting_(const image2d<T>& input,
			  const image2d<T2>& mask,
			  unsigned nb_it_max)
      {
	image2d<ntg::int_u8> res(input.size());
	image2d<T> ii(input.size());
	image2d<T> save(input.size());
	image2d<T2> dil = morpho::erosion(mask, win_c4p());
	image2d<T2> all(input.size());

	image2d<T> lpl(input.size());
	image2d<std::vector<double> > v_lpl(input.size());
	image2d<std::vector<double> > n(input.size());
	image2d<T> g(input.size());
	image2d<T> b(input.size());
	oln_iter_type(image2d<T>) p(input);

	bool conv = false;
	double error = 0;
	double norm = 0;
 	int i = 1;

	for_all(p)
          {
	    ii[p] = mask[p] == false ? 0 : input[p];
	    all[p] = false;
          }

	ii = level::anisotropic_diffusion(ii, mask, 1500, false);
 	ii = level::anisotropic_diffusion(ii, all, 10, true);

	while (conv == false)
	  {
	    error = 0;
	    lpl = convolution::laplacian(ii);
	    v_lpl = vect_laplacian_(lpl, dil);
	    n = compute_direction_(ii, dil);
	    b = compute_b_(v_lpl, n, dil);
	    g = compute_grad_(ii, b, dil);

	    point2d pt;

	    for_all(p)
	      {
		pt = p;
		if (mask[p] == false)
		  {
		    ii[p] = ii[p] + 0.1 * (g[p] * b[p]) / 255;
		    ii[p] = ii[p] < 1 ? 0 : (ii[p] > 255 ? 255 : ii[p]);
		    error += b[p] > 0 ? b[p] : -b[p];
		  }
	      }

	    std::cerr << "It : " << i << " | Erreur :" << error << std::endl;

	    if (i % 15 == 0)
	      ii = level::anisotropic_diffusion(ii, mask, 2);

	    if (error < 0.1 or ((++i > nb_it_max) and nb_it_max != 0))
	      {
		for_all(p)
		  {
		    res[p] = ii[p] < 0 ? 0 : (ii[p] > 255 ? 255 : ii[p]);
		  }
 		conv = true;
	      }

	  };
	return res;
      }

    } // end of namespace oln::impl::inpainting

    template <typename T, typename T2>
    image2d<ntg::int_u8>
      image_inpainting(const image2d<T>& input,
		       const image2d<T2>& mask,
		       unsigned nb_it_max = 0)
    {
      return impl::image_inpainting_(input, mask, nb_it_max);
    }

  } // end of namespace oln::appli

} // end of namespace oln


#endif // ! OLENA_IMAGE_INPAINTING_HH
