// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

#include <oln/types/cast.hh>
#include <oln/convol/fast_gaussian_coefficient.hh>
#include <vector>

namespace oln {
  namespace convol {
    namespace fast {
      namespace internal {

	// Recursive filter, works on a line.
	// FIXME: Until something clever is designed, the line is
	// defined by two points (START and FINISH) and a displacement
	// dpoint (D).
	template <class WorkType, class FloatType, class I>
	void _recursivefilter(I& image,
			      const _RecursiveFilterCoef<FloatType>& c,
			      const Point(I)& start,
			      const Point(I)& finish,
			      coord len,
			      const DPoint(I)& d)
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


	  Point(I) current(start + d + d + d + d);
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
	      image[current] = cast::force<Value(I)>(tmp1[i] + tmp2[i]);
	      current += d;
	    }
	}


	template<unsigned dim>
	struct _gaussian {};


	// Gaussian filter for 1D images
	template<>
	struct _gaussian<1>
	{
	  template <class _I, class F> static
	  void
	  doit(image<_I>& _img, const F& coef)
	  {
	    Exact_ref(I, img);

	    // Apply on columns.
	    _recursivefilter<float>(img, coef,
				    Point(I)(0),
				    Point(I)(img.ncols() - 1),
				    img.ncols(),
				    DPoint(I)(1));
	  }
	};


	// Gaussian filter for 2D images
	template<>
	struct _gaussian<2>
	{
	  template <class _I, class F> static
	  void
	  doit(image<_I>& _img, const F& coef)
	  {
	    Exact_ref(I, img);

	    // Apply on rows.
	    for (coord j = 0; j < img.ncols(); ++j)
	      _recursivefilter<float>(img, coef,
				      Point(I)(0, j),
				      Point(I)(img.nrows() - 1, j),
				      img.nrows(),
				      DPoint(I)(1, 0));

	    // Apply on columns.
	    for (coord i = 0; i < img.nrows(); ++i)
	      _recursivefilter<float>(img, coef,
				      Point(I)(i, 0),
				      Point(I)(i, img.ncols() - 1),
				      img.ncols(),
				      DPoint(I)(0, 1));
	  }
	};

	// Gaussian filter for 3D images
	template<>
	struct _gaussian<3>
	{
	  template <class _I, class F> static
	  void
	  doit(image<_I>& _img, const F& coef)
	  {
	    Exact_ref(I, img);

	    // Apply on slices.
	    for (coord j = 0; j < img.nrows(); ++j)
	      for (coord k = 0; k < img.ncols(); ++k)
		_recursivefilter<float>(img, coef,
					Point(I)(0, j, k),
					Point(I)(img.nslices() - 1, j, k),
					img.ncols(),
					DPoint(I)(1, 0, 0));

	    // Apply on rows.
	    for (coord i = 0; i < img.nslices(); ++i)
	      for (coord k = 0; k < img.ncols(); ++k)
		_recursivefilter<float>(img, coef,
					Point(I)(i, 0, k),
					Point(I)(i, img.nrows() - 1, k),
					img.nrows(),
					DPoint(I)(0, 1, 0));

	    // Apply on columns.
	    for (coord i = 0; i < img.nslices(); ++i)
	      for (coord j = 0; j < img.nrows(); ++j)
		_recursivefilter<float>(img, coef,
					Point(I)(i, j, 0),
					Point(I)(i, j, img.ncols() - 1),
					img.ncols(),
					DPoint(I)(0, 0, 1));
	  }
	};

	template <class _C, class _I, class F>
	typename mute<_I, typename convoutput<_C,Value(_I)>::ret>::ret
	_gaussian_common(const conversion<_C>& _c,
			 const image<_I>& _in,
			 const F& coef)
	{
	  Exact_cref(C, c);
	  Exact_cref(I, in);

	  typename mute<_I, sfloat>::ret work_img(in.size());

	  Iter(I) it(in);
	  for_all(it)
	    work_img[it] = cast::force<sfloat>(in[it]);

	  _gaussian<I::dim>::doit(work_img, coef);

	  /* Convert the result image to the user-requested datatype.
	     FIXME: We are making an unnecessary copy in case the
	     user expects a sfloat image.  */
	  typename mute<_I, typename convoutput<_C,Value(_I)>::ret>::ret
	    out_img(in.size());
	  for_all(it)
	    out_img[it] = c(work_img[it]);

	  return out_img;
	}

      } // internal

      template <class C, class _I>
      typename mute<_I, typename convoutput<C,Value(_I)>::ret>::ret
      gaussian(const conversion<C>& c,
	       const image<_I>& _in, sfloat sigma)
      {
	Exact_cref(I, in);

	internal::_RecursiveFilterCoef<float>
	  coef(1.68f, 3.735f,
	       1.783f, 1.723f,
	       -0.6803f, -0.2598f,
	       0.6318f, 1.997f,
	       sigma,
	       internal::_RecursiveFilterCoef<float>::DericheGaussian);

	return internal::_gaussian_common(c, in, coef);
      }

      template <class C, class _I>
      typename mute<_I, typename convoutput<C,Value(_I)>::ret>::ret
      gaussian_derivative(const conversion<C>& c,
			  const image<_I>& _in, sfloat sigma)
      {
	Exact_cref(I, in);

	internal::_RecursiveFilterCoef<float>
	  coef(-0.6472f, -4.531f,
	       1.527f, 1.516f,
	       0.6494f, 0.9557f,
	       0.6719f, 2.072f,
	       sigma,
	       internal::_RecursiveFilterCoef<float>
	       ::DericheGaussianFirstDerivative);

	return internal::_gaussian_common(c, in, coef);
      }

      template <class C, class _I>
      typename mute<_I, typename convoutput<C,Value(_I)>::ret>::ret
      gaussian_second_derivative(const conversion<C>& c,
				 const image<_I>& _in, sfloat sigma)
      {
	Exact_cref(I, in);

	internal::_RecursiveFilterCoef<float>
	  coef(-1.331f, 3.661f,
	       1.24f, 1.314f,
	       0.3225f, -1.738f,
	       0.748f, 2.166f,
	       sigma,
	       internal::_RecursiveFilterCoef<float>
	       ::DericheGaussianSecondDerivative);

	return internal::_gaussian_common(c, in, coef);
      }

    } // fast
  } // convol
} // oln
