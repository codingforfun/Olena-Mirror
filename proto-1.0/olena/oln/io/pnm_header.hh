// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLN_IO_PNM_HEADER_HH
# define OLN_IO_PNM_HEADER_HH

namespace oln {


  namespace io {


    namespace pnm {


      struct header
      {
	int max_val;
	int rows;
	int cols;
	std::string type;
      };

      bool read_header(std::istream& istr,
		       header& hdr)
      {
	std::getline(istr, hdr.type);

	hdr.max_val = 1;

	// skip comments
	while (istr.peek() == '#')
	  {
	    std::string line;
	    std::getline(istr, line);
	  }

	// read size
	istr >> hdr.cols;
	// skip comments
	while (istr.peek() == '#')
	  {
	    std::string line;
	    std::getline(istr, line);
	  }


	istr >> hdr.rows;
	// skip comments
	while (istr.peek() == '#')
	  {
	    std::string line;
	    std::getline(istr, line);
	  }

	if (hdr.cols <= 0 || hdr.rows <= 0) return false;

	// skip comments
	while (istr.peek() == '#')
	  {
	    std::string line;
	    std::getline(istr, line);
	  }

	// FIXME: it can be either '\n', 'whitespace', ..., not only '\n'!
	if (istr.get() != '\n') return false;
	// extract or skip maxvalue
	if (hdr.type != "P1" && hdr.type != "P4")
	  {
	    istr >> hdr.max_val;
	    if (hdr.max_val > 65535 ||
		istr.get() != '\n' ||
		hdr.max_val <= 0)
	      return false;
	  }
	return true;
      }

      bool write_header(std::ostream& ostr,
			const std::string& type,
			int ncols,
			int nrows,
			int max_val)
      {
	if (max_val > 65535)
	  return false;

	ostr << type << std::endl
	     << "# Olena 1.0" << std::endl
	     << ncols << " " << nrows << std::endl;
	if (type != "P1" && type != "P4")
	  ostr << max_val << std::endl;
	return true;
      }


    }


  }


}

#endif // ! OLN_IO_PNM_HEADER_HH
