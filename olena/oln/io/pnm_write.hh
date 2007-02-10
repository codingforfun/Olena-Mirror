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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_IO_PNM_WRITE_HH
# define OLENA_IO_PNM_WRITE_HH

# include <oln/io/image_base.hh>
# include <oln/io/pnm_common.hh>

namespace oln {
  
  namespace io {

    namespace internal {

      /*-------------------.
      | default pnm_writer |
      `-------------------*/

      /*! \class pnm_write
      **
      ** Default version.
      */
      
      template <writer_id W, unsigned Dim, pnm_type V, class I>
      struct pnm_writer
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
	write(std::ostream&, const I&)
	{ 
	  return false;
	}
      };

      /*-----------------.
      | writer and tools |
      `-----------------*/

      template <class I>
      struct image_writer<WritePnmPlain, I> 
	: public pnm_writer<WritePnmPlain, I::dim, get_pnm_type<I>::ret, I>
      {};

      template <class I>
      struct image_writer<WritePnmRaw, I> 
	: public pnm_writer<WritePnmRaw, I::dim, get_pnm_type<I>::ret, I>
      {};
      
    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln


#endif // OLENA_IO_PNM_WRITE_HH
