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

#ifndef OLENA_IO_PNM_WRITE_2D_HH_
# define OLENA_IO_PNM_WRITE_2D_HH_

# include <ntg/basics.hh>
# include <oln/core/traverse.hh>
# include <oln/utils/stat.hh>
# include <oln/io/pnm_common.hh>
# include <oln/io/pnm_write_data.hh>
# include <oln/io/pnm_write.hh>

namespace oln {
  
  namespace io {

    namespace internal {

      /*------------------.
      | pnm_write_header2d |
      `------------------*/

      // FIXME: should be in a .cc file ?
      static bool pnm_write_header2d(std::ostream& s, 
				     char type, 
				     const pnm2d_info& info)
      {
	s.put('P'); s.put(type); s.put('\n');
	s << "# Creator: OLENA / Epita-LRDE" << std::endl
	  << info.cols << ' ' << info.rows << std::endl;

	if (info.max_val >= 0)
	  s << info.max_val << std::endl;
	return true;
      }

      /*--------------------.
      | pnm_writer (Binary) |
      `--------------------*/

      template <writer_id W, class I>
      struct pnm_writer<W, 2, PnmBinary, I>
      {
	static const char pnm_id = (W == WritePnmPlain) ? '1' : '4';

	static std::string name()
	{ 
	  static const std::string _name("pnm/P");
	  return _name + pnm_id;
	}
	
	static bool knows_ext(const std::string& ext)
	{ 
	  if (W == WritePnmPlain)
	    return (ext == "ppbm") || (ext == "pbm");
	  return ext == "pbm";
	}

	static bool write(std::ostream& out, const I& im)
	{
	  pnm2d_info info;
	  info.cols = im.ncols();
	  info.rows = im.nrows();
	  info.max_val = -1;

	  if (!pnm_write_header2d(out, pnm_id, info))
	    return false;

	  if (!pnm_write_data<PnmBinary, W>::write(out, im, info))
	    ; // std::clog << "Unable to write data!" << std::endl;
	  return true;
	}
      };

      /*---------------------.
      | pnm_writer (Integer) |
      `---------------------*/

      template <writer_id W, class I>
      struct pnm_writer<W, 2, PnmInteger, I>
      {
	static const char pnm_id = (W == WritePnmPlain) ? '2' : '5';

	static std::string name()
	{ 
	  static const std::string _name("pnm/P");	  
	  return _name + pnm_id;
	}

	static bool knows_ext(const std::string& ext)
	{ 
	  if (W == WritePnmPlain)
	    return (ext == "ppgm") || (ext == "pgm");
	  return ext == "pgm";
	}

	static bool write(std::ostream& out, const I& im)
	{
	  if (ntg_max_val(oln_value_type(I)) > ntg::to_ntg(65535U))
	    return false;

	  pnm2d_info info;
	  info.cols = im.ncols();
	  info.rows = im.nrows();
	  oln::utils::f_minmax<oln_value_type(I)> f;
	  traverse(f, im);
	  info.max_val = f.max();

	  if (!pnm_write_header2d(out, pnm_id, info))
	    return false;

	  if (!pnm_write_data<PnmInteger, W>::write(out, im, info))
	    ; // std::clog << "Unable to write data!" << std::endl;
	  return true;
	}
      };

      /*-----------------------.
      | pnm_writer (Vectorial) |
      `-----------------------*/

      template <writer_id W, class I>
      struct pnm_writer<W, 2, PnmVectorial, I>
      {
	static const char pnm_id = (W == WritePnmPlain) ? '3' : '6';

	static std::string name()
	{ 
	  static const std::string _name("pnm/P");
	  return _name + pnm_id;
	}

	static bool knows_ext(const std::string& ext)
	{ 
	  if (W == WritePnmPlain)
	    return (ext == "pppm") || (ext == "ppm");
	  return ext == "ppm";
	}

	static bool write(std::ostream& out, const I& im)
	{
	  typedef ntg_comp_type(oln_value_type(I)) comp_type;
	  
	  if (!ntg_is_a(comp_type, ntg::unsigned_integer)::ret)
	    return false;
	  
	  if (ntg_nb_comp(oln_value_type(I)) != 3)
	    return false;
	  
	  if (ntg_max_val(comp_type) > ntg::to_ntg(65535U))
	    return false;

	  pnm2d_info info;
	  info.cols = im.ncols();
	  info.rows = im.nrows();
	  info.max_val = ntg_max_val(comp_type);
	  
	  if (!pnm_write_header2d(out, pnm_id, info))
	    return false;
	  
	  if (!pnm_write_data<PnmVectorial, W>::write(out, im, info))
	    ; // std::clog << "Unable to write data!" << std::endl;
	  return true;
	}
      };

    } // internal

  } // io

} // oln

#endif // ! OLENA_IO_PNM_WRITE_2D_HH_
