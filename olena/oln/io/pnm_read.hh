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

#ifndef OLENA_IO_PNM_READ_HH_
# define OLENA_IO_PNM_READ_HH_

# include <mlc/bool.hh>

# include <oln/core/abstract/image_with_dim.hh>
# include <oln/io/pnm_common.hh>
# include <oln/io/pnm_data.hh>

# include <ntg/bin.hh>

namespace oln {

  namespace io {

    namespace internal {      

      /*-------------------.
      | default pnm_reader |
      `-------------------*/

      template <reader_id R, unsigned Dim, pnm_type V, class I>
      struct pnm_reader
      {
	static const std::string& name()
	{ return "-"; }
	
	static bool knows_ext(const std::string& ext)
	{ return false; }
	
	static bool read(std::istream& in, I& output)
	{ return false; }
      };

      /*------------------.
      | pnm_read_header2d |
      `------------------*/

      // FIXME: should return a struct with available informations
      // read_header should not take an image argument !
      bool pnm_read_header2d(std::istream& s, char type, pnm2d_info& info)
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
	if (type != '1' && type != '4') //FIXME: if (T==bin)
	  {
	    s >> info.max_val;
	    if (info.max_val > 65535 || s.get() != '\n')
	      return false;
	  }
	return true;
      }

      /*---------------------------.
      | pnm_reader (Plain, Binary) |
      `---------------------------*/

      template <class I>
      struct pnm_reader<ReadPnmPlain, 2, PnmBinary, I>
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P1"); return _name; }

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppbm"; }

	static bool read(std::istream& in, I& im)
	{
	  pnm2d_info info;
	  if (!pnm_read_header2d(in, '1', info))
	    return false;

	  // FIXME: uncomment when ready
	  //	  im.resize(info.rows, info.cols);
	  im = I(info.rows, info.cols);
	  pnm_read_data_plain_binary(in, im);
	  return true;
	}
      };
      
      /*----------------------------.
      | pnm_reader (Plain, Integer) |
      `----------------------------*/

      template <class I>
      struct pnm_reader<ReadPnmPlain, 2, PnmInteger, I>
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P2"); return _name; }

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppgm"; }

	static bool read(std::istream& in, I& im)
	{
	  pnm2d_info info;
	  
	  if (!pnm_read_header2d(in, '2', info))
	    return false;

	  // FIXME: uncomment when ready
	  // im.resize(info.rows, info.cols);
	  im = I(info.rows, info.cols);

	  // Check that value type is large enough
	  if (ntg::to_ntg(info.max_val) > ntg_max_val(Value(I)))
	    return false;

	  pnm_read_data_plain_integer(in, im);
	  return true;
	}
      };

      /*-------.
      | reader |
      `-------*/

      template <class I>
      struct reader<ReadPnmPlain, I> 
	: public pnm_reader<ReadPnmPlain, I::dim, get_pnm_type<I>::ret, I>
      {};

    } // end of internal

  } // end of io

} // end of oln


#endif // ! OLENA_IO_PNM_READ_HH_
