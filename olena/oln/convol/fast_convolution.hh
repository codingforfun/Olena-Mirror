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


#ifndef OLENA_CONVOL_FAST_CONVOLUTION_HH
# define OLENA_CONVOL_FAST_CONVOLUTION_HH

# include <mlc/cmp.hh>
# include <mlc/array/all.hh>
# include <oln/transforms/fft.hh>
# include <oln/morpher/piece_morpher.hh>
# include <oln/morpher/border_morpher.hh>

namespace oln {

  /*!  \brief Algorithms related to convolution.
  */
  namespace convol {

    /*! \brief Convolution algorithms.
    */
    namespace fast {

      /// Internal namespace
      namespace internal {

	inline const coord
	center_dst(coord n)
	{
	  return n % 2 ? n / 2 + 1 : n / 2;
	}

	inline const coord
	center_src(coord n)
	{
	  return n % 2 ? n / 2 : (n - 1) / 2;
	}

	template <class I, class J, class P>
	const P
	center(const abstract::image<I>& big_ima, const abstract::image<J>& ima)
	{
	  P p;
	  for (unsigned i = 0; i < image_id<I>::dim; i++)
	    p.nth(i) = internal::center_dst(big_ima.size().nth(0)) -
	      internal::center_src(ima.size().nth(0)) - 1;
	  return p;
	}

      } // end of namespace internal

      /*!
      ** \brief Perform a convolution of two images.
      **
      ** \param DestValue Data type of the output image you want.
      ** \param I Exact type of the input image.
      ** \param J Exact type of the input image (convolution kernel).
      **
      ** \arg input The image to process.
      ** \arg k The kernel to convolve with.
      **
      ** \todo FIXME: the kernel could be bigger than the image.
      ** \todo FIXME: supports only 2 dimensions (like the fft).
      **
      ** The algorithm is just the convolution theorem application.
      **
      ** This example compute a fast gaussian convolution :
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/convol/slow_gaussian.hh>
      ** #include <oln/convol/fast_convolution.hh>
      **
      ** using namespace oln;
      ** using namespace ntg;
      **
      ** int main() {
      ** #ifdef HAVE_FFTW
      **   image2d<int_u8> src(IMG_IN "lena.pgm");
      **   float_d sigma = 2.5;
      **   float_d radius = 3;
      **   image2d<float_d> k = convol::slow::gaussian_kernel<2>::kernel(sigma, radius);
      **   image2d<float_d> f_src(src.size());
      **   oln_iter_type_(image2d<float_d>) i(src);
      **   for_all(i)
      **     f_src[i] = float_d(src[i]);
      **   image2d<float_d> tmp = convol::fast::convolve<float_d>(f_src, k);
      **   for_all(i)
      **     src[i] = int_u8(tmp[i]);
      **   save(src, IMG_OUT "oln_convol_fast_convolve.pgm");
      ** #endif
      ** }
      ** \endcode
      ** \image html lena_pgm.png
      ** \image latex lena_pgm.png
      ** =>
      ** \image html oln_convol_fast_convolve.pgm
      ** \image latex oln_convol_fast_convole.pgm
      **
      */
      template<class DestValue, class I, class J>
      typename mute<I, DestValue>::ret
      convolve(const abstract::image< I >& input,
	       const abstract::image< J >& k)
      {
# ifdef HAVE_FFTW
	mlc::eq<I::dim, J::dim>::ensure();
	mlc::eq<I::dim, 2>::ensure();

	oln_size_type(I) big_size;
	coord width_input = 0;
	coord width_k = 0;
	for (unsigned i = 0; i < I::dim; i++)
	{
	  big_size.nth(i) = input.size().nth(i) + k.size().nth(i) - 1;
	  if (width_input < k.size().nth(i) - 1)
	  {
	    width_input = k.size().nth(i) - 1;
	    width_k = input.size().nth(i) - 1;
	  }
	}
	big_size.border() = input.border();

	const morpher::border_morpher< const image2d<oln_value_type(I)>, replicate_behavior<> >
	  big_input(input.exact(), (width_input + 1) / 2, replicate_bhv());

	J big_k(big_size);
	oln_iter_type(J) big_iter(big_k);
	for_all(big_iter)
	  big_k[big_iter] = 0;
	oln_iter_type(J) k_iter(k);
	oln_iter_type(J) input_iter(input);

	morpher::piece_morpher<J>
	  piece_k(big_k, internal::center<J, J, oln_dpoint_type(J)>(big_k, k), k.size());

	for_all(k_iter)
	  piece_k[k_iter] = k[k_iter];

	/// \todo FIXME: unfortunately, fft does not support morphers for now.
	I big_input_(big_input.size());
	for_all(big_iter)
	  big_input_[big_iter] = big_input[big_iter];

	transforms::fft<oln_value_type(I), ntg::rect> tr_input(big_input_.exact());
	transforms::fft<oln_value_type(J), ntg::rect> tr_k(big_k.exact());

	tr_input.transform();
	typename mute<I, ntg::cplx<ntg::rect, ntg::float_d> >::ret&
	  Input = tr_input.transformed_image();
	const typename mute<J, ntg::cplx<ntg::rect, ntg::float_d> >::ret
	  K = tr_k.transform();

	for_all(big_iter) {
	  Input[big_iter] *= K[big_iter];
	  // Scale.
	  Input[big_iter] *= Input.nrows() * Input.ncols();
	}

	typename mute<I, DestValue>::ret big_output = tr_input.shift_transform_inv();
	typename mute<I, DestValue>::ret output(input.size());

	morpher::piece_morpher<J>
	  piece_output(big_output, internal::center<J, J, oln_dpoint_type(J)>(big_input_, input), input.size());
	for_all(input_iter)
	    output[input_iter] = piece_output[input_iter];

	return output;
# else
	assert(0);
# endif // !HAVE_FFTW
      }

      /*!
      ** \brief Perform a convolution of an image and a window.
      **
      ** \param DestValue Data type of the output image you want.
      ** \param I Exact type of the input image.
      ** \param Info Informations about the array.
      ** \param Win Data type of the array.
      **
      ** \arg input The image to process.
      ** \arg arr The array to convolve with.
      **
      ** \todo FIXME: don't use array1d, ..., arraynd.
      */
      template<class DestValue, class I, class Info, class Win>
      typename mute<I, DestValue>::ret
      convolve(const abstract::image < I >& input,
	       const mlc::array2d<Info, Win >& arr)
      {
	return convolve<DestValue>(input, static_cast< image2d<Win> >(arr));
	// FIXME: Should be abstract::w_window<T_arr>.  Adjust #include once done.
      }

    } // end namespace fast

  } // end namespace convol

} // end namespace oln

#endif // OLENA_CONVOL_FAST_CONVOLUTION_HH
