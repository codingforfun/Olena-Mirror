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
# include <oln/basics.hh>
# include <oln/basics2d.hh>
# include <ntg/all.hh>
# include <oln/convolution/laplacian.hh>
# include <oln/utils/clone.hh>
# include <oln/morpho/gradient_morpho.hh>
# include <oln/level/anisotropic_diffusion.hh>
# include <vector>

# define get_data(STR, ITER, ATTR) STR[ITER].call(ATTR)
# define get_data2(STR, X, Y, ATTR) STR(X,Y).call(ATTR)
# define set_data(STR, ITER, ATTR, VAL) STR[ITER].call(ATTR, VAL)
# define SQUARE(u) u*u

namespace oln {

  namespace inpainting {


    // Fwd decl of the image inpainting computation

    template <typename I>
    oln_type_of(I, concrete)
      image_inpainting(const abstract::image<I>& input);


    namespace impl {


      template <typename I, typename II>
      oln_ch_concrete_type(I, std::vector<double>)
	vect_laplacian_(const abstract::image<I>& lpl,
			const abstract::image<II>& mb)
      {
	oln_ch_concrete_type(I, std::vector<double>) v_lpl(lpl.size(), "laplacian_vector");
	oln_type_of(I, fwd_piter) p(lpl.size());
	oln_type_of(I, point) pt;

	for_all_p(p)
	  {
	    if (mb[p] == false)
	      {
		pt = p;
		set_data(v_lpl, p, &std::vector<double>::push_back,
			 (lpl[oln_type_of(I, point)((coord_t)(pt.row() + 1), pt.col())].value() -
			  lpl[oln_type_of(I, point)((coord_t)(pt.row() - 1), pt.col())].value()) / 2);

		set_data(v_lpl, p, &std::vector<double>::push_back,
			 (lpl[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() + 1))].value() -
			  lpl[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() - 1))].value()) / 2);
	      }
	  }
	return v_lpl;
      }


      template <typename I, typename II>
      oln_ch_concrete_type(I, std::vector<double>)
	compute_direction_(const abstract::image<I>& input,
			   const abstract::image<II>& mb)
      {
	oln_ch_concrete_type(I, std::vector<double>) n(input.size(), "direction_vector");
	oln_type_of(I, fwd_piter) p(input.size());
	oln_type_of(I, point) pt;
	double Iyn, Ixn;

	for_all_p(p)
	  {
	    if (mb[p] == false)
	      {
		pt = p;

		Iyn = (-input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() + 1))].value() +
		  input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() - 1))].value()) / 2;

		Ixn = (input[oln_type_of(I, point)((coord_t)(pt.row() + 1), pt.col())].value() -
		  input[oln_type_of(I, point)((coord_t)(pt.row() - 1), pt.col())].value()) / 2;

		set_data(n, p, &std::vector<double>::push_back, Iyn);
		set_data(n, p, &std::vector<double>::push_back, Ixn);
	      }
	  }
	return n;
      }

      // do the inner product between lpl and n
      template <typename I, typename II, typename III>
      oln_ch_concrete_type(I, double)
	compute_b_(const abstract::image<I>& v_lpl,
		   const abstract::image<II>& n,
		   const abstract::image<III>& mb)
      {
	oln_ch_concrete_type(I, double) b(v_lpl.size(), "beta");
	oln_type_of(I, fwd_piter) p(v_lpl.size());

	for_all_p(p)
	  {
	    if (mb[p] == false)
	      b[p] = (double) get_data(n, p, &std::vector<double>::front) * get_data(v_lpl, p, &std::vector<double>::front) +
		get_data(n, p, &std::vector<double>::back) * get_data(v_lpl, p, &std::vector<double>::back);
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

      template <typename I, typename II>
      oln_type_of(I, concrete)
	compute_grad_(const abstract::image<I>& input,
		      const abstract::image<I>& b,
		      const abstract::image<II>& mb)
      {
	oln_type_of(I, concrete) g(input.size(), "gradient");
	oln_type_of(I, fwd_piter) p(input.size());
	oln_type_of(I, point) pt;

	for_all_p(p)
	  {
	    if (mb[p] == false)
	      {
		pt = p;
		g[p] = 0;

		if (b[p] > 0)
		  {

		    g[p] = (sqrt(SQUARE(min(input[p].value() -
					    input[oln_type_of(I, point)((coord_t)(pt.row() - 1), pt.col())].value(), 0)) +

				 SQUARE(max(input[oln_type_of(I, point)((coord_t)(pt.row() + 1), pt.col())].value() -
					    input[p].value(), 0)) +

				 SQUARE(min(input[p].value() -
					    input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() - 1))].value(), 0)) +

				 SQUARE(max(input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() + 1))].value() -
					    input[p].value(), 0))) / 2);

		    if (g[p] > 10)
		      g[p] = 10;
		  }

		if (b[p] < 0)
		  {
		    g[p] = (sqrt(SQUARE(max(input[p].value() -
					    input[oln_type_of(I, point)((coord_t)(pt.row() - 1), pt.col())].value(), 0)) +

				 SQUARE(min(input[oln_type_of(I, point)((coord_t)(pt.row() + 1), pt.col())].value() -
					    input[p].value(), 0)) +

				 SQUARE(max(input[p].value() -
					    input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() - 1))].value(), 0)) +

				 SQUARE(min(input[oln_type_of(I, point)(pt.row(), (coord_t)(pt.col() + 1))].value() -
					    input[p].value(), 0))) / 2);

		    if (g[p] > 10)
		      g[p] = 10;

		  }
	      }
      }
	return g;
      }

      template <typename T>
      std::ostream& operator<<(std::ostream& ostr, const image2d<std::vector<T> >& ima)
      {
	for (int i = 0; i < 9; i++)
	  {
	    for (int j = 0; j < 9; j++)
	      {
		ostr << "(" << ima(i,j).value()[0] << "," << ima(i,j).value()[1] << ") " << "\t";
		if ((ima(i,j).value()[0] == 0 or ima(i,j).value()[0] == 1) and
		    (ima(i,j).value()[1] == 0 or ima(i,j).value()[1] == 1))
		  ostr << "\t";
	      }

	    ostr << std::endl;
	  }
	return ostr;
      }


      template <typename T>
      std::ostream& operator<<(std::ostream& ostr, const image2d<T>& ima)
      {
	for (int i = 0; i < 9; i++)
	  {
	    for (int j = 0; j < 9; j++)
	      ostr << ima(i,j).value() << "\t";
	    ostr << std::endl;
	  }
	return ostr;
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


      template <typename I, typename II>
      oln_ch_concrete_type(I, ntg::int_u8)
	image_inpainting_(const abstract::image<I>& input,
			  const abstract::image<II>& mask)
      {
	oln_ch_concrete_type(I, ntg::int_u8) res(input.size(), "res");
	oln_type_of(I, concrete) ii(input.size(), "image_inpainting");
	oln_type_of(II, concrete) dil = morpho::erosion(morpho::erosion(mask.exact(), win_c8p()), win_c8p());
	oln_type_of(I, concrete) lpl(input.size(), "laplacian");
	oln_ch_concrete_type(I, std::vector<double>) v_lpl(input.size(), "laplacian_vector");
	oln_ch_concrete_type(I, std::vector<double>) n(input.size(), "direction_vector");
	oln_type_of(I, concrete) g(input.size(), "gradient");
	oln_type_of(I, concrete) b(input.size(), "beta");
	oln_type_of(I, fwd_piter) p(input.size());

	bool conv = false;
	double error = 0;
	double norm = 0;
 	int i = 0;

	ii = clone(input.exact());
	ii = level::anisotropic_diffusion(ii, 1);

	while (conv == false)
	  {
	    clean_ima(ii);
	    error = 0;
	    lpl = convolution::laplacian(ii);
	    v_lpl = vect_laplacian_(lpl, dil);
	    n = compute_direction_(ii, dil);
	    b = compute_b_(v_lpl, n, dil);
	    g = compute_grad_(ii, b, dil);

	    for_all_p(p)
	      {
		if (mask[p] == false)
		  {
 		    ii[p] = ii[p] + 0.1 * (g[p] * b[p] > 0 ? (g[p] / 255.0) * b[p] : (g[p] / 255.0) * b[p]);
		    error += b[p] > 0 ? b[p] : -b[p];
		  }
	      }

 	    std::cerr << "erreur :" << error << std::endl;

	    if (i % 15 == 0)
	      ii = level::anisotropic_diffusion(ii, 1);

  	    if (error < 0.1 or ++i > 10000000)
	      {
		for_all_p(p)
		  {
		    res[p] = ii[p].value() < 0 ? 0 : (ii[p].value() > 255 ? 255 : ii[p].value());
		  }
 		conv = true;
	      }

	  };
	return res;
      }

    } // end of namespace oln::impl::inpainting

    template <typename I, typename II>
    oln_ch_concrete_type(I, ntg::int_u8)
      image_inpainting(const abstract::image<I>& input,
		       const abstract::image2d<II>& mask)
    {
      return impl::image_inpainting_(input.exact(), mask.exact());
    }

  } // end of namespace oln::appli

} // end of namespace oln


#endif // ! OLENA_IMAGE_INPAINTING_HH
