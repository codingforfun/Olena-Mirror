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

#ifndef OLENA_IO_PNM_READ_HXX_
# define OLENA_IO_PNM_READ_HXX_

# include <oln/core/image2d.hh>
# include <oln/core/fwd_iter2d.hh>
# include <oln/io/pnm_read_common.hxx>

# include <ntg/bin.hh>

namespace oln {

  using namespace ntg;

  namespace io {
    namespace internal {

      template <typename T>
      bool pnm_read_header(std::istream& s, image2d<T>& im, char type)
      {
	// check magic
	if (s.get() != 'P' ) return false;
	if (s.get() != type) return false;
	if (s.get() != '\n') return false;

	// skip comments
	while (s.peek() == '#')
	  {
	    std::string line;
	    std::getline(s,line);
	  }

	// read size
	coord cols, rows;
	s >> cols >> rows;
	if (cols <= 0 || rows <= 0) return false;

	// resize image
	im = image2d<T>(rows, cols);

	// skip maxvalue
	if (s.get() != '\n') return false;
	if (type != '1' && type != '4') //FIXME: if (T==bin)
	  {
	    std::string line;
	    std::getline(s,line);
	  }
	return true;
      }

      template<>
      struct reader<ReadPnmPlain, image2d<bin> >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P1"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppbm"; }

	static bool read(std::istream& in, image2d<bin>& im)
	{
	  if (!pnm_read_header(in, im, '1'))
	    return false;
	  if (im.border())
	    for (coord j = 0; j < im.nrows(); ++j)
	      readmany_plain(in, &im(j,0), im.ncols());
	  else
	    readmany_plain(in, &im(0,0), im.ncols() * im.nrows());
	  return true;
	}
      };

      template <unsigned N, class behaviour>
      struct reader<ReadPnmPlain, image2d<int_u<N, behaviour> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P2"); return _name; }

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppgm"; }

	static bool read(std::istream& in, image2d<int_u<N, behaviour> >& im)
	{
	  if (!pnm_read_header(in, im, '2'))
	    return false;
	  if (im.border())
	    for (coord j = 0; j < im.nrows(); ++j)
	      readmany_plain(in, &im(j,0), im.ncols());
	  else
	    readmany_plain(in, &im(0,0), im.ncols() * im.nrows());
	  return true;
	}
      };

      template <template <unsigned> class color_system>
      struct reader<ReadPnmPlain, image2d< color<3, 8, color_system> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P3"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pppm"; }

	static bool read(std::istream& in,
			 image2d< color<3, 8, color_system> >& im)
	{
	  if (!pnm_read_header(in, im, '3'))
	    return false;
	  if (im.border())
	    for (coord j = 0; j < im.nrows(); ++j)
	      readmany_plain(in, &im(j,0), im.ncols());
	  else
	    readmany_plain(in, &im(0,0), im.ncols() * im.nrows());
	  return true;
	}
      };

      template<>
      struct reader<ReadPnmRaw, image2d<bin> >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P4"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pbm"; }

	static bool read(std::istream& in, image2d<bin>& im)
	{
	  if (!pnm_read_header(in, im, '4'))
	    return false;
	  image2d<bin>::iter it(im);
	  coord cols = 0;
	  unsigned bits = 0;
	  unsigned char c = 0;
	  // FIXME: begin and end should be included in namespace olena somewhere
	  for (it = begin; it != end; ++it)
	    {
	      if (bits == 0)
		{
		  c = in.get();
		  bits = 8;
		}
	      im[it] = (c & (1 << --bits)) ? false : true;
	      if (++cols >= im.ncols())
		c = cols = bits = 0;
	    }
	  return true;
	}
      };

      template <unsigned N, class behaviour>
      struct reader<ReadPnmRaw, image2d<int_u<N, behaviour> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P5"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pgm"; }

	static bool read(std::istream& in, image2d<int_u<N, behaviour> >& im)
	{
	  if (!pnm_read_header(in, im, '5'))
	    return false;
	  if (im.border())
	    for (coord j = 0; j < im.nrows(); ++j)
	      readmany_raw(in, &im(j,0), im.ncols());
	  else
	    readmany_raw(in, &im(0,0), im.ncols() * im.nrows());
	  return true;
	}
      };

      template <template <unsigned> class color_system>
      struct reader<ReadPnmRaw, image2d< color<3, 8, color_system> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P6"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppm"; }

	static bool read(std::istream& in,
			 image2d< color<3, 8, color_system> >& im)
	{
	  if (!pnm_read_header(in, im, '6'))
	    return false;
	  if (im.border())
	    for (coord j = 0; j < im.nrows(); ++j)
	      readmany_raw(in, &im(j,0), im.ncols());
	  else
	    readmany_raw(in, &im(0,0), im.ncols() * im.nrows());
	  return true;
	}
      };

    } // internal
  } // io
} // oln


#endif // OLENA_IO_PNM_READ_HXX_
