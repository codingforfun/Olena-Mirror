// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_IO_UTILS_HH
# define OLENA_IO_UTILS_HH

# include <oln/config/system.hh>

# include <iostream>
# include <fstream>
# include <string>

namespace oln {

  namespace io {

    namespace internal {

      /*!
      ** \brief Utils for io (get extension of a file).
      */
      struct utils
      {

	/*!
	** \brief Return the extension of a filename.
	** \arg name The filename.
	** \return The extension (lower case).
	*/
	static std::string
	extension(const std::string& name)
	{
	  std::string ext;
	  int pos = name.rfind('.');
	  if (pos > 0)
	  {
	    ext.assign(name, pos + 1, name.size() - pos);
	    for (std::string::iterator i = ext.begin(); i != ext.end(); ++i)
	      *i = tolower(*i);
	  }
	  return ext;
	}

      };


      struct pnm_info
      {
	int max_val;
	int rows;
	int cols;
	std::string type;
      };

      bool read_pnm_header(std::ifstream& istr,
			   internal::pnm_info& info,
			   const std::string& name)
      {
	istr.open(name.c_str(), std::ifstream::in);

	if (!istr.is_open())
	  return false;

	std::getline(istr, info.type);

	// skip comments
	while (istr.peek() == '#')
	  {
	    std::string line;
	    std::getline(istr, line);
	  }

	// read size
	istr >> info.cols;
	// skip comments
	while (istr.peek() == '#')
	  {
	    std::string line;
	    std::getline(istr, line);
	  }


	istr >> info.rows;
	// skip comments
	while (istr.peek() == '#')
	  {
	    std::string line;
	    std::getline(istr, line);
	  }

	if (info.cols <= 0 || info.rows <= 0) return false;

	// skip comments
	while (istr.peek() == '#')
	  {
	    std::string line;
	    std::getline(istr, line);
	  }


	// FIXME: it can be either '\n', 'whitespace', ..., not only '\n'!

	// extract or skip maxvalue
	if (istr.get() != '\n') return false;
	if (info.type != "P1" && info.type != "P4")
	  {
	    istr >> info.max_val;
	    if (info.max_val > 65535 ||
		istr.get() != '\n' ||
		info.max_val <= 0)
	      return false;
	  }
	return true;
      }

      bool write_pnm_header(std::ofstream& ostr,
			    const std::string& name,
			    const std::string& type,
			    int ncols,
			    int nrows,
			    int max_val)
      {
	if (max_val > 65535)
	  {
	    std::cerr << "error: can't save " << name
		      << ", data type too large"
		      << std::endl;
	    return false;
	  }

	ostr.open(name.c_str(), std::ofstream::out);

	if (ostr.is_open() == false)
	  {
	    std::cerr << "error: couldn't open " << name << std::endl;
	    return false;
	  }

	ostr << type << std::endl
	     << "# Olena 1.0" << std::endl
	     << ncols << " " << nrows << std::endl;
	if (type != "P1" && type != "P4")
	  ostr << max_val << std::endl;
	return true;
      }


    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // ! OLENA_IO_UTILS_HH
