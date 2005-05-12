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
// As a special exception, you may use this filek as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_CANVAS_IO_HH
# define OLENA_CANVAS_IO_HH

# include <iostream>

# include <mlc/any.hh>

# include <oln/core/abstract/image.hh>
# include <oln/io/utils.hh>

namespace oln {


  namespace canvas {


    template <typename I, typename E>
    struct write : public mlc::any<E>
    {

      void impl_extra_work()
      {
      }

      void extra_work()
      {
	this->exact().impl_extra_work();
      }

      void write_point()
      {
	this->exact().impl_write_point();
      }

      void run()
      {
	for (p.row() = 0; p.row() < out.size().nrows(); ++p.row())
	  {
	    for (p.col() = 0; p.col() < out.size().ncols(); ++p.col())
	      write_point();
	    extra_work();
	  }
      }


    protected:

      write(const oln::abstract::image<I>& input,
	    std::ostream& ostr) :
	input(input),
	ostr(ostr)
      {}

      const I& input;
      std::ostream& ostr;
      oln_type_of(I, point) p;


    };


    template <typename I, typename E>
    struct read : public mlc::any<E>
    {

      void impl_extra_work() {}

      void extra_work()
      {
	this->exact().impl_extra_work();
      }

      void read_point()
      {
	this->exact().impl_read_point();
      }

      void preconditions()
      {
	this->exact().impl_preconditions();
      }

      void run()
      {
	preconditions();

	for (p.row() = 0; p.row() < hdr.rows && !istr.eof(); ++p.row())
	  {
	    extra_work();
	    for (p.col() = 0; p.col() < hdr.cols && !istr_.eof(); ++p.col())
	      read_point();
	  }
      }

    protected:

      read(std::istream& istr,
	   const io::internal::pnm_header& hdr) :
	istr(istr),
	hdr(hdr)
      {}

      box<I> output;
      std::istream& istr;
      const io::internal::pnm_header& hdr;
      oln_type_of(I, point) p;

    };


  }


}


#endif // ! OLENA_CANVAS_IO_HH
