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

#ifndef OLENA_IO_PNM_READ_COMMON_HXX_
# define OLENA_IO_PNM_READ_COMMON_HXX_

# include <oln/core/image.hh>
# include <oln/core/iter.hh>
# include <ntg/bin.hh>
# include <ntg/optraits_int_u.hh>
# include <ntg/int_u.hh>
# include <ntg/optraits_int_s.hh>
# include <ntg/int_s.hh>

# include <oln/io/base.hh>

namespace oln {

  using namespace ntg;

  namespace io {
    namespace internal {

      inline void
      readmany_plain(std::istream& in, bin* elt, coord n)
      {
	for (; n; --n)
	  {
	    unsigned char c;
	    do
	      {
		c = in.get();
	      }
	    while ((c != '0') && (c != '1'));
	    *elt++ = (c == '0');
	  }
      }

      template< unsigned N, class behaviour >
      inline void
      readmany_plain(std::istream& in, int_u<N, behaviour>* elt, coord n)
      {
	for (; n; --n)
	  {
	    int tmp;
	    in >> tmp;
	    *elt++ = tmp;
	  }
      }

      template< unsigned N, class behaviour >
      inline void
      readmany_raw(std::istream& in, int_u<N, behaviour>* elt, coord n)
      {
	in.read((char*)elt, n * sizeof(*elt));
      }

      template<template <unsigned> class color_system>
      inline void
      readmany_plain(std::istream& in, color<3, 8, color_system>* elt, coord n)
      {
	for (; n; --n)
	  {
	    int tmp;
	    in >> tmp; (*elt)[0] = tmp;
	    in >> tmp; (*elt)[1] = tmp;
	    in >> tmp; (*elt)[2] = tmp;
	    ++elt;
	  }
      }

      template<template <unsigned> class color_system>
      inline void
      readmany_raw(std::istream& in, color<3, 8, color_system>* elt, coord n)
      {
	in.read((char*)elt, n * sizeof(*elt));
      }

    } // internal
  } // io
} // oln


#endif // OLENA_IO_PNM_READ_HXX_
