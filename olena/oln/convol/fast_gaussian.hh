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

#ifndef OLENA_CONVOL_FAST_GAUSSIAN_FILTER_HH__
# define OLENA_CONVOL_FAST_GAUSSIAN_FILTER_HH__

# include <oln/basics.hh>
# include <oln/convert/basics.hh>
# include <ntg/float.hh>
# include <oln/core/behavior.hh>
//
// Gaussian filter implementation from
// "Recursively implementing the gaussian and its derivatives"
// Deriche 93 INRIA REPORT
//

namespace oln {
  namespace convol {
    /*!
    ** \brief oln::convol::fast namespace.
    **
    ** The algorithms you can find here are fast. This mean these ones
    ** are evolved versions.
    */
    namespace fast {

      // FIXME: add tests!

      /*!
      ** \brief Gaussian filter.
      **
      ** Gaussian filter implementation from "Recursively implementing
      ** the gaussian and its derivatives" Deriche 93 INRIA REPORT.
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
      **
      */
      template <class C, class B, class I, class BE>
      typename mute<I, typename convoutput<C, B, oln_value_type(I)>::ret>::ret
      gaussian(const convert::abstract::conversion<C, B>& input_conv,
	       const abstract::image<I>& in, ntg::float_s sigma,
	       const abstract::behavior<BE> &behavior);

      /*!
      ** \brief Derivative gaussian filter.
      **
      ** Gaussian filter implementation from "Recursively implementing
      ** the gaussian and its derivatives" Deriche 93 INRIA REPORT.
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
      **
      */
      template <class C, class B, class I, class BE>
      typename mute<I, typename convoutput<C, B, oln_value_type(I)>::ret>::ret
      gaussian_derivative(const convert::abstract::conversion<C, B>& input_conv,
			  const abstract::image<I>& in, ntg::float_s sigma,
			  const abstract::behavior<BE> &behavior);

      /*!
      ** \brief Second derivative gaussian filter.
      **
      ** Gaussian filter implementation from "Recursively implementing
      ** the gaussian and its derivatives" Deriche 93 INRIA REPORT.
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
      **
      */
      template <class C, class B, class I, class BE>
      typename mute<I, typename convoutput<C, B, oln_value_type(I)>::ret>::ret
      gaussian_second_derivative(const convert::abstract::conversion<C, B>& input_conv,
				 const abstract::image<I>& in,
				 ntg::float_s sigma,
				 const abstract::behavior<BE> &behavior);

      /* Same functions, with a default conversion.  */

      /*
      ** \brief Gaussian filter with a default conversion.
      **
      ** Gaussian filter implementation from "Recursively implementing
      ** the gaussian and its derivatives" Deriche 93 INRIA REPORT.
      **
      ** \param I Exact type of the image.
      ** \param BE Exact type of the behavior.
      **
      ** \arg in Input image.
      ** \arg sigma Value of sigma when computing the gaussian.
      ** \arg behavior Object to know how to work on borders.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/convol/fast_gaussian.hh>
      ** #include <oln/core/behavior.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
      **
      **   oln::save(oln::convol::fast::gaussian(im1, 2,
      **                                         oln::value_behavior<oln_value_type_(im_type)>(0)),
      **             IMG_OUT "oln_convol_fast_gaussian.pbm");
      **   return 0;
      ** }
      ** \endcode
      **
      ** \image html lena256_pgm.png
      ** \image latex lena256_pgm.png
      ** =>
      ** \image html oln_convol_fast_gaussian.png
      ** \image latex oln_convol_fast_gaussian.png
      */
      template <class I, class BE> inline
      oln_concrete_type(I)
      gaussian(const abstract::image<I>& in, ntg::float_s sigma,
	       const abstract::behavior<BE> &behavior)
      { return gaussian(convert::force<oln_value_type(I)>(), in, sigma,
			behavior); }

      /*!
      ** \brief Derivative gaussian filter with a default conversion.
      **
      ** Gaussian filter implementation from "Recursively implementing
      ** the gaussian and its derivatives" Deriche 93 INRIA REPORT.
      **
      ** \param I Exact type of the image.
      ** \param BE Exact type of the behavior.
      **
      ** \arg in Input image.
      ** \arg sigma Value of sigma when computing the gaussian.
      ** \arg behavior Object to know how to work on borders.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/convol/fast_gaussian.hh>
      ** #include <oln/core/behavior.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
      **
      **   oln::save(oln::convol::fast::gaussian(im1, 2,
      **                                         oln::value_behavior<oln_value_type_(im_type)>(0)),
      **             IMG_OUT "oln_convol_fast_gaussian_derivative.pbm");
      **   return  0;
      ** }
      ** \endcode
      **
      ** \image html lena256_pgm.png
      ** \image latex lena256_pgm.png
      ** =>
      ** \image html oln_convol_fast_gaussian_derivative.png
      ** \image latex oln_convol_fast_gaussian_derivative.png
      **
      ** \todo FIXME: Make the example use the good function.
      */
      template <class I, class BE> inline
      oln_concrete_type(I)
      gaussian_derivative(const abstract::image<I>& in, ntg::float_s sigma,
			  const abstract::behavior<BE> &behavior)
      { return gaussian_derivative(convert::force<oln_value_type(I)>(), in, sigma,
				   behavior); }
      /*!
      ** \brief Second derivative gaussian filter with a default conversion.
      **
      ** Gaussian filter implementation from "Recursively implementing
      ** the gaussian and its derivatives" Deriche 93 INRIA REPORT.
      **
      ** \param I Exact type of the image.
      ** \param BE Exact type of the behavior.
      **
      ** \arg in Input image.
      ** \arg sigma Value of sigma when computing the gaussian.
      ** \arg behavior Object to know how to work on borders.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/convol/fast_gaussian.hh>
      ** #include <oln/core/behavior.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
      **
      **   oln::save(oln::convol::fast::gaussian(im1, 2,
      **                                         oln::value_behavior<oln_value_type_(im_type)>(0)),
      **             IMG_OUT "oln_convol_fast_gaussian_second_derivative.pbm");
      **   return 0;
      ** }
      ** \endcode
      **
      ** \image html lena256_pgm.png
      ** \image latex lena256_pgm.png
      ** =>
      ** \image html oln_convol_fast_gaussian_second_derivative.png
      ** \image latex oln_convol_fast_gaussian_second_derivative.png
      **
      ** \todo FIXME: Make the example use the good function.
      */
      template <class I, class BE> inline
      oln_concrete_type(I)
      gaussian_second_derivative(const abstract::image<I>& in,
				 ntg::float_s sigma,
				 const abstract::behavior<BE> &behavior = mirror_behavior<>())
      { return gaussian_second_derivative(convert::force<oln_value_type(I)>(),
					  in, sigma, behavior); }

      /* Same functions, with a default behavior (mirror_behavior). */
      /*!
      ** \brief  Gaussian  filter  with  a default  conversion  and  a
      ** default behavior.
      **
      ** Gaussian filter implementation from "Recursively implementing
      ** the gaussian and its derivatives" Deriche 93 INRIA REPORT.\n
      **
      ** \warning  The content  of the  border  is the  mirror of  the
      ** image.
      **
      ** \param I Exact type of the image.
      **
      ** \arg in Input image.
      ** \arg sigma Value of sigma when computing the gaussian.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/convol/fast_gaussian.hh>
      ** #include <oln/core/behavior.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
      **
      **   oln::save(oln::convol::fast::gaussian(im1, 2),
      **             IMG_OUT "oln_convol_fast_gaussian_default.pbm");
      **   return 0;
      ** }
      ** \endcode
      **
      ** \image html lena256_pgm.png
      ** \image latex lena256_pgm.png
      ** =>
      ** \image html oln_convol_fast_gaussian_default.png
      ** \image latex oln_convol_fast_gaussian_default.png
      */
      template <class I> inline
      oln_concrete_type(I)
	gaussian(const abstract::image<I>& in, ntg::float_s sigma)
      { return gaussian(convert::force<oln_value_type(I)>(), in, sigma,
			mirror_bhv()); }

      /*!
      ** \brief Derivative  gaussian filter with  a default conversion
      ** and a default behavior.
      **
      ** Gaussian filter implementation from "Recursively implementing
      ** the gaussian and its derivatives" Deriche 93 INRIA REPORT.
      **
      ** \warning  The content  of the  border  is the  mirror of  the
      ** image.
      **
      ** \param I Exact type of the image.
      ** \param BE Exact type of the behavior.
      **
      ** \arg in Input image.
      ** \arg sigma Value of sigma when computing the gaussian.
      ** \arg behavior Object to know how to work on borders.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/convol/fast_gaussian.hh>
      ** #include <oln/core/behavior.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
      **
      **   oln::save(oln::convol::fast::gaussian(im1, 2),
      **             IMG_OUT "oln_convol_fast_gaussian_derivative_default.pbm");
      **   return 0;
      ** }
      ** \endcode
      **
      ** \image html lena256_pgm.png
      ** \image latex lena256_pgm.png
      ** =>
      ** \image html oln_convol_fast_gaussian_derivative_default.png
      ** \image latex oln_convol_fast_gaussian_derivative_default.png
      **
      ** \todo FIXME: Make the example use the good function.
      */
      template <class I> inline
      oln_concrete_type(I)
	gaussian_derivative(const abstract::image<I>& in, ntg::float_s sigma)
      { return gaussian_derivative(convert::force<oln_value_type(I)>(), in, sigma,
				   mirror_bhv()); }

      /*!
      ** \brief  Second  derivative  gaussian  filter with  a  default
      ** conversion and a default behavior.
      **
      ** Gaussian filter implementation from "Recursively implementing
      ** the gaussian and its derivatives" Deriche 93 INRIA REPORT.
      **
      ** \warning  The content  of the  border  is the  mirror of  the
      ** image.
      **
      ** \param I Exact type of the image.
      ** \param BE Exact type of the behavior.
      **
      ** \arg in Input image.
      ** \arg sigma Value of sigma when computing the gaussian.
      ** \arg behavior Object to know how to work on borders.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/convol/fast_gaussian.hh>
      ** #include <oln/core/behavior.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
      **
      **   oln::save(oln::convol::fast::gaussian(im1, 2),
      **             IMG_OUT "oln_convol_fast_gaussian_second_derivative_default.pbm");
      **   return 0;
      ** }
      ** \endcode
      **
      ** \image html lena256_pgm.png
      ** \image latex lena256_pgm.png
      ** =>
      ** \image html oln_convol_fast_gaussian_second_derivative_default.png
      ** \image latex oln_convol_fast_gaussian_second_derivative_default.png
      **
      ** \todo FIXME: Make the example use the good function.
      */
      template <class I> inline
      oln_concrete_type(I)
	gaussian_second_derivative(const abstract::image<I>& in, ntg::float_s sigma)
      { return gaussian_second_derivative(convert::force<oln_value_type(I)>(), in, sigma,
					  mirror_bhv()); }
    }
  }
}

# include <oln/convol/fast_gaussian.hxx>

#endif // OLENA_CONVOL_FAST_GAUSSIAN_FILTER_HH__
