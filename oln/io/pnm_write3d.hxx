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

#ifndef OLENA_IO_PNM_WRITE3D_HXX_
# define OLENA_IO_PNM_WRITE3D_HXX_

# include <oln/io/pnm_write_common.hxx>

namespace oln {
  namespace io {
    namespace internal {

      using oln::internal::image3d_data;

      inline
      void mosaic(coord nslices, coord& mrows, coord& mcols)
      {
	// multiply by 3/4 so that the resulting image fits
	// better on a screen.
	mrows = (coord)roundf(sqrtf((float)nslices * 3.0/4.0));
	mcols = nslices / mrows;
	// Due to rounding we may lack space for a few slices.  So add
	// another row or column (we prefer to add column).
	if (mrows * mcols < nslices)
	  if (mrows * (mcols + 1) >= nslices)
	    ++mcols;
	  else
	    ++mrows;
	assert(mrows * mcols >= nslices);
      }

      template<class U, typename T>
      bool pnm_write_header3d(std::ostream& s, const image3d<T>& im, char type,
			      coord& mrows, coord& mcols)
      {
	mosaic(im.nslices(), mrows, mcols);
	s.put('P'); s.put(type); s.put('\n');
	s << "# Creator: OLENA / Epita-LRDE" << std::endl
	  // We add an extra comment the looks as follow:
	  // # Mosaic: SLICES MCOLS MROWS
	  // where SLICES is the number of slices in the image,
	  // and MCOLS,MROWS the dimension of the mosaic.
	  // (SLICES <= MCOLS * MROWS).
	  // These three values are needed to reconstruct the 3D image.
	  << "# Mosaic: " << im.nslices()
	  << ' ' << mcols << ' ' << mrows << std:: endl
	  << im.ncols() * mcols << ' ' << im.nrows() * mrows << std::endl;
	if (type != '1' && type != '4')
	  s << optraits<U>::max() << std::endl;
	return true;
      }

      template<>
      struct writer<WritePnmPlain, image3d<bin> >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P1"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pppm"; }

	static bool write(std::ostream& out, const image3d<bin>& im)
	{
	  coord mrows;
	  coord mcols;
	  if (!pnm_write_header3d<bin>(out, im, '1', mrows, mcols))
	    return false;
	  bin* empty_row = new bin[im.ncols()];

	  for (coord mrow = 0; mrow < mrows; ++mrow)
	    for (coord nrow = 0; nrow < im.nrows(); ++nrow)
	      for (coord mcol = 0; mcol < mcols; ++mcol)
		{
		  coord slice = mrow * mcols + mcol;
		  writemany_plain(out, (slice < im.nslices()
					? &im.data()->at(slice,nrow,0)
					: empty_row),
				  im.ncols());
		}
	  delete[] empty_row;
	  return true;
	}
      };

      template<unsigned N>
      struct writer<WritePnmPlain, image3d< int_u<N> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P2"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppgm"; }

	static bool write(std::ostream& out, const image3d< int_u<N> >& im)
	{
	  coord mrows;
	  coord mcols;
	  if (!pnm_write_header3d<int_u<N> >(out, im, '2', mrows, mcols))
	    return false;
	  int_u<N>* empty_row = new int_u<N>[im.ncols()];

	  for (coord mrow = 0; mrow < mrows; ++mrow)
	    for (coord nrow = 0; nrow < im.nrows(); ++nrow)
	      for (coord mcol = 0; mcol < mcols; ++mcol)
		{
		  coord slice = mrow * mcols + mcol;
		  writemany_plain(out, (slice < im.nslices()
					? &im.data()->at(slice,nrow,0)
					: empty_row),
				  im.ncols());
		}
	  delete[] empty_row;
	  return true;
	}
      };

      template<template <unsigned> class color_system>
      struct writer<WritePnmPlain, image3d< color<3, 8, color_system> > >
      {
	static const std::string& name()
	{ static const std::string _name("ppm/P3"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pppm"; }

	static bool write(std::ostream& out,
			  const image3d< color<3, 8, color_system> >& im)
	{
	  coord mrows;
	  coord mcols;
	  if (!pnm_write_header3d<color<3, 8,
	      color_system> >(out, im, '3', mrows, mcols))
	    return false;
	  color<3, 8, color_system>* empty_row =
	    new color<3, 8, color_system>[im.ncols()];

	  for (coord mrow = 0; mrow < mrows; ++mrow)
	    for (coord nrow = 0; nrow < im.nrows(); ++nrow)
	      for (coord mcol = 0; mcol < mcols; ++mcol)
		{
		  coord slice = mrow * mcols + mcol;
		  writemany_plain(out, (slice < im.nslices()
					? &im.data()->at(slice,nrow,0)
					: empty_row),
				  im.ncols());
		}
	  delete[] empty_row;
	  return true;
	}
      };

      template<unsigned N>
      struct writer<WritePnmRaw, image3d< int_u<N> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P5"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pgm"; }

	static bool write(std::ostream& out, const image3d< int_u<N> >& im)
	{
	  coord mrows;
	  coord mcols;
	  if (!pnm_write_header3d<int_u<N> >(out, im, '5', mrows, mcols))
	    return false;
	  int_u<N>* empty_row = new int_u<N>[im.ncols()];

	  for (coord mrow = 0; mrow < mrows; ++mrow)
	    for (coord nrow = 0; nrow < im.nrows(); ++nrow)
	      for (coord mcol = 0; mcol < mcols; ++mcol)
		{
		  coord slice = mrow * mcols + mcol;
		  writemany_raw(out, (slice < im.nslices()
				      ? &im.data()->at(slice,nrow,0)
				      : empty_row),
				im.ncols());
		}
	  delete[] empty_row;
	  return true;
	}
      };

      template<template <unsigned> class color_system>
      struct writer<WritePnmRaw, image3d< color<3, 8, color_system> > >
      {
	static const std::string& name()
	{ static const std::string _name("ppm/P6"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppm"; }

	static bool write(std::ostream& out,
			  const image3d< color<3, 8, color_system> >& im)
	{
	  coord mrows;
	  coord mcols;
	  if (!pnm_write_header3d<color<3, 8,
	      color_system> >(out, im, '6', mrows, mcols))
	    return false;
	  color<3, 8, color_system>* empty_row =
	    new color<3, 8, color_system>[im.ncols()];

	  for (coord mrow = 0; mrow < mrows; ++mrow)
	    for (coord nrow = 0; nrow < im.nrows(); ++nrow)
	      for (coord mcol = 0; mcol < mcols; ++mcol)
		{
		  coord slice = mrow * mcols + mcol;
		  writemany_plain(out, (slice < im.nslices()
					? &im.data()->at(slice,nrow,0)
					: empty_row),
				  im.ncols());
		}
	  delete[] empty_row;
	  return true;
	}
      };

    } // internal
  } // io
} // oln


#endif // OLENA_IO_PNM_WRITE3D_HXX_
