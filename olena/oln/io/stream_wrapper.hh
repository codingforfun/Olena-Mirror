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

#ifndef OLENA_IO_STREAM_WRAPPER_HH
# define OLENA_IO_STREAM_WRAPPER_HH

# include <oln/config/system.hh>

# include <oln/io/utils.hh>

# include <list>
# include <string>

namespace oln {

  namespace io {

    namespace internal {

      // stream wrappers
    
      enum stream_id { StreamNone = 0,
		       StreamFile = 1,
		       StreamGz   = 2,
		       StreamAny  = 2 };
    
      template< stream_id W >
      struct stream_wrapper
      {
	static const std::string&
	name()
	{ 
	  static const std::string name_("-"); 
	  return name_; 
	}
      
	static bool 
	knows_ext(const std::string&) 
	{ 
	  return false; 
	}
      
	static std::istream* 
	wrap_in(std::string&) 
	{ 
	  return 0; 
	}

	static std::ostream* 
	wrap_out(std::string&) 
	{ 
	  return 0; 
	}
      
	static void 
	find(std::list<std::string>&, const std::string&) 
	{}
      };

      /*---------------------------.
      | stream_wrappers_find_files |
      `---------------------------*/
    
      // Find files compatible with the given root, extension free filename

      template <stream_id W>
      struct stream_wrappers_find_files
      {
	static void 
	doit(std::list<std::string>& names, const std::string& name)
	{
	  stream_wrapper<W>::find(names, name);
	  return stream_wrappers_find_files<stream_id(unsigned(W)-1)>
                   ::doit(names, name);
	}
      };

      // End of the loop

      template <>
      struct stream_wrappers_find_files<StreamNone>
      {
	static void 
	doit(std::list<std::string>&, const std::string&) 
	{}
      };

      /*-----------------------.
      | try_stream_wrappers_in |
      `-----------------------*/

      // Try stream wrappers recursively   
      // On each potential stream format, try to read using Reader

      template<stream_id W, typename T, class Reader>
      struct try_stream_wrappers_in
      {
	static bool 
	by_extension(T& output, const std::string& name, const std::string& ext)
	{
	  if (stream_wrapper<W>::knows_ext(ext))
	    {
	      std::string wrapped_name = name;
	      if (std::istream* in = stream_wrapper<W>::wrap_in(wrapped_name))
		{
		  std::string wrapped_ext = utils::extension(wrapped_name);
		  bool result = Reader::doit(output, *in, wrapped_ext);
		  delete in;
		  if (result)
		    return true;
		}
	    }
	  return try_stream_wrappers_in<stream_id(unsigned(W)-1),T,Reader>
                   ::by_extension(output, name, ext);
	}

	// FIXME: it sounds strange to read wrapped file without
	// matching its extension.
	static bool 
	by_data(T& output, const std::string& name)
	{
	  std::string wrapped_name = name;
	  if (std::istream* in = stream_wrapper<W>::wrap_in(wrapped_name))
	    {
	      std::string wrapped_ext = utils::extension(wrapped_name);
	      bool result = Reader::doit(output, *in, wrapped_ext);
	      delete in;
	      if (result)
		return true;
	    }
	  return try_stream_wrappers_in<stream_id(unsigned(W)-1), T, Reader>
	           ::by_data(output, name);
	}
      };

      // End of loop
    
      template< typename T, class Reader >
      struct try_stream_wrappers_in<StreamNone, T, Reader>
      {
	static bool 
	by_extension(T&, const std::string&, const std::string&)
	{ 
	  return false; 
	}

	static bool 
	by_data(T&, const std::string&) 
	{ 
	  return false; 
	}
      };

      /*------------------------.
      | try_stream_wrappers_out |
      `------------------------*/

      // Try stream wrappers recursively
      // On each potential stream format, try to write using the given Writer

      template<stream_id W, typename T, class Writer>
      struct try_stream_wrappers_out
      {
	static bool 
	by_extension(const T& input, 
		     const std::string& name, 
		     const std::string& ext)
	{
	  if (stream_wrapper<W>::knows_ext(ext))
	    {
	      std::string wrapped_name = name;
	      std::ostream* out = stream_wrapper<W>::wrap_out(wrapped_name);
	      if (out)
		{
		  std::string wrapped_ext = utils::extension(wrapped_name);
		  bool result = Writer::doit(input, *out, wrapped_ext);
		  delete out;
		  if (result)
		    return true;
		}
	    }
	  return try_stream_wrappers_out<stream_id(unsigned(W)-1),T,Writer>
                   ::by_extension(input, name, ext);
	}
      };

      // End of loop
    
      template< typename T, class Reader >
      struct try_stream_wrappers_out<StreamNone, T, Reader>
      {
	static bool 
	by_extension(const T&, const std::string&, const std::string&) 
	{ 
	  return false; 
	}
      };

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // ! OLENA_IO_STREAM_WRAPPER_HH
