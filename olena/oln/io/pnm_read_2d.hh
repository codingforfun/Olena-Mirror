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

#ifndef OLENA_IO_PNM_READ_2D_HH_
# define OLENA_IO_PNM_READ_2D_HH_

# include <ntg/bin.hh>
# include <oln/io/pnm_read.hh>

# include <iostream>

namespace oln {

  namespace io {

    namespace internal {      

      /*------------------.
      | pnm_read_header2d |
      `------------------*/

      // FIXME: should be in a .cc file ?
      static bool pnm_read_header2d(std::istream& s, 
				    char type, 
				    pnm2d_info& info)
      {
	// check magic
	if (s.get() != 'P' ) return false;
	if (s.get() != type) return false;
	if (s.get() != '\n') return false;

	// skip comments
	while (s.peek() == '#')
	  {
	    std::string line;
	    std::getline(s,line);
	  }

	// read size
	s >> info.cols >> info.rows;
	if (info.cols <= 0 || info.rows <= 0) return false;

	// FIXME: it can be either '\n', 'whitespace', ..., not only '\n'!

	// extract or skip maxvalue
	if (s.get() != '\n') return false;
	if (type != '1' && type != '4')
	  {
	    s >> info.max_val;
	    if (info.max_val > 65535 || s.get() != '\n')
	      return false;
	  }
	return true;
      }

      /*--------------------.
      | pnm_reader (Binary) |
      `--------------------*/

      template <reader_id R, class I>
      struct pnm_reader<R, 2, PnmBinary, I>
      {
	static const char pnm_id = (R == ReadPnmPlain) ? '1' : '4';

	static std::string name()
	{ 
	  static const std::string _name("pnm/P");
	  return _name + pnm_id;
	}

	static bool knows_ext(const std::string& ext)
	{ 
	  if (R == ReadPnmPlain)
	    return (ext == "ppbm") || (ext == "pbm");
	  return ext == "pbm";
	}

	static bool read(std::istream& in, I& im)
	{
	  pnm2d_info info;
	  if (!pnm_read_header2d(in, pnm_id, info))
	    return false;

	  im = I(info.rows, info.cols);

	  pnm_read_data<PnmBinary, R>::read(in, im, info);
	  return true;
	}
      };

      /*---------------------.
      | pnm_reader (Integer) |
      `---------------------*/

      template <reader_id R, class I>
      struct pnm_reader<R, 2, PnmInteger, I>
      {
	static const char pnm_id = (R == ReadPnmPlain) ? '2' : '5';

	static std::string name()
	{ 
	  static const std::string _name("pnm/P");
	  return _name + pnm_id;
	}

	static bool knows_ext(const std::string& ext)
	{ 
	  if (R == ReadPnmPlain)
	    return (ext == "ppgm") || (ext == "pgm");
	  return ext == "pgm";
	}

	static bool read(std::istream& in, I& im)
	{
	  pnm2d_info info;
	  
	  if (!pnm_read_header2d(in, pnm_id, info))
	    return false;

	  im = I(info.rows, info.cols);

	  // Check that value type is large enough
	  if (ntg::to_ntg(info.max_val) > ntg_max_val(oln_value_type(I)))
	    return false;

	  pnm_read_data<PnmInteger, R>::read(in, im, info);
	  return true;
	}
      };

      /*-----------------------.
      | pnm_reader (Vectorial) |
      `-----------------------*/

      template <reader_id R, class I>
      struct pnm_reader<R, 2, PnmVectorial, I>
      {
	static const char pnm_id = (R == ReadPnmPlain) ? '3' : '6';

	static std::string name()
	{ 
	  static const std::string _name("pnm/P");
	  return _name + pnm_id;
	}

	static bool knows_ext(const std::string& ext)
	{ 
	  if (R == ReadPnmPlain)
	    return (ext == "pppm") || (ext == "ppm");
	  return ext == "ppm";
	}

	static bool read(std::istream& in, I& im)
	{
	  pnm2d_info info;

	  // Components have to be integers
	  if (!ntg_is_a(ntg_comp_type(oln_value_type(I)), ntg::unsigned_integer)::ret)
	    return false;

	  // Must have only 3 components
	  if (ntg_nb_comp(oln_value_type(I)) != 3)
	    return false;
	  
	  if (!pnm_read_header2d(in, pnm_id, info))
	    return false;

	  im = I(info.rows, info.cols);

	  // Check that value type is large enough
	  if (ntg::to_ntg(info.max_val) > ntg_max_val(ntg_comp_type(oln_value_type(I))))
	    return false;

	  pnm_read_data<PnmVectorial, R>::read(in, im, info);
	  return true;
	}
      };

    } // end of internal

  } // end of io

} // end of oln


#endif // ! OLENA_IO_PNM_READ_2D_HH_
