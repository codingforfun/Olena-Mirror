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

#ifndef OLENA_IO_PNM_WRITE_COMMON_HXX_
# define OLENA_IO_PNM_WRITE_COMMON_HXX_

# include <oln/core/image.hh>
# include <oln/core/iter.hh>
# include <oln/value/bin.hh>
# include <oln/value/int_u.hh>
# include <oln/value/rgb.hh>
# include <oln/io/base.hh>

namespace oln {
  namespace io {
    namespace internal {

      inline void
      writemany_plain(std::ostream& out, const bin* elt, coord n)
      {
	unsigned stride = 0;
	for (; n; --n)
	  {
	    out.put(*elt++ == true ? '0' : '1');
	    if (++stride >= 70)
	      {
		out << std::endl;
		stride = 0;
	      }
	  }
	out << std::endl;
      }

      template<unsigned N>
      inline void
      writemany_plain(std::ostream& out, const int_u<N>* elt, coord n)
      {
	for (; n; --n)
	  {
	    out << *elt++;
	    out << std::endl;
	  }
      }

      template<unsigned N>
      inline void
      writemany_raw(std::ostream& out, const int_u<N>* elt, coord n)
      {
	out.write((char*)elt, n * sizeof(*elt));
      }

      template<template<unsigned> class color_system>
      inline void
      writemany_plain(std::ostream& out,
		      const color<3, 8, color_system>* elt, coord n)
      {
	for (; n; --n)
	  {
	    out << (*elt)[0] << ' '
		<< (*elt)[1] << ' '
		<< (*elt)[2] << std::endl;
	    ++elt;
	  }
      }

      template<template<unsigned> class color_system>
      inline void
      writemany_raw(std::ostream& out,
		    const color<3, 8, color_system>* elt, coord n)
      {
	out.write((char*)elt, n * sizeof(*elt));
      }

    } // internal
  } // io
} // oln


#endif // OLENA_IO_PNM_WRITE_COMMON_HXX_
