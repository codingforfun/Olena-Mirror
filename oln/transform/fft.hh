// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

// FIXME : WARNING, W.I.P...

#ifndef OLENA_TRANSFORM_FFT_HH
# define OLENA_TRANSFORM_FFT_HH

# include <fftw.h>
# include <rfftw.h>

# include <oln/types/optraits.hh>
# include <oln/types/all.hh>
# include <oln/basics2d.hh>

namespace oln {

  namespace internal {

    // dispatch traits for fftw
    
    enum e_fft_dispatch { fft_cplx, fft_real };

    template <class T>
    struct fft_trait {
      void ensure() { is_a(optraits<T>, optraits_scalar)::ensure(); }
      static const e_fft_dispatch which = fft_real;
    };

    template <cplx_representation R1, class T1>
    struct fft_trait<cplx<R1, T1> > {
      static const e_fft_dispatch which = fft_cplx;
    };

  } // end of namespace internal

  template <class T, internal::e_fft_dispatch which = internal::fft_trait<T>::which >
  struct fft; // FIXME : name might change

  //
  // fft<T, fft_cplx>
  //
  //////////////////////////////////////

  template <class T>
  class fft<T, internal::fft_cplx>
  {

  public:

    template <cplx_representation R>
    fft(const image2d<cplx<R, T> >& original_im)
    {
      in  = new fftw_complex[original_im.nrows() * ima.ncols()];
      out = new fftw_complex[original_im.nrows() * ima.ncols()];

      for (int row = 0; row < original_im.nrows(); ++row)
	for (int col = 0; col < original_im.ncols(); ++col)
	  {
	    in[row * original_im.ncols() + col].re = original_im(row, col).real();
	    in[row * original_im.ncols() + col].im = original_im(row, col).imag();
	  }

      // FIXME : allow user to modify fftw parameters ?
      p = fftw2d_create_plan(original_im.nrows(), original_im.ncols(), FFTW_FORWARD, FFTW_ESTIMATE);
      p_inv = fftw2d_create_plan(original_im.nrows(), original_im.ncols(), FFTW_BACKWARD, FFTW_ESTIMATE);

      trans_im = image2d<cplx<rect, dfloat> >(original_im.nrows(), original_im.ncols());
    }

    image2d<cplx<rect, dfloat> > transform()
    {
      fftwnd_one(p, in, out);

      unsigned denom = trans_im.nrows() * trans_im.ncols();
      int i = 0;
      for (int row = 0; row < input.nrows(); ++row)
	for (int col = 0; col < input.ncols(); ++col)
	  {
	    cplx<rect, dfloat> tmp(out[i].re, out[i].im);
	    trans_im(row, col) = tmp / denom;
	    ++i;
	  }
      return trans_im;
    }

    const image2d<cplx<rect, dfloat> > transformed_image() const
    {
      return trans_im;
    }

    image2d<cplx<rect, dfloat> >& transformed_image()
    {
      return trans_im;
    }

    // FIXME : bool or specific enum for the second parameter ?
    image2d<T> transformed_image_clipped_magn(const range<dfloat, bounded_u<0, 1>, strict> c = 1, bool ordered = true) const
    {
      image2d<T> new_im(trans_im.nrows(), trans_im.ncols());
      dfloat max = 0;

      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols() / 2 + 1; ++col)
	  if (max < trans_im(row, col).magn())
	    max = trans_im(row, col).magn();

      if (ordered)
	for (int row = 0; row < new_im.nrows(); ++row)
	  for (int col = 0; col < new_im.ncols(); ++col)
	    {
	      if (trans_im((row + trans_im.nrows() / 2) % trans_im.nrows(),
			   (col + trans_im.ncols() / 2) % trans_im.ncols()).magn() >= max * c)
		new_im(row, col) = optraits<T>::max();
	      else 
		new_im(row, col) = (dfloat)optraits<T>::max() * trans_im((row + trans_im.nrows() / 2) % trans_im.nrows(),
									 (col + trans_im.ncols() / 2) % trans_im.ncols()).magn() / (max * c);
	    }
      else
	for (int row = 0; row < new_im.nrows(); ++row)
	  for (int col = 0; col < new_im.ncols(); ++col)
	    {
	      if (trans_im(row, col).magn() >= max * c)
		new_im(row, col) = optraits<T>::max();
	      else 
		new_im(row, col) = (dfloat)optraits<T>::max() * trans_im(row, col).magn() / (max * c);
	    }
      
      return new_im;
    }

    // FIXME : bool or specific enum for the third parameter ?
    image2d<T> transformed_image_log_magn(const range<dfloat, bounded_u<0, 1000>, strict> a = 1,
					  const range<dfloat, bounded_u<0, 1000>, strict> b = 100,
					  bool ordered = true) const
    {
      image2d<T> new_im(trans_im.nrows(), trans_im.ncols());
      dfloat max = 0;

      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols() / 2 + 1; ++col)
	  if (max < trans_im(row, col).magn())
	    max = trans_im(row, col).magn();

      if (ordered)
	for (int row = 0; row < new_im.nrows(); ++row)
	  for (int col = 0; col < new_im.ncols(); ++col)
	    new_im(row, col) = log(a + b * trans_im((row + trans_im.nrows() / 2) % trans_im.nrows(),
						    (col + trans_im.ncols() / 2) % trans_im.ncols()).magn()) / log(a + b * max) * optraits<T>::max();
      else
	for (int row = 0; row < new_im.nrows(); ++row)
	  for (int col = 0; col < new_im.ncols(); ++col)
	    new_im(row, col) = log(a + b * trans_im(row, col).magn()) / log(a + b * max) * optraits<T>::max();
      
      return new_im;
    }

    template <cplx_representation R>
    image2d<cplx<R, T> > transform_inv()
    {
      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols() / 2 + 1; ++col)
	  {
	    out[row * (trans_im.ncols() / 2 + 1) + col].re = trans_im(row, col).real();
	    out[row * (trans_im.ncols() / 2 + 1) + col].im = trans_im(row, col).imag();
	  }
      fftwnd_one(pinv, out, in);

      image2d<cplx<R, T> > new_im(trans_im.nrows(), trans_im.ncols());
      int i = 0;
      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols(); ++col)
	  {
	    new_im(row, col) = in[i];
	    ++i;
	  }
      return new_im;
    }

    ~fft()
    {
      delete [] in;
      delete [] out;
      fftwnd_destroy_plan(p);
      fftwnd_destroy_plan(pinv);
    }

  private:

    fftw_complex			*in;
    fftw_complex			*out;
    fftwnd_plan				p;
    fftwnd_plan				p_inv;
    image2d<cplx<rect, dfloat> >	trans_im;

  };

  //
  // fft<T, fft_real>
  //
  //////////////////////////////////////

  template <class T>
  class fft<T, internal::fft_real>
  {

  public:

    fft(const image2d<T>& original_im)
    {
      in  = new fftw_real[original_im.nrows() * original_im.ncols()];
      out = new fftw_complex[original_im.nrows() * (original_im.ncols() / 2 + 1)];

      for (int row = 0; row < original_im.nrows(); ++row)
	for (int col = 0; col < original_im.ncols(); ++col)
	  in[row * original_im.ncols() + col] = original_im(row, col);

      // FIXME : allow user to modify fftw parameters ?
      p = rfftw2d_create_plan(original_im.nrows(), original_im.ncols(), FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
      pinv = rfftw2d_create_plan(original_im.nrows(), original_im.ncols(), FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);

      trans_im = image2d<cplx<rect, dfloat> >(original_im.nrows(), original_im.ncols());
    }

    image2d<cplx<rect, dfloat> > transform()
    {
      rfftwnd_one_real_to_complex(p, in, out);

      unsigned denom = trans_im.nrows() * trans_im.ncols();
      int i = 0;
      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols() / 2 + 1; ++col)
	  {
	    cplx<rect, dfloat> tmp(out[i].re, out[i].im);
	    trans_im(row, col) = trans_im(row, trans_im.ncols() - col - 1) = tmp / denom;
	    i++;
	  }
      return trans_im;
    }

    const image2d<cplx<rect, dfloat> > transformed_image() const
    {
      return trans_im;
    }

    image2d<cplx<rect, dfloat> >& transformed_image()
    {
      return trans_im;
    }

    // FIXME : bool or specific enum for the second parameter ?
    image2d<T> transformed_image_clipped_magn(const range<dfloat, bounded_u<0, 1>, strict> c = 1, bool ordered = true) const
    {
      image2d<T> new_im(trans_im.nrows(), trans_im.ncols());
      dfloat max = 0;

      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols() / 2 + 1; ++col)
	  if (max < trans_im(row, col).magn())
	    max = trans_im(row, col).magn();

      if (ordered)
	for (int row = 0; row < new_im.nrows(); ++row)
	  for (int col = 0; col < new_im.ncols(); ++col)
	    {
	      if (trans_im((row + trans_im.nrows() / 2) % trans_im.nrows(),
			   (col + trans_im.ncols() / 2) % trans_im.ncols()).magn() >= max * c)
		new_im(row, col) = optraits<T>::max();
	      else 
		new_im(row, col) = (dfloat)optraits<T>::max() * trans_im((row + trans_im.nrows() / 2) % trans_im.nrows(),
									 (col + trans_im.ncols() / 2) % trans_im.ncols()).magn() / (max * c);
	    }
      else
	for (int row = 0; row < new_im.nrows(); ++row)
	  for (int col = 0; col < new_im.ncols(); ++col)
	    {
	      if (trans_im(row, col).magn() >= max * c)
		new_im(row, col) = optraits<T>::max();
	      else 
		new_im(row, col) = (dfloat)optraits<T>::max() * trans_im(row, col).magn() / (max * c);
	    }
      
      return new_im;
    }

    // FIXME : bool or specific enum for the third parameter ?
    image2d<T> transformed_image_log_magn(const range<dfloat, bounded_u<0, 1000>, strict> a = 1,
					  const range<dfloat, bounded_u<0, 1000>, strict> b = 100,
					  bool ordered = true) const
    {
      image2d<T> new_im(trans_im.nrows(), trans_im.ncols());
      dfloat max = 0;

      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols() / 2 + 1; ++col)
	  if (max < trans_im(row, col).magn())
	    max = trans_im(row, col).magn();

      if (ordered)
	for (int row = 0; row < new_im.nrows(); ++row)
	  for (int col = 0; col < new_im.ncols(); ++col)
	    new_im(row, col) = log(a + b * trans_im((row + trans_im.nrows() / 2) % trans_im.nrows(),
						    (col + trans_im.ncols() / 2) % trans_im.ncols()).magn()) / log(a + b * max) * optraits<T>::max();
      else
	for (int row = 0; row < new_im.nrows(); ++row)
	  for (int col = 0; col < new_im.ncols(); ++col)
	    new_im(row, col) = log(a + b * trans_im(row, col).magn()) / log(a + b * max) * optraits<T>::max();
      
      return new_im;
    }

    image2d<T> transform_inv()
    {
      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols() / 2 + 1; ++col)
	  {
	    out[row * (trans_im.ncols() / 2 + 1) + col].re = trans_im(row, col).real();
	    out[row * (trans_im.ncols() / 2 + 1) + col].im = trans_im(row, col).imag();
	  }
      rfftwnd_one_complex_to_real(pinv, out, in);

      image2d<T> new_im(trans_im.nrows(), trans_im.ncols());
      int i = 0;
      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols(); ++col)
	  {
	    new_im(row, col) = (in[i] >= optraits<T>::min() ? (in[i] <= optraits<T>::max() ? in [i] : optraits<T>::max()) : optraits<T>::min());
	    ++i;
	  }
      return new_im;
    }

    ~fft()
    {
      delete [] in;
      delete [] out;
      rfftwnd_destroy_plan(p);
      rfftwnd_destroy_plan(pinv);
    }

  private:

    fftw_real				*in;
    fftw_complex			*out;
    fftwnd_plan				p;
    fftwnd_plan				pinv;
    image2d<cplx<rect, dfloat> >	trans_im;

  };

} // end of namespace oln

#endif // ! OLENA_TRANSFORM_FFT_HH
