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

#ifndef OLENA_IO_PNM_WRITE_3D_HH_
# define OLENA_IO_PNM_WRITE_3D_HH_

# include <ntg/bin.hh>
# include <oln/core/image2d.hh>
# include <oln/io/pnm_write_2d.hh>

# include <list>
# include <iostream>
# include <cstdio>

namespace oln {

  namespace io {

    namespace internal {      

      /*-------------------.
      | pnm_writer3d (Any) |
      `-------------------*/

      // Only PnmRaw images can store more than one 2d image. The
      // format is simple: each slice is stored as a bi-dimensional
      // image.

      template <pnm_type P, class I>
      struct pnm_writer<WritePnmRaw, 3, P, I>
      {
	typedef pnm_writer<WritePnmRaw, 2, P, image2d<oln_value_type(I)> > writer_2d;
	typedef image2d<oln_value_type(I)> image2d_type;

	static std::string name()
	{ 
	  return writer_2d::name();
	}

	static bool knows_ext(const std::string& ext)
	{ 
	  return writer_2d::knows_ext(ext);
	}

	static bool write(std::ostream& out, const I& im)
	{
	  for (int slice = 0; slice < im.nslices(); ++slice)
	    {
	      image2d_type tmp_2d (im.nrows(), im.ncols());
	      for (int row = 0; row < im.nrows(); ++row)
		for (int col = 0; col < im.ncols(); ++col)
		  tmp_2d(row,col) = im(slice, row, col);
	      if (!writer_2d::write(out, tmp_2d))
		return false;
	    }
	  return true;
	}
      };

    } // end of internal

  } // end of io

} // end of oln


#endif // ! OLENA_IO_PNM_WRITE_3D_HH_
