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

#ifndef OLENA_TRANSFORM_FFT_HH
# define OLENA_TRANSFORM_FFT_HH

# include <oln/config/pconf.hh>

# ifndef HAVE_FFTW
#  error You need the fftw library if you want to use the fft class of Olena \
         (see "www.fftw.org" for more details about fftw and how to get it).
# else

#  include <fftw.h>
#  include <rfftw.h>

#  include <oln/types/optraits.hh>
#  include <oln/types/all.hh>
#  include <oln/basics2d.hh>

namespace oln {

  namespace internal {

    // dispatch traits for fftw
    
    enum fft_dispatch { fft_cplx, fft_real };

    template <class T>
    struct fft_trait
    {
      void ensure() { is_a(optraits<T>, optraits_scalar)::ensure(); }

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

    const image2d<cplx<rect, dfloat> > transformed_image() const
    {
      return trans_im;
    }

    image2d<cplx<rect, dfloat> >& transformed_image()
    {
      return trans_im;
    }

    template <class T1>
    image2d<T1> transformed_image_clipped_magn(const range<dfloat, bounded_u<0, 1>, saturate> c = 1,
					       bool ordered = true) const
    {
      is_a(optraits<T1>, optraits_scalar)::ensure();

      image2d<T1> new_im(trans_im.nrows(), trans_im.ncols());
      dfloat max = 0;

      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col <= trans_im.ncols(); ++col)
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
		new_im(row, col) = (dfloat)optraits<T>::max() *
		  trans_im((row + trans_im.nrows() / 2) % trans_im.nrows(),
			   (col + trans_im.ncols() / 2) % trans_im.ncols()).magn() / (max * c);
	    }
      else
	for (int row = 0; row < new_im.nrows(); ++row)
	  for (int col = 0; col < new_im.ncols(); ++col)
	    {
	      if (trans_im(row, col).magn() >= max * c)
		new_im(row, col) = optraits<T>::max();
	      else 
		new_im(row, col) = (dfloat)optraits<T>::max() *
		  trans_im(row, col).magn() / (max * c);
	    }
      
      return new_im;
    }

    image2d<T> transformed_image_clipped_magn(const range<dfloat, bounded_u<0, 1>,
					      saturate> c = 1, bool ordered = true) const
    {
      return transformed_image_clipped_magn<T>(c, ordered);
    }

    // FIXME : Find a more elegant way to fix range interval on a and b.
    template <class T1>
    image2d<T1> transformed_image_log_magn(const range<dfloat, bounded_u<0, 1000>, saturate> a = 1,
					   const range<dfloat, bounded_u<0, 1000>, saturate> b = 100,
					   bool ordered = true) const
    {
      is_a(optraits<T1>, optraits_scalar)::ensure();

      image2d<T1> new_im(trans_im.nrows(), trans_im.ncols());
      dfloat max = 0;

      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col <= trans_im.ncols(); ++col)
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

    // FIXME : Find a more elegant way to fix boundaries of a and b.
    image2d<T> transformed_image_log_magn(const range<dfloat, bounded_u<0, 1000>, saturate> a = 1,
					  const range<dfloat, bounded_u<0, 1000>, saturate> b = 100,
					  bool ordered = true) const
    {
      return transformed_image_log_magn<T>(a, b, ordered);
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
    fftwnd_plan				p;
    fftwnd_plan				p_inv;
    // FIXME : See with David if we cannot use his 'generic morpher' for trans_im.
    image2d<cplx<R, dfloat> >		trans_im;

  };

  } // end of namespace internal

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
      out = new fftw_complex[original_im.nrows() * (original_im.ncols() / 2 + 1)];

      for (int row = 0; row < original_im.nrows(); ++row)
	for (int col = 0; col < original_im.ncols(); ++col)
	  in[row * original_im.ncols() + col] = original_im(row, col);

      // FIXME : allow user to modify fftw parameters ?
      p = rfftw2d_create_plan(original_im.nrows(), original_im.ncols(), FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
      p_inv = rfftw2d_create_plan(original_im.nrows(), original_im.ncols(), FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);

      trans_im = image2d<cplx<R, dfloat> >(original_im.nrows(), original_im.ncols());
    }

    image2d<cplx<R, dfloat> > transform()
    {
      rfftwnd_one_real_to_complex(p, in, out);

      unsigned denom = trans_im.nrows() * trans_im.ncols();
      int i = 0;
      for (int row = 0; row < trans_im.nrows(); ++row)
	{
	  for (int col = 0; col <= trans_im.ncols() / 2; ++col)
	    {
	      out[i].re /= denom;
	      out[i].im /= denom;
	      trans_im(row, col) = cplx<rect, dfloat>(out[i].re, out[i].im);
	      ++i;
	    }
	  for (int col = trans_im.ncols() - 1; col > trans_im.ncols() / 2; --col)
	    trans_im(row, col) = trans_im(row, trans_im.ncols() - 1 - col);
	}
      return trans_im;
    }

    template <class T1>
    image2d<T1> transform_inv()
    {
      is_a(optraits<T1>, optraits_scalar)::ensure();

      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col <= trans_im.ncols() / 2; ++col)
	  {
	    out[row * (trans_im.ncols() / 2 + 1) + col].re = trans_im(row, col).real();
	    out[row * (trans_im.ncols() / 2 + 1) + col].im = trans_im(row, col).imag();
	  }
      rfftwnd_one_complex_to_real(p_inv, out, in);

      image2d<T1> new_im(trans_im.nrows(), trans_im.ncols());
      int i = 0;
      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols(); ++col)
	  {
	    new_im(row, col) =
	      (in[i] >= optraits<T1>::min() ? (in[i] <= optraits<T1>::max() ? in [i] : optraits<T1>::max()) : optraits<T1>::min());
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

  // FIXME : make some tests.
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
      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols(); ++col)
	  {
	      out[i].re /= denom;
	      out[i].im /= denom;
	      trans_im(row, col) = cplx<rect, dfloat>(out[i].re, out[i].im);
	      ++i;
	  }
      return trans_im;
    }

    template <class T1>
    image2d<cplx<rect, T1> > transform_inv()
    {
      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols(); ++col)
	  {
	    out[row * trans_im.ncols() + col].re = trans_im(row, col).real();
	    out[row * trans_im.ncols() + col].im = trans_im(row, col).imag();
	  }
      fftwnd_one(p_inv, out, in);

      image2d<cplx<rect, T1> > new_im(trans_im.nrows(), trans_im.ncols());
      int i = 0;
      for (int row = 0; row < trans_im.nrows(); ++row)
	for (int col = 0; col < trans_im.ncols(); ++col)
	  {
	    new_im(row, col) = in[i];
	    ++i;
	  }
      return new_im;
    }

    image2d<cplx<rect, T> > transform_inv()
    {
      return transform_inv<rect, T>();
    }

  };

} // end of namespace oln

# endif // HAVE_FFTW

#endif // ! OLENA_TRANSFORM_FFT_HH
