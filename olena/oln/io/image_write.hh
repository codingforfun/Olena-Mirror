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

#ifndef OLENA_IO_IMAGE_WRITE_HH_
# define OLENA_IO_IMAGE_WRITE_HH_

# include <oln/config/system.hh>
# include <oln/core/image.hh>

# include <oln/io/image_base.hh>
# include <oln/io/pnm_write.hh>
# include <oln/io/stream_wrapper.hh>
# include <oln/io/utils.hh>

# include <list>
# include <iostream>

namespace oln {

  namespace io {

    namespace internal {
      
      /*------------.
      | try_writers |
      `------------*/

      // Try writers recursively

      template< writer_id W, typename T >
      struct try_writers
      {
	// Try to deduce the file format from the extension
	static bool by_extension(const T& input, std::ostream& out, 
				 const std::string& ext)
	{
	  if (writer<W,T>::knows_ext(ext))
	    if (writer<W,T>::write(out, input))
	      return true;
	  return try_writers<writer_id(unsigned(W)-1), T>
                   ::by_extension(input, out, ext);
	}

	// Try to match the file format referring to the data only
	static bool by_data(const T& input, 
			    std::ostream& out, 
			    const std::string& ext)
	{
	  if (writer<W,T>::write(out, input))
	    {
	      std::clog << "[ambiguous extension '" << ext
			<< "' wrote as " << writer<W,T>::name() << "] " 
			<< std::flush;
	      return true;
	    }
	  return try_writers<writer_id(unsigned(W)-1), T>
                   ::by_data(input, out, ext);
	}
      };

      // End of try_writers loop

      template< typename T >
      struct try_writers<WriteNone, T>
      {
	static bool by_extension(const T&, std::ostream&, const std::string&) 
	{ return false; }

	static bool by_data(const T&, std::ostream&, const std::string&) 
	{ return false; }
      };

      /*----------------------.
      | write(image, filename) |
      `----------------------*/

      // Writers trier functor, helper for stream_wrappers

      struct writers_trier
      {
	template <class T>
	static bool doit(const T& input, 
			 std::ostream& out, 
			 const std::string ext)
	{	  
	  bool result = try_writers<WriteAny,T>::by_extension(input, out, ext);
	  if (!result)
	    result = try_writers<WriteAny,T>::by_data(input, out, ext);
	  return result;
	}
      };

      // Write images

      template <class E>
      inline bool
      write(const abstract::image<E>& input, const std::string& name)
      {
	typedef try_stream_wrappers_out<StreamAny, E, writers_trier> 
	  stream_trier;

	std::string ext = utils::extension(name);

	if (stream_trier::by_extension(input.exact(), name, ext))
	  return true;
	std::clog << "[unable to write '" << name << "'] " << std::endl;
	return false;
      }

    } // end of internal
    
  } // end of io
  
} // end of oln

#endif // ! OLENA_IO_IMAGE_WRITE_HH_
