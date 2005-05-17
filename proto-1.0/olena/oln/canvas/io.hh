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
# include <oln/core/box.hh>

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

      void work()
      {
	this->exact().impl_work();
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
	for (p.row() = 0; p.row() < input.size().nrows(); ++p.row())
	  {
	    work();
	    extra_work();
	  }
      }


    protected:

      write(const oln::abstract::image<I>& input,
	    std::ostream& ostr) :
	input(input.exact()),
	ostr(ostr)
      {}

      box<const I> input;
      std::ostream& ostr;
      oln_type_of(I, point) p;

    };



    template <typename I, typename E>
    struct write_2d: public write<I, E>
    {

      void impl_work()
      {
	for (this->p.col() = 0;
	     this->p.col() < this->input.size().ncols();
	     ++this->p.col())
	  this->write_point();
      }

    protected:
      typedef write<I, E> super_type;

      write_2d(const abstract::image<I>& input,
	       std::ostream& ostr) :
	super_type(input, ostr)
      {}

    };

    template <typename I, typename E>
    struct write_1d: public write<I, E>
    {

      void impl_work()
      {
	this->write_point();
      }

    protected:
      typedef write<I, E> super_type;

      write_1d(const abstract::image<I>& input,
	       std::ostream& ostr) :
	super_type(input, ostr)
      {}

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

      void work()
      {
	this->exact().impl_work();
      }

      void get_output(I& ima)
      {
	ima = output.unbox();
      }

      void run()
      {
	preconditions();

	I tmp(hdr.rows, hdr.cols);

	output = tmp;

	for (p.row() = 0; p.row() < hdr.rows && !istr.eof(); ++p.row())
	  {
	    extra_work();
	    work();
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

    template <typename I, typename E>
    struct read_2d: public read<I, E>
    {

      void impl_work()
      {
	for (this->p.col() = 0;
	     this->p.col() < this->hdr.cols && !this->istr.eof();
	     ++this->p.col())
	  this->read_point();
      }

    protected:
      typedef read<I, E> super_type;

      read_2d(std::istream& istr,
	      const io::internal::pnm_header& hdr) :
	super_type(istr, hdr)
      {}

    };


    template <typename I, typename E>
    struct read_1d: public read<I, E>
    {

      void impl_work()
      {
	this->read_point();
      }

    protected:
      typedef read<I, E> super_type;

      read_1d(std::istream& istr,
	      const io::internal::pnm_header& hdr) :
	super_type(istr, hdr)
      {}

    };


  }


}


#endif // ! OLENA_CANVAS_IO_HH
