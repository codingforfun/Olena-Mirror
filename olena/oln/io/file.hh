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

#ifndef OLENA_IO_FILE_HH
# define OLENA_IO_FILE_HH

# include <oln/io/stream_wrapper.hh>

# include <fstream>
# include <sys/types.h>
# include <dirent.h>

namespace oln {

  namespace io {

    namespace internal {

      /*! \class stream_wrapper<StreamFile>
      **
      ** Specialized version for StreamFile.
      */
      
      template<>
      struct stream_wrapper<StreamFile>
      {
	static const std::string& 
	name()
	{ 
	  static const std::string name_("file:"); return name_; 
	}

	
	static bool 
	knows_ext(const std::string&) // knows all extensions
	{ 
	  return true; 
	}

	/// Open a input stream on the file named \a name. Return 0 on failure.
	static std::istream* 
	wrap_in(std::string& name)
	{
	  std::ifstream* in = new std::ifstream(name.c_str());
	  if (!in->fail())
	    return in;
	  delete in;
	  return 0;
	}

	/// Open a output stream on the file named \a name. Return 0 on failure.
	static std::ostream* 
	wrap_out(std::string& name)
	{
	  std::ofstream* out = new std::ofstream(name.c_str());
	  if (!out->fail())
	    return out;
	  delete out;
	  return 0;
	}

	/*! \brief Insert in \a names all the files that have the same 
	** suffix as \a name in the \a name or the current directory.
	*/

  	static void 
	find(std::list<std::string>& names, const std::string& name)
	{
	  // extract dir/base from name
	  int pos = name.rfind('/');
	  std::string base(name, pos+1);
	  //FIXME: add a '.' at the end of base if not already
	  std::string dir;
	  if (pos >= 0)
	    dir.assign(name, 0, pos+1);
	  else
	    dir = "./";
	  // add files in dir starting by base.
	  if (DIR* d = opendir(dir.c_str()))
	    {
	      struct dirent* ent;
	      while ((ent = readdir(d)))
		if (!strncmp(base.c_str(), ent->d_name, base.size()))
		  names.push_back(dir + ent->d_name);
	      closedir(d);
	    }
	}

      };

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln


#endif // OLENA_IO_FILE_HH
