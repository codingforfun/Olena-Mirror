// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

//
// Gaussian filter implementation from
// "Recursively implementing the gaussian and its derivatives"
// Deriche 93 INRIA REPORT
//

#include <ntg/utils/cast.hh>
#include <oln/convol/fast_gaussian_coefficient.hh>
#include <vector>

namespace oln {
  namespace convol {
    namespace fast {
      /*! \brief Internal purpose only.
      */
      namespace internal {

	/*!
	** \brief Recursive filter.
	**
	** Recursive filter, works on a line.
	**
	** \todo FIXME: Until something clever is designed, the line is
	** defined by two points (START and FINISH) and a displacement
	** dpoint (D).
	**
	** \param Worktype Type the algorithm work on.
	** \param FloatType Type of coefficients.
	** \param I Type of image.
	**
	** \arg image Image to process.
	** \arg c Coefficients.
	** \arg start Start of the line.
	** \arg finish End point of the line.
	** \arg len Length of data to process.
	** \arg d Displacement dpoint.
	*/
	template <class WorkType, class FloatType, class I>
	void
	recursivefilter_(I& image,
			 const recursivefilter_coef_<FloatType>& c,
			 const oln_point_type(I)& start,
			 const oln_point_type(I)& finish,
			 coord len,
			 const oln_dpoint_type(I)& d)
	{
	  std::vector<WorkType>	tmp1(len);
	  std::vector<WorkType>	tmp2(len);

	  // The fourth degree approximation implies to have a special
	  // look on the four first points we consider that there is
	  // no signal before 0 (to be discussed)

	  // --
	  // Causal part

	  tmp1[0] =
	    c.n[0]*image[start];

	  tmp1[1] =
  	      c.n[0]*image[start + d]
	    + c.n[1]*image[start]
	    - c.d[1]*tmp1[0];

	  tmp1[2] =
	      c.n[0]*image[start + d + d]
	    + c.n[1]*image[start + d]
	    + c.n[2]*image[start]
	    - c.d[1]*tmp1[1]
	    - c.d[2]*tmp1[0];

	  tmp1[3] =
  	      c.n[0]*image[start + d + d + d]
	    + c.n[1]*image[start + d + d]
	    + c.n[2]*image[start + d]
	    + c.n[3]*image[start]
	    - c.d[1]*tmp1[2] - c.d[2]*tmp1[1]
	    - c.d[3]*tmp1[0];

	  oln_point_type(I) current(start + d + d + d + d);
	  for (coord i = 4; i < len; ++i)
	    {
	      tmp1[i] =
		  c.n[0]*image[current]
		+ c.n[1]*image[current - d]
		+ c.n[2]*image[current - d - d]
 		+ c.n[3]*image[current - d - d - d]
		- c.d[1]*tmp1[i - 1] - c.d[2]*tmp1[i - 2]
		- c.d[3]*tmp1[i - 3] - c.d[4]*tmp1[i - 4];
	      current += d;
	    }

	  // Non causal part

	  tmp2[len - 1] = 0;

	  tmp2[len - 2] =
	    c.nm[1]*image[finish];

	  tmp2[len - 3] =
	      c.nm[1]*image[finish - d]
	    + c.nm[2]*image[finish]
	    - c.dm[1]*tmp2[len-2];

	  tmp2[len - 4] =
  	      c.nm[1]*image[finish - d - d]
	    + c.nm[2]*image[finish - d]
	    + c.nm[3]*image[finish]
	    - c.dm[1]*tmp2[len-3]
	    - c.dm[2]*tmp2[len-2];

	  current = finish - d - d - d ;

	  for (coord i = len - 5; i >= 0; --i)
	    {
	      tmp2[i] =
		  c.nm[1]*image[current]
		+ c.nm[2]*image[current + d]
		+ c.nm[3]*image[current + d + d]
		+ c.nm[4]*image[current + d + d + d]
		- c.dm[1]*tmp2[i+1] - c.dm[2]*tmp2[i+2]
		- c.dm[3]*tmp2[i+3] - c.dm[4]*tmp2[i+4];
	      current -= d;
	    }

	  // Combine results from causal and non-causal parts.

	  current = start;
	  for (coord i = 0; i < len; ++i)
	    {
	      image[current] = ntg::cast::force<oln_value_type(I)>(tmp1[i] + tmp2[i]);
	      current += d;
	    }
	}

	/*!
	** \brief Compute the gaussian filter.
	**
	** Look at specialization for details.
	*/
	template<unsigned dim>
	struct gaussian_ {};


	/*! Gaussian filter for 1D images
	**
	** gaussian_ specialization for 1D images.
	**
	** \param I Exact type of the image.
	** \param F Type of coefficients.
	**
	** \arg img Image to process.
	** \arg coef Coefficients to use.
	*/
	template<>
	struct gaussian_<1>
	{
	  template <class I, class F> static
	  void
	  doit(abstract::image_with_dim<1, I>& img, const F& coef)
	  {

	    // Apply on columns.
	    recursivefilter_<float>(img, coef,
				    oln_point_type(I)(-img.border()),
				    oln_point_type(I)(img.ncols() - 1 + img.border()),
				    img.ncols() + 2 * img.border(),
				    oln_dpoint_type(I)(1));
	  }
	};


	/*! Gaussian filter for 2D images
	**
	** gaussian_ specialization for 2D images.
	**
	** \param I Exact type of the image.
	** \param F Type of coefficients.
	**
	** \arg img Image to process.
	** \arg coef Coefficients to use.
	*/
	template<>
	struct gaussian_<2>
	{
	  template <class I, class F> static
	  void
	  doit(abstract::image_with_dim<2, I>& img, const F& coef)
	  {
	    // Apply on rows.
	    for (coord j = 0; j < img.ncols(); ++j)
	      recursivefilter_<float>(img, coef,
				      oln_point_type(I)(-img.border(), j),
				      oln_point_type(I)(img.nrows() - 1 + img.border(), j),
				      img.nrows() + 2 * img.border(),
				      oln_dpoint_type(I)(1, 0));

	    // Apply on columns.
	    for (coord i = 0; i < img.nrows(); ++i)
	      recursivefilter_<float>(img, coef,
				      oln_point_type(I)(i, -img.border()),
				      oln_point_type(I)(i, img.ncols() - 1 + img.border()),
				      img.ncols() + 2 * img.border(),
 				      oln_dpoint_type(I)(0, 1));
	  }
	};

	/*! Gaussian filter for 3D images
	**
	** gaussian_ specialization for 3D images.
	**
	** \param I Exact type of the image.
	** \param F Type of coefficients.
	**
	** \arg img Image to process.
	** \arg coef Coefficients to use.
	*/
	template<>
	struct gaussian_<3>
	{
	  template <class I, class F> static
	  void
	  doit(abstract::image_with_dim<3, I>& img, const F& coef)
	  {
	    // Apply on slices.
	    for (coord j = 0; j < img.nrows(); ++j)
	      for (coord k = 0; k < img.ncols(); ++k)
		recursivefilter_<float>(img, coef,
					oln_point_type(I)(-img.border(), j, k),
					oln_point_type(I)(img.nslices() - 1 + img.border(), j, k),
					img.ncols() + 2 * img.border(),
					oln_dpoint_type(I)(1, 0, 0));

	    // Apply on rows.
	    for (coord i = 0; i < img.nslices(); ++i)
	      for (coord k = 0; k < img.ncols(); ++k)
		recursivefilter_<float>(img, coef,
					oln_point_type(I)(i, -img.border(), k),
					oln_point_type(I)(i, img.nrows() - 1 + img.border(), k),
					img.nrows() + 2 * img.border(),
					oln_dpoint_type(I)(0, 1, 0));

	    // Apply on columns.
	    for (coord i = 0; i < img.nslices(); ++i)
	      for (coord j = 0; j < img.nrows(); ++j)
		recursivefilter_<float>(img, coef,
					oln_point_type(I)(i, j, -img.border()),
					oln_point_type(I)(i, j, img.ncols() - 1 + img.border()),
					img.ncols() + 2 * img.border(),
					oln_dpoint_type(I)(0, 0, 1));
	  }
	};

	/*!
	** \brief Common  code for filter (derivative,
	** second derivative, etc.).
	**
	** \param C Exact type of the conversion object.
	** \param B Base type of the conversion object.
	** \param I Exact type of the image.
	** \param BE Exact type of the behavior.
	**
	** \arg input_conv Converter object.
	** \arg in Input image.
	** \arg sigma Value of sigma when computing the gaussian.
	** \arg behavior Object to know how to work on borders.
	*/
	template <class C, class B, class I, class F, class BE>
	typename mute<I, typename convoutput<C,B,oln_value_type(I)>::ret>::ret
	gaussian_common_(const convert::abstract::conversion<C,B>& c,
			 const abstract::image<I>& in,
			 const F& coef,
			 ntg::float_s sigma,
			 const abstract::behavior<BE> &behavior)
	{
	  typename mute<I, ntg::float_s>::ret work_img(in.size());

	  oln_iter_type(I) it(in);
	  for_all(it)
	    work_img[it] = ntg::cast::force<ntg::float_s>(in[it]);

	  /* FIXME: relation between sigma and the border shouldn't
	     be linear, so when sigma is big enougth, the signal may
	     be parasitized by the non signal values.
	   */
	  behavior.adapt_border(work_img, ntg::cast::round<coord>(5 * sigma));

	  gaussian_<I::dim>::doit(work_img, coef);

	  /* Convert the result image to the user-requested datatype.
	     FIXME: We are making an unnecessary copy in case the
	     user expects a ntg::float_s image.  */
	  typename mute<I, typename convoutput<C,B,oln_value_type(I)>::ret>::ret
	    out_img(in.size());

	  for_all(it)
	    out_img[it] = c(work_img[it]);

	  return out_img;
	}

      } // internal

      template <class C, class B, class I, class BE>
      typename mute<I, typename convoutput<C,B,oln_value_type(I)>::ret>::ret
      gaussian(const convert::abstract::conversion<C,B>& c,
	       const abstract::image<I>& in, ntg::float_s sigma,
	       const abstract::behavior<BE> &behavior)
      {
	internal::recursivefilter_coef_<float>
	  coef(1.68f, 3.735f,
	       1.783f, 1.723f,
	       -0.6803f, -0.2598f,
	       0.6318f, 1.997f,
	       sigma,
	       internal::recursivefilter_coef_<float>::DericheGaussian);

	return internal::gaussian_common_(c, in, coef, sigma, behavior);
      }

      template <class C, class B, class I, class BE>
      typename mute<I, typename convoutput<C,B,oln_value_type(I)>::ret>::ret
      gaussian_derivative(const convert::abstract::conversion<C,B>& c,
			  const abstract::image<I>& in, ntg::float_s sigma,
			  const abstract::behavior<BE> &behavior)
      {
	internal::recursivefilter_coef_<float>
	  coef(-0.6472f, -4.531f,
	       1.527f, 1.516f,
	       0.6494f, 0.9557f,
	       0.6719f, 2.072f,
	       sigma,
	       internal::recursivefilter_coef_<float>
	       ::DericheGaussianFirstDerivative);

	return internal::gaussian_common_(c, in, coef, sigma, behavior);
      }

      template <class C, class B, class I, class BE>
      typename mute<I, typename convoutput<C,B,oln_value_type(I)>::ret>::ret
      gaussian_second_derivative(const convert::abstract::conversion<C,B>& c,
				 const abstract::image<I>& in, ntg::float_s sigma,
				 const abstract::behavior<BE> &behavior)
      {
	internal::recursivefilter_coef_<float>
	  coef(-1.331f, 3.661f,
	       1.24f, 1.314f,
	       0.3225f, -1.738f,
	       0.748f, 2.166f,
	       sigma,
	       internal::recursivefilter_coef_<float>
	       ::DericheGaussianSecondDerivative);

	return internal::gaussian_common_(c, in, coef, sigma, behavior);
      }

    } // fast
  } // convol
} // oln
