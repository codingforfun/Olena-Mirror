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

#ifndef OLENA_IO_PNM_WRITE_HXX_
# define OLENA_IO_PNM_WRITE_HXX_

# include <ntg/basics.hh>
# include <oln/io/pnm_write_common.hxx>

namespace oln {
  
  using namespace ntg;

  namespace io {
    namespace internal {

      template <class U, typename T>
      bool pnm_write_header(std::ostream& s, const image2d<T>& im, char type)
      {
	s.put('P'); s.put(type); s.put('\n');
	s << "# Creator: OLENA / Epita-LRDE" << std::endl
	  << im.ncols() << ' ' << im.nrows() << std::endl;
	if (type != '1' && type != '4')
	  s << (unsigned) ntg_max_val(U) << std::endl;
	return true;
      }


      template<>
      struct writer<WritePnmPlain, image2d<bin> >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P1"); return _name; }

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppbm"; }

	static bool write(std::ostream& out, const image2d<bin>& im)
	{
	  if (!pnm_write_header<bin>(out, im, '1'))
	    return false;
	  if (im.border())
	    for (coord j = 0; j < im.nrows(); ++j)
	      writemany_plain(out, &im.data()->at(j,0), im.ncols());
	  else
	    writemany_plain(out, &im.data()->at(0,0), im.ncols() * im.nrows());
	  return true;
	}
      };

      template <unsigned N, class behavior>
      struct writer<WritePnmPlain, image2d< int_u<N, behavior> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P2"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppgm"; }

	static bool write(std::ostream& out, const image2d< int_u<N,
			  behavior> >& im)
	{
	  if (!pnm_write_header<int_u<N, behavior> >(out, im, '2'))
	    return false;
	  if (im.border())
	    for (coord j = 0; j < im.nrows(); ++j)
	      writemany_plain(out, &im.data()->at(j,0), im.ncols());
	  else
	    writemany_plain(out, &im.data()->at(0,0), im.ncols() * im.nrows());
	  return true;
	}
      };

      template <template <unsigned> class color_system>
      struct writer<WritePnmPlain, image2d< color<3, 8, color_system> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P3"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pppm"; }

	static bool write(std::ostream& out,
			  const image2d< color<3, 8, color_system> >& im)
	{
	  if (!pnm_write_header<int_u8>(out, im, '3'))
	    return false;
	  if (im.border())
	    for (coord j = 0; j < im.nrows(); ++j)
	      writemany_plain(out, &im.data()->at(j,0), im.ncols());
	  else
	    writemany_plain(out, &im.data()->at(0,0), im.ncols() * im.nrows());
	  return true;
	}
      };

      template<>
      struct writer<WritePnmRaw, image2d<bin> >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P4"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pbm"; }

	static bool write(std::ostream& out, const image2d<bin>& im)
	{
	  if (!pnm_write_header<bin>(out, im, '4'))
	    return false;
	  image2d<bin>::iter it(im);
	  coord cols = 0;
	  unsigned char c = 0;
	  coord stride = 0;
	  for ( it = begin; it != end; ++it )
	    {
	      c <<= 1;
	      c += (im[it] == true ? 0 : 1);
	      if (++cols >= im.ncols())
		{
		  c <<= (8 - stride - 1);
		  out << c;
		  c = cols = stride = 0;
		}
	      else
		if (++stride >= 8)
		  {
		    out << c;
		    c = stride = 0;
		  }
	    }
	  assertion(stride == 0);
	  return true;
	}
      };

      template <unsigned N, class behavior>
      struct writer<WritePnmRaw, image2d< int_u<N, behavior> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P5"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pgm"; }

	static bool write(std::ostream& out, const image2d< int_u<N,
			  behavior> >& im)
	{
	  if (!pnm_write_header<int_u<N, behavior> >(out, im, '5'))
	    return false;
	  if (im.border())
	    for (coord j = 0; j < im.nrows(); ++j)
	      writemany_raw(out, &im.data()->at(j,0), im.ncols());
	  else
	    writemany_raw(out, &im.data()->at(0,0), im.ncols() * im.nrows());
	  return true;
	}
      };

      template <template <unsigned> class color_system>
      struct writer<WritePnmRaw, image2d< color<3, 8, color_system> > >
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P6"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppm"; }

	static bool write(std::ostream& out,
			  const image2d< color<3, 8, color_system> >& im)
	{
	  if (!pnm_write_header<int_u8>(out, im, '6'))
	    return false;
	  if (im.border())
	    for (coord j = 0; j < im.nrows(); ++j)
	      writemany_raw(out, &im.data()->at(j,0), im.ncols());
	  else
	    writemany_raw(out, &im.data()->at(0,0), im.ncols() * im.nrows());
	  return true;
	}
      };

    } // internal
  } // io
} // oln


#endif // OLENA_IO_PNM_WRITE_HXX_
