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

#ifndef OLENA_IO_PNM_READ3D_HXX_
# define OLENA_IO_PNM_READ3D_HXX_

# include <oln/core/image3d.hh>
# include <oln/io/pnm_read_common.hxx>
# include <cstdio>

namespace oln {
  namespace io {
    namespace internal {

      template< typename T >
      bool pnm_read_header3d(std::istream& s, image3d<T>& im, char type,
			     coord& mrows, coord& mcols)
      {
	// check magic
	if (s.get() != 'P' ) return false;
	if (s.get() != type) return false;
	if (s.get() != '\n') return false;

	// skip all comments except 'Mosaic:'
	coord slices = 0;
	mrows = mcols = 0;
	while (s.peek() == '#')
	  {
	    std::string line;
	    std::getline(s, line);
	    sscanf(line.c_str(), "# Mosaic: %d %d %d\n",
		   &slices, &mcols, &mrows);
	  }

	if (slices <= 0
	    || mrows <= 0
	    || mcols <= 0
	    || mrows * mcols < slices)
	  return false;

	// read size
	coord cols, rows;
	s >> cols >> rows;
	cols /= mcols;
	rows /= mrows;
	if (cols <= 0 || rows <= 0) return false;

	// resize image
	im = image3d<T>(slices, rows, cols);

	// skip maxvalue
	if (s.get() != '\n') return false;
	if (type != '1' && type != '4')
	  {
	    std::string line;
	    std::getline(s,line);
	  }
	return true;
      }

      template<>
      struct reader<ReadPnmPlain, image3d<bin> >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P1"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pppm"; }

	static bool read(std::istream& in, image3d<bin>& im)
	{
	  coord mrows, mcols;
	  if (!pnm_read_header3d(in, im, '1', mrows, mcols))
	    return false;
	  bin* empty_row = new bin[im.ncols()];

	  for (coord mrow = 0; mrow < mrows; ++mrow)
	    for (coord nrow = 0; nrow < im.nrows(); ++nrow)
	      for (coord mcol = 0; mcol < mcols; ++mcol)
		{
		  coord slice = mrow * mcols + mcol;
		  if (slice < im.nslices())
		    readmany_plain(in, &im(slice,nrow,0), im.ncols());
		  else
		    readmany_plain(in, empty_row, im.ncols());
		}
	  return true;
	}
      };

      struct reader<ReadPnmPlain, image3d<int_u<8> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P2"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppgm"; }

	static bool read(std::istream& in, image3d<int_u<8> >& im)
	{
	  coord mrows, mcols;
	  if (!pnm_read_header3d(in, im, '2', mrows, mcols))
	    return false;
	  int_u<8>* empty_row = new int_u<8>[im.ncols()];

	  for (coord mrow = 0; mrow < mrows; ++mrow)
	    for (coord nrow = 0; nrow < im.nrows(); ++nrow)
	      for (coord mcol = 0; mcol < mcols; ++mcol)
		{
		  coord slice = mrow * mcols + mcol;
		  if (slice < im.nslices())
		    readmany_plain(in, &im(slice,nrow,0), im.ncols());
		  else
		    readmany_plain(in, empty_row, im.ncols());
		}
	  return true;
	}
      };

      template<template <unsigned> class color_system>
      struct reader<ReadPnmPlain, image3d< color<3, 8, color_system> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P3"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pppm"; }

	static bool read(std::istream& in, 
			 image3d< color<3, 8, color_system > >& im)
	{
	  coord mrows, mcols;
	  if (!pnm_read_header3d(in, im, '3', mrows, mcols))
	    return false;
	  color<3, 8, color_system>* empty_row =
	    new color<3, 8, color_system>[im.ncols()];

	  for (coord mrow = 0; mrow < mrows; ++mrow)
	    for (coord nrow = 0; nrow < im.nrows(); ++nrow)
	      for (coord mcol = 0; mcol < mcols; ++mcol)
		{
		  coord slice = mrow * mcols + mcol;
		  if (slice < im.nslices())
		    readmany_plain(in, &im(slice,nrow,0), im.ncols());
		  else
		    readmany_plain(in, empty_row, im.ncols());
		}
	  return true;
	}
      };

      // FIXME: Add support for pnm/P4.

      template<unsigned N>
      struct reader<ReadPnmRaw, image3d<int_u<N> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P5"); return _name; }

	static bool knows_ext(const std::string& ext)
	{ return ext == "pgm"; }

	static bool read(std::istream& in, image3d<int_u<N> >& im)
	{
	  coord mrows, mcols;
	  if (!pnm_read_header3d(in, im, '5', mrows, mcols))
	    return false;
	  int_u<N>* empty_row = new int_u<N>[im.ncols()];

	  for (coord mrow = 0; mrow < mrows; ++mrow)
	    for (coord nrow = 0; nrow < im.nrows(); ++nrow)
	      for (coord mcol = 0; mcol < mcols; ++mcol)
		{
		  coord slice = mrow * mcols + mcol;
		  if (slice < im.nslices())
		    readmany_raw(in, &im(slice,nrow,0), im.ncols());
		  else
		    readmany_raw(in, empty_row, im.ncols());
		}
	  return true;
	}
      };

      template<template <unsigned> class color_system>
      struct reader<ReadPnmRaw, image3d< color<3, 8, color_system> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P6"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppm"; }

	static bool read(std::istream& in, 
			 image3d< color<3, 8, color_system> >& im)
	{
	  coord mrows, mcols;
	  if (!pnm_read_header3d(in, im, '6', mrows, mcols))
	    return false;
	  color<3, 8, color_system>* empty_row =
	    new color<3, 8, color_system>[im.ncols()];

	  for (coord mrow = 0; mrow < mrows; ++mrow)
	    for (coord nrow = 0; nrow < im.nrows(); ++nrow)
	      for (coord mcol = 0; mcol < mcols; ++mcol)
		{
		  coord slice = mrow * mcols + mcol;
		  if (slice < im.nslices())
		    readmany_plain(in, &im(slice,nrow,0), im.ncols());
		  else
		    readmany_plain(in, empty_row, im.ncols());
		}
	  return true;
	}
      };

    } // internal
  } // io
} // oln


#endif // OLENA_IO_PNM_READ_HXX_
