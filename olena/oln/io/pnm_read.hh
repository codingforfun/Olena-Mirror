// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_IO_PNM_READ_HH
# define OLENA_IO_PNM_READ_HH

# include <mlc/bool.hh>

# include <oln/core/abstract/image_with_dim.hh>
# include <oln/io/pnm_common.hh>
# include <oln/io/pnm_read_data.hh>

# include <ntg/bin.hh>

namespace oln {

  namespace io {

    namespace internal {      

      /*-------------------.
      | default pnm_reader |
      `-------------------*/

      /*! \class pnm_reader
      **
      ** Default version.
      */
      
      template <reader_id R, unsigned Dim, pnm_type V, class I>
      struct pnm_reader
      {
	static std::string 
	name()
	{ 
	  return "-"; 
	}
	
	static bool
	knows_ext(const std::string&)
	{ 
	  return false; 
	}
	
	static bool 
	read(std::istream&, I&)
	{ 
	  return false; 
	}
      };

      /*---------------.
      | reader for pnm |
      `---------------*/

      /*! \class image_reader<ReadPnmPlain, I>
      **
      ** Specialized version for ReadPnmPlain
      */

      template <class I>
      struct image_reader<ReadPnmPlain, I>
	: public pnm_reader<ReadPnmPlain, I::dim, get_pnm_type<I>::ret, I>
      {};
      
      /*! \class image_reader<ReadPnmRaw, I>
      **
      ** Specialized version for ReadPnmRaw
      */

      template <class I>
      struct image_reader<ReadPnmRaw, I>
	: public pnm_reader<ReadPnmRaw, I::dim, get_pnm_type<I>::ret, I>
      {};

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // ! OLENA_IO_PNM_READ_HH
