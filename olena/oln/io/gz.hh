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

#ifndef OLENA_IO_GZ_HH_
# define OLENA_IO_GZ_HH_

# include <oln/io/stream_wrapper.hh>
# include <oln/io/gz_stream.hh>

namespace oln {

  namespace io {

    namespace internal {

      template<>
      struct stream_wrapper<StreamGz>
      {
	static const std::string& name()
	{ static const std::string _name("gz:"); return _name; }

	static bool knows_ext(const std::string& ext)
	{ return ext == "gz" || ext == "z"; }

	static void adjust_name(std::string& name)
	{
	  std::string ext = utils::extension(name);
	  if (knows_ext(ext))
	    name.erase(name.rfind('.'));
	}

	static std::istream* wrap_in(std::string& name)
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

	static std::ostream* wrap_out(std::string& name)
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

  	static void find(std::list<std::string>&, const std::string&) {}
      };

    } // internal

  } // io

} // oln


#endif // OLENA_IO_GZ_HH_
