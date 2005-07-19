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

#ifndef OLN_IO_PNM_BIN_HH
# define OLN_IO_PNM_BIN_HH

# include <iostream>

# include <oln/canvas/io.hh>
# include <oln/io/pnm_header.hh>
# include <oln/core/2d/grid2d.hh>
# include <oln/core/2d/point2d.hh>
# include <oln/core/2d/size2d.hh>


namespace oln {

  namespace io {

    namespace pnm {


      template <typename G>
      struct read_bin {};

      template <typename G>
      struct write_bin {};

      template <>
      struct read_bin<oln::grid2d>
	: public canvas::read_from_file<bool, grid2d, read_bin<grid2d> >
      {
	typedef oln_grd_type_of_(grid2d, size) size_type;
	typedef oln_grd_type_of_(grid2d, point) point_type;

	unsigned impl_npoints()
	{
	  return hdr.cols * hdr.rows;
	}

	size_type impl_size()
	{
	  return size_type(hdr.rows, hdr.cols);
	}

	void extra_work()
	{
	  offset = -1;
	}

	bool impl_read_value()
	{
	  if (offset == -1)
	    {
	      this->istr.read(&v, 1);
	      offset = 7;
	    }
	  if ((int)(v & (1<<offset--)) == 0)
	    return false;
	  else
	    return true;
	}

	void impl_next_point()
	{
	  precondition(p.row() < hdr.rows && p.col() < hdr.cols);
	  ++p.col();
	  if (p.col() >= hdr.cols)
	    {
	      p.col() = 0;
	      ++p.row();
	      extra_work();
	    }
	}

	const point_type& impl_point()
	{
	  return p;
	}

	read_bin(std::istream& istr, const header& hdr) :
	  istr(istr),
	  hdr(hdr),
	  v(0),
	  offset(-1)
	{
	  p.col() = 0;
	  p.row() = 0;

	}

	std::istream& istr;
	const header& hdr;
	point_type p;

	char v;
	int offset;

      };


      template <>
      struct write_bin<oln::grid2d>
	: public canvas::write_to_file<bool, grid2d, write_bin<grid2d> >
      {
	typedef oln_grd_type_of_(grid2d, size) size_type;
	typedef oln_grd_type_of_(grid2d, point) point_type;

	void set_size(const size_type& size)
	{
	  nrows = size.nrows();
	  ncols = size.ncols();
	}

	void extra_work()
	{
	  if (offset != 7)
	    {
	      this->ostr.write(&v, 1);
	      offset = 7;
	      v = 0;
	    }
	}

	void impl_write_value(bool& b)
	{
	  if (offset == -1)
	    {
	      this->ostr.write(&v, 1);
	      offset = 7;
	      v = 0;
	    }
	  if (b == true)
	    v |= 1 << offset;
	  offset--;
	}

	void impl_next_point()
	{
	  precondition(p.row() < nrows && p.col() < ncols);
	  ++p.col();
	  if (p.col() >= ncols)
	    {
	      p.col() = 0;
	      ++p.row();
	      extra_work();
	    }
	}

	bool impl_write_header()
	{
	  return pnm::write_header(ostr, "P4", ncols, nrows, 0);
	}

	const point_type& impl_point()
	{
	  return p;
	}

	write_bin(std::ostream& ostr) :
	  ostr(ostr),
	  offset(7),
	  v(0)
	{
	  p.col() = 0;
	  p.row() = 0;
	}

	std::ostream& ostr;
	point_type p;
	int nrows, ncols;

	int offset;
	char v;

      };


    }

  }

}

#endif // ! OLN_IO_PNM_BIN_HH
