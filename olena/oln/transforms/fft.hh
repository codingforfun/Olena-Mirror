// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_TRANSFORMS_FFT_HH
# define OLENA_TRANSFORMS_FFT_HH

# include <oln/config/system.hh>

# ifdef HAVE_FFTW

#  include <fftw.h>
#  include <rfftw.h>

#  include <ntg/basics.hh>
#  include <ntg/all.hh>
#  include <oln/basics2d.hh>

namespace oln {

  using namespace ntg;

  namespace internal {

    // dispatch traits for fftw
    
    enum fft_dispatch { fft_cplx, fft_real };

    template <class T>
    struct fft_trait
    {
      void ensure() { ntg_is_a(T, ntg::real)::ensure(); }

      static const fft_dispatch		which = fft_real;
      typedef fftw_real			fftw_input;
    };

    template <cplx_representation R, class T>
    struct fft_trait<cplx<R, T> >
    {
      static const fft_dispatch		which = fft_cplx;
      typedef fftw_complex		fftw_input;
    };

    //
    // _fft<cplx_representation, T>
    //
    //////////////////////////////////////

    template <class T, cplx_representation R>
    class _fft
    {
    public:

      const image2d<cplx<R, float_d> > transformed_image() const
      {
	return trans_im;
      }

      image2d<cplx<R, float_d> >& transformed_image()
      {
	return trans_im;
      }

      template <class T1>
      image2d<T1> transformed_image_magn(bool ordered = true) const
      {
	ntg_is_a(T1, ntg::real)::ensure();

	image2d<T1> new_im(trans_im.size());

	if (ordered)
	  for (int row = 0; row < new_im.nrows(); ++row)
	    for (int col = 0; col < new_im.ncols(); ++col)
	      new_im(row, col) =
		trans_im((row + trans_im.nrows() / 2) % trans_im.nrows(),
			 (col + trans_im.ncols() / 2) % trans_im.ncols()).magn();
	else
	  {
	    typename image2d<cplx<R, float_d> >::iter it(trans_im);

	    for_all(it)
	      new_im[it] = trans_im[it].magn();
	  }

	return new_im;
      }

      image2d<T> transformed_image_magn(bool ordered = true) const
      {
	return transformed_image_magn<T>(ordered);
      }

      template <class T1>
      image2d<T1> transformed_image_clipped_magn(const float_d clip,
						 bool ordered = true) const
      {
	ntg_is_a(T1, ntg::real)::ensure();

	image2d<T1> new_im(trans_im.size());
	range<float_d, bounded_u<0, 1>, saturate> c(clip);
	float_d max = 0;
	typename image2d<cplx<R, float_d> >::iter it(trans_im);

	for_all(it)
	  if (max < trans_im[it].magn())
	    max = trans_im[it].magn();

	if (ordered)
	  for (int row = 0; row < new_im.nrows(); ++row)
	    for (int col = 0; col < new_im.ncols(); ++col)
	      {
		if (trans_im((row + trans_im.nrows() / 2) % trans_im.nrows(),
			     (col + trans_im.ncols() / 2) %
			     trans_im.ncols()).magn() >= max * c)
		  new_im(row, col) = ntg_max_val(T);
		else
		  new_im(row, col) =
		    ntg_max_val(T) *
		    trans_im((row + trans_im.nrows() / 2) % trans_im.nrows(),
			     (col + trans_im.ncols() / 2) %
			     trans_im.ncols()).magn() / (max * c);
	      }
	else
	  for_all(it)
	      {
		if (trans_im[it].magn() >= max * c)
		  new_im[it] = ntg_max_val(T);
		else 
		  new_im[it] = ntg_max_val(T) *
		    trans_im[it].magn() / (max * c);
	      }
      
	return new_im;
      }

      image2d<T> transformed_image_clipped_magn(const float_d clip,
						bool ordered = true) const
      {
	return transformed_image_clipped_magn<T>(clip, ordered);
      }

      template <class T1>
      image2d<T1> transformed_image_clipped_magn(bool ordered = true) const
      {
	return transformed_image_clipped_magn<T1>(1, ordered);
      }

      image2d<T> transformed_image_clipped_magn(bool ordered = true) const
      {
	return transformed_image_clipped_magn<T>(1, ordered);
      }

      // FIXME: Find a more elegant way to fix range interval on a and b.
      template <class T1>
      image2d<T1> transformed_image_log_magn(const range<float_d,
					     bounded_u<0, 1000>,
					     saturate> a,
					     const range<float_d,
					     bounded_u<0, 1000>,
					     saturate> b,
					     bool ordered = true) const
      {
	ntg_is_a(T1, ntg::real)::ensure();

	image2d<T1> new_im(trans_im.size());
	float_d max = 0;
	typename image2d<cplx<R, float_d> >::iter it(trans_im);

	for_all(it)
	  if (max < trans_im[it].magn())
	    max = trans_im[it].magn();

	if (ordered)
	  for (int row = 0; row < new_im.nrows(); ++row)
	    for (int col = 0; col < new_im.ncols(); ++col)
	      new_im(row, col) =
		log(a + b * trans_im((row + trans_im.nrows() / 2) %
				     trans_im.nrows(),
				     (col + trans_im.ncols() / 2) %
				     trans_im.ncols()).magn()) /
		log(a + b * max) * ntg_max_val(T);
	else
	  for_all(it)
	    new_im[it] = log(a + b * trans_im[it].magn()) /
	    log(a + b * max) * ntg_max_val(T);

	return new_im;
      }

      // FIXME: Find a more elegant way to fix boundaries of a and b.
      image2d<T> transformed_image_log_magn(const range<float_d,
					    bounded_u<0, 1000>,
					    saturate> a,
					    const range<float_d,
					    bounded_u<0, 1000>,
					    saturate> b,
					    bool ordered = true) const
      {
	return transformed_image_log_magn<T>(a, b, ordered);
      }

      template <class T1>
      image2d<T1> transformed_image_log_magn(bool ordered = true) const
      {
	return transformed_image_log_magn<T1>(1, 100, ordered);
      }

      image2d<T> transformed_image_log_magn(bool ordered = true) const
      {
	return transformed_image_log_magn<T>(1, 100, ordered);
      }

      ~_fft()
      {
	delete [] in;
	delete [] out;
	fftwnd_destroy_plan(p);
	fftwnd_destroy_plan(p_inv);
      }

    protected:

      typename fft_trait<T>::fftw_input	*in;
      fftw_complex			*out;
      fftwnd_plan			p;
      fftwnd_plan			p_inv;
      image2d<cplx<R, float_d> >		trans_im;

    };

  } // end of namespace internal

  namespace transforms {

    template <class T,
	      cplx_representation R = polar,
	      internal::fft_dispatch which = internal::fft_trait<T>::which >
    class fft;

    //
    // fft<T, cplx_representation, fft_real>
    //
    //////////////////////////////////////

    template <class T, cplx_representation R>
    class fft<T, R, internal::fft_real> : public internal::_fft<T, R>
    {

    public:

      fft(const image2d<T>& original_im)
      {
	in  = new fftw_real[original_im.nrows() * original_im.ncols()];
	out = new fftw_complex[original_im.nrows() *
			       (original_im.ncols() / 2 + 1)];

	for (int row = 0; row < original_im.nrows(); ++row)
	  for (int col = 0; col < original_im.ncols(); ++col)
	    in[row * original_im.ncols() + col] = original_im(row, col);

	p = rfftw2d_create_plan(original_im.nrows(), original_im.ncols(),
				FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
	p_inv = rfftw2d_create_plan(original_im.nrows(), original_im.ncols(),
				    FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);

	trans_im = image2d<cplx<R, float_d> >(original_im.size());
      }

      image2d<cplx<R, float_d> > transform()
      {
	rfftwnd_one_real_to_complex(p, in, out);

	unsigned denom = trans_im.nrows() * trans_im.ncols();
	int i = 0;
	for (int row = 0; row < trans_im.nrows(); ++row)
	  for (int col = 0; col <= trans_im.ncols() / 2; ++col)
	    {
	      out[i].re /= denom;
	      out[i].im /= denom;
	      trans_im(row, col) = cplx<rect, float_d>(out[i].re, out[i].im);
	      ++i;
	    }
	for (int row = 0; row < trans_im.nrows(); ++row)
	  for (int col = trans_im.ncols() - 1; col > trans_im.ncols() / 2; --col)
	    trans_im(row, col) = trans_im(trans_im.nrows() - row - 1,
					  trans_im.ncols() - col - 1);
	return trans_im;
      }

      template <class T1>
      image2d<T1> transform_inv()
      {
	ntg_is_a(T1, ntg::real)::ensure();

	for (int row = 0; row < trans_im.nrows(); ++row)
	  for (int col = 0; col <= trans_im.ncols() / 2; ++col)
	    {
	      out[row * (trans_im.ncols() / 2 + 1) + col].re =
		trans_im(row, col).real();
	      out[row * (trans_im.ncols() / 2 + 1) + col].im =
		trans_im(row, col).imag();
	    }
	rfftwnd_one_complex_to_real(p_inv, out, in);

	image2d<T1> new_im(trans_im.size());
	int i = 0;
	for (int row = 0; row < trans_im.nrows(); ++row)
	  for (int col = 0; col < trans_im.ncols(); ++col)
	    {
	      new_im(row, col) = (in[i] >= ntg_inf_val(T1) ?
				  (in[i] <= ntg_sup_val(T1) ?
				   in [i] :
				   ntg_sup_val(T1)) :
				  ntg_inf_val(T1));
	      ++i;
	    }
	return new_im;
      }

      image2d<T> transform_inv()
      {
	return transform_inv<T>();
      }

    };

    //
    // fft<T, cplx_representation, fft_cplx>
    //
    //////////////////////////////////////

    template <class T, cplx_representation R>
    class fft<T, R, internal::fft_cplx> : public internal::_fft<T, R>
    {

    public:

      template <cplx_representation R1>
      fft(const image2d<cplx<R1, T> >& original_im)
      {
	in  = new fftw_complex[original_im.nrows() * original_im.ncols()];
	out = new fftw_complex[original_im.nrows() * original_im.ncols()];

	for (int row = 0; row < original_im.nrows(); ++row)
	  for (int col = 0; col < original_im.ncols(); ++col)
	    {
	      in[row * original_im.ncols() + col].re =
		original_im(row, col).real();
	      in[row * original_im.ncols() + col].im =
		original_im(row, col).imag();
	    }

	p = fftw2d_create_plan(original_im.nrows(), original_im.ncols(),
			       FFTW_FORWARD, FFTW_ESTIMATE);
	p_inv = fftw2d_create_plan(original_im.nrows(), original_im.ncols(),
				   FFTW_BACKWARD, FFTW_ESTIMATE);

	trans_im = image2d<cplx<rect, float_d> >(original_im.size());
      }

      image2d<cplx<R, float_d> > transform()
      {
	fftwnd_one(p, in, out);

	unsigned denom = trans_im.nrows() * trans_im.ncols();
	int i = 0;
	for (int row = 0; row < trans_im.nrows(); ++row)
	  for (int col = 0; col < trans_im.ncols(); ++col)
	    {
	      out[i].re /= denom;
	      out[i].im /= denom;
	      trans_im(row, col) = cplx<rect, float_d>(out[i].re, out[i].im);
	      ++i;
	    }
	return trans_im;
      }

      template <class T1>
      image2d<cplx<R, T1> > transform_inv()
      {
	for (int row = 0; row < trans_im.nrows(); ++row)
	  for (int col = 0; col < trans_im.ncols(); ++col)
	    {
	      out[row * trans_im.ncols() + col].re = trans_im(row, col).real();
	      out[row * trans_im.ncols() + col].im = trans_im(row, col).imag();
	    }
	fftwnd_one(p_inv, out, in);

	image2d<cplx<R, T1> > new_im(trans_im.size());
	int i = 0;
	for (int row = 0; row < trans_im.nrows(); ++row)
	  for (int col = 0; col < trans_im.ncols(); ++col)
	    {
	      new_im(row, col) = in[i];
	      ++i;
	    }
	return new_im;
      }

      image2d<cplx<R, T> > transform_inv()
      {
	return transform_inv<T>();
      }

    };

  } // end of namespace transforms
  
} // end of namespace oln

# endif // HAVE_FFTW

#endif // ! OLENA_TRANSFORM_FFT_HH
