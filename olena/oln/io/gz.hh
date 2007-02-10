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

#ifndef OLENA_IO_GZ_HH
# define OLENA_IO_GZ_HH

# include <oln/io/stream_wrapper.hh>
# include <oln/io/gz_stream.hh>

namespace oln {

  namespace io {

    namespace internal {
      

      /*! \class stream_wrapper<StreamGz>
      **
      ** Specialized version for StreamGz.
      */
      
      template<>
      struct stream_wrapper<StreamGz>
      {
	
	/// Return "gz:"

	static const std::string& 
	name()
	{ 
	  static const std::string name_("gz:"); 
	  return name_; 
	}

	/// Return true if \a ext == "gz" or \a ext == "z'
	
	static bool 
	knows_ext(const std::string& ext)
	{ 
	  return ext == "gz" || ext == "z"; 
	}
	
	
	/// Delete the file extension of \a name.
	static void 
	adjust_name(std::string& name)
	{
	  std::string ext = utils::extension(name);
	  if (knows_ext(ext))
	    name.erase(name.rfind('.'));
	}
	
	/*! \brief Open a input stream on \a name then return it.
	** On failure \a wrap_in returns 0.
	*/

	static std::istream* 
	wrap_in(std::string& name)
	{
	  gz::zifstream* in = new gz::zifstream(name.c_str());
	  if (!in->fail())
	    {
	      adjust_name(name);
	      return in;
	    }
	  delete in;
	  return 0;
	}

	/*! \brief Open a output stream on \a name then return it.
	** On failure \a wrap_out returns 0.
	*/
	static std::ostream* 
	wrap_out(std::string& name)
	{
	  gz::zofstream* out = new gz::zofstream(name.c_str());
	  if (!out->fail())
	    {
	      adjust_name(name);
	      return out;
	    }
	  delete out;
	  return 0;
	}

  	static void 
	find(std::list<std::string>&, const std::string&) 
	{}
      };

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // ! OLENA_IO_GZ_HH
