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

#ifndef OLENA_IO_PNM_WRITE_HH_
# define OLENA_IO_PNM_WRITE_HH_

# include <ntg/basics.hh>

# include <oln/core/traverse.hh>
# include <oln/utils/stat.hh>
# include <oln/io/pnm_common.hh>
# include <oln/io/pnm_data.hh>

namespace oln {
  
  namespace io {

    namespace internal {

      /*-------------------.
      | default pnm_writer |
      `-------------------*/

      template <writer_id R, unsigned Dim, pnm_type V, class I>
      struct pnm_writer
      {
	static const std::string& name()
	{ return "-"; }
	
	static bool knows_ext(const std::string& ext)
	{ return false; }
	
	static bool write(std::istream& in, I& output)
	{ return false; }
      };

      /*------------------.
      | pnm_write_header2d |
      `------------------*/

      template <class Exact>
      bool pnm_write_header2d(std::ostream& s, 
			      char type, 
			      const abstract::image_with_dim<2, Exact>& im)
      {
	s.put('P'); s.put(type); s.put('\n');
	s << "# Creator: OLENA / Epita-LRDE" << std::endl
	  << im.ncols() << ' ' << im.nrows() << std::endl;

	oln::utils::f_minmax<Value(Exact)> f;
	traverse(f, im);

	if (type != '1' && type != '4')
	  s << f.max() << std::endl;
	return true;
      }

      /*---------------------------.
      | pnm_writer (Plain, Binary) |
      `---------------------------*/

      template <class I>
      struct pnm_writer<WritePnmPlain, 2, PnmBinary, I>
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P1"); return _name; }

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppbm"; }

	static bool write(std::ostream& out, const I& im)
	{
	  if (!pnm_write_header2d(out, '1', im))
	    return false;
	  pnm_write_data_plain_binary(out, im);
	  return true;
	}
      };
      
      /*----------------------------.
      | pnm_writer (Plain, Integer) |
      `----------------------------*/

      template <class I>
      struct pnm_writer<WritePnmPlain, 2, PnmInteger, I>
      {
	static const std::string& name()
	{ static const std::string _name("pnm/P2"); return _name; }

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppgm"; }

	static bool write(std::ostream& out, const I& im)
	{
	  if (ntg_max_val(Value(I)) > ntg::to_ntg(65535U))
	    return false;
	  if (!pnm_write_header2d(out, '2', im))
	    return false;
	  pnm_write_data_plain_integer(out, im);
	  return true;
	}
      };

      /*-----------------.
      | writer and tools |
      `-----------------*/

      template <class I>
      struct writer<WritePnmPlain, I> 
	: public pnm_writer<WritePnmPlain, I::dim, get_pnm_type<I>::ret, I>
      {};
      

    } // internal
  } // io
} // oln


#endif // OLENA_IO_PNM_WRITE_HH_
