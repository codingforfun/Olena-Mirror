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

// FIXME: this file has not been adjusted to the coding style since it
// will be completely rewritten in next release.

# include <oln/config/system.hh>

# ifdef HAVE_FFTW

#  include <fftw.h>
#  include <rfftw.h>

#  include <ntg/basics.hh>
#  include <ntg/all.hh>
#  include <oln/basics2d.hh>

namespace oln {

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

    template <ntg::cplx_representation R, class T>
    struct fft_trait<ntg::cplx<R, T> >
    {
      static const fft_dispatch		which = fft_cplx;
      typedef fftw_complex		fftw_input;
    };

    //
    // _fft<ntg::cplx_representation, T>
    //
    //////////////////////////////////////

    template <class T, ntg::cplx_representation R>
    class _fft
    {
    public:

      const image2d<ntg::cplx<R, ntg::float_d> > transformed_image() const
      {
	return trans_im;
      }

      image2d<ntg::cplx<R, ntg::float_d> >& transformed_image()
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
	    typename image2d<ntg::cplx<R, ntg::float_d> >::iter it(trans_im);

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
      image2d<T1> transformed_image_clipped_magn(const ntg::float_d clip,
						 bool ordered = true) const
      {
	ntg_is_a(T1, ntg::real)::ensure();

	image2d<T1> new_im(trans_im.size());
	ntg::range<ntg::float_d, ntg::bounded_u<0, 1>, ntg::saturate> c(clip);
	ntg::float_d max = 0;
	typename image2d<ntg::cplx<R, ntg::float_d> >::iter_type it(trans_im);

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

      image2d<T> transformed_image_clipped_magn(const ntg::float_d clip,
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
      image2d<T1> transformed_image_log_magn(const ntg::range<ntg::float_d,
					     ntg::bounded_u<0, 1000>,
					     ntg::saturate> a,
					     const ntg::range<ntg::float_d,
					     ntg::bounded_u<0, 1000>,
					     ntg::saturate> b,
					     bool ordered = true) const
      {
	ntg_is_a(T1, ntg::real)::ensure();

	image2d<T1> new_im(trans_im.size());
	ntg::float_d max = 0;
	typename image2d<ntg::cplx<R, ntg::float_d> >::iter_type it(trans_im);

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
      image2d<T> transformed_image_log_magn(const ntg::range<ntg::float_d,
					    ntg::bounded_u<0, 1000>,
					    ntg::saturate> a,
					    const ntg::range<ntg::float_d,
					    ntg::bounded_u<0, 1000>,
					    ntg::saturate> b,
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
      image2d<ntg::cplx<R, ntg::float_d> >		trans_im;

    };

  } // end of namespace internal

  namespace transforms {

    template <class T,
	      ntg::cplx_representation R = ntg::polar,
	      internal::fft_dispatch which = internal::fft_trait<T>::which >
    class fft;

    //
    // fft<T, ntg::cplx_representation, fft_real>
    //
    //////////////////////////////////////

    template <class T, ntg::cplx_representation R>
    class fft<T, R, internal::fft_real> : public internal::_fft<T, R>
    {

    public:

      fft(const image2d<T>& original_im)
      {
	this->in  = new fftw_real[original_im.nrows() * original_im.ncols()];
	this->out = new fftw_complex[original_im.nrows() *
				     (original_im.ncols() / 2 + 1)];

	for (int row = 0; row < original_im.nrows(); ++row)
	  for (int col = 0; col < original_im.ncols(); ++col)
	    this->in[row * original_im.ncols() + col] = original_im(row, col);

	this->p = rfftw2d_create_plan(original_im.nrows(), original_im.ncols(),
				FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
	this->p_inv = rfftw2d_create_plan(original_im.nrows(), original_im.ncols(),
				    FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);

	this->trans_im = image2d<ntg::cplx<R, ntg::float_d> >(original_im.size());
      }

      image2d<ntg::cplx<R, ntg::float_d> > transform()
      {
	rfftwnd_one_real_to_complex(this->p, this->in, this->out);

	unsigned denom = this->trans_im.nrows() * this->trans_im.ncols();
	int i = 0;
	for (int row = 0; row < this->trans_im.nrows(); ++row)
	  for (int col = 0; col <= this->trans_im.ncols() / 2; ++col)
	    {
	      this->out[i].re /= denom;
	      this->out[i].im /= denom;
	      this->trans_im(row, col) = ntg::cplx<ntg::rect, ntg::float_d>(this->out[i].re, 
							     this->out[i].im);
	      ++i;
	    }
	for (int row = 0; row < this->trans_im.nrows(); ++row)
	  for (int col = this->trans_im.ncols() - 1; col > this->trans_im.ncols() / 2; --col)
	    this->trans_im(row, col) = this->trans_im(this->trans_im.nrows() - row - 1,
					  this->trans_im.ncols() - col - 1);
	return this->trans_im;
      }

      template <class T1>
      image2d<T1> transform_inv()
      {
	ntg_is_a(T1, ntg::real)::ensure();

	for (int row = 0; row < this->trans_im.nrows(); ++row)
	  for (int col = 0; col <= this->trans_im.ncols() / 2; ++col)
	    {
	      this->out[row * (this->trans_im.ncols() / 2 + 1) + col].re =
		this->trans_im(row, col).real();
	      this->out[row * (this->trans_im.ncols() / 2 + 1) + col].im =
		this->trans_im(row, col).imag();
	    }
	rfftwnd_one_complex_to_real(this->p_inv, this->out, this->in);

	image2d<T1> new_im(this->trans_im.size());
	int i = 0;
	for (int row = 0; row < this->trans_im.nrows(); ++row)
	  for (int col = 0; col < this->trans_im.ncols(); ++col)
	    {
	      new_im(row, col) = (this->in[i] >= ntg_inf_val(T1) ?
				  (this->in[i] <= ntg_sup_val(T1) ?
				   this->in [i] :
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
    // fft<T, ntg::cplx_representation, fft_cplx>
    //
    //////////////////////////////////////

    template <class T, ntg::cplx_representation R>
    class fft<T, R, internal::fft_cplx> : public internal::_fft<T, R>
    {

    public:

      template <ntg::cplx_representation R1>
      fft(const image2d<ntg::cplx<R1, T> >& original_im)
      {
	this->in = new fftw_complex[original_im.nrows() * original_im.ncols()];
	this->out = new fftw_complex[original_im.nrows() * original_im.ncols()];

	for (int row = 0; row < original_im.nrows(); ++row)
	  for (int col = 0; col < original_im.ncols(); ++col)
	    {
	      this->in[row * original_im.ncols() + col].re =
		original_im(row, col).real();
	      this->in[row * original_im.ncols() + col].im =
		original_im(row, col).imag();
	    }

	this->p = fftw2d_create_plan(original_im.nrows(), original_im.ncols(),
				     FFTW_FORWARD, FFTW_ESTIMATE);
	this->p_inv = fftw2d_create_plan(original_im.nrows(), original_im.ncols(),
					 FFTW_BACKWARD, FFTW_ESTIMATE);

	this->trans_im = image2d<ntg::cplx<ntg::rect, ntg::float_d> >(original_im.size());
      }

      image2d<ntg::cplx<R, ntg::float_d> > transform()
      {
	fftwnd_one(this->p, this->in, this->out);

	unsigned denom = this->trans_im.nrows() * this->trans_im.ncols();
	int i = 0;
	for (int row = 0; row < this->trans_im.nrows(); ++row)
	  for (int col = 0; col < this->trans_im.ncols(); ++col)
	    {
	      this->out[i].re /= denom;
	      this->out[i].im /= denom;
	      this->trans_im(row, col) = ntg::cplx<ntg::rect, ntg::float_d>(this->out[i].re, 
						       this->out[i].im);
	      ++i;
	    }
	return this->trans_im;
      }

      template <class T1>
      image2d<ntg::cplx<R, T1> > transform_inv()
      {
	for (int row = 0; row < this->trans_im.nrows(); ++row)
	  for (int col = 0; col < this->trans_im.ncols(); ++col)
	    {
	      this->out[row * this->trans_im.ncols() + col].re = this->trans_im(row, col).real();
	      this->out[row * this->trans_im.ncols() + col].im = this->trans_im(row, col).imag();
	    }
	fftwnd_one(this->p_inv, this->out, this->in);

	image2d<ntg::cplx<R, T1> > new_im(this->trans_im.size());
	int i = 0;
	for (int row = 0; row < this->trans_im.nrows(); ++row)
	  for (int col = 0; col < this->trans_im.ncols(); ++col)
	    {
	      new_im(row, col) = this->in[i];
	      ++i;
	    }
	return new_im;
      }

      image2d<ntg::cplx<R, T> > transform_inv()
      {
	return transform_inv<T>();
      }

    };

  } // end of namespace transforms
  
} // end of namespace oln

# endif // HAVE_FFTW

#endif // ! OLENA_TRANSFORM_FFT_HH
