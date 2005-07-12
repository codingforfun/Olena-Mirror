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


    template <typename I, typename F, typename E>
    struct write_from_oln : public mlc::any<E>
    {
      typedef oln_type_of(I, value) value_type;

      void write_point()
      {
	this->exact().impl_write_point();
      }

      void run()
      {
	unsigned npoints = input.size().npoints();

	writer.set_size(input.size());
	if (writer.write_header() == false)
	  {
	    std::cerr << "io error: data type too large" << std::endl;
	    return;
	  }

	for (unsigned i = 0; i < npoints; ++i)
	  {
	    write_point();
	    writer.next_point();
	  }
      }

      write_from_oln(const abstract::image<I>& input, F& writer) :
	writer(writer),
	input(input)      {}

      F& writer;
      box<const I> input;
    };


    template <typename T, typename G, typename E>
    struct write_to_file : public mlc::any<E>
    {

      typedef oln_grd_type_of(G, size) size_type;
      typedef oln_grd_type_of(G, point) point_type;


      bool write_header()
      {
	return this->exact().impl_write_header();
      }

      void set_size(const size_type& size)
      {
	this->exact().impl_set_size(size);
      }

      void write_value(T v)
      {
	this->exact().impl_write_value(v);
      }

      void next_point()
      {
	this->exact().impl_next_point();
      }

      const point_type& point()
      {
	return this->exact().impl_point();
      }

    };


    template <typename I, typename F, typename E>
    struct read_to_oln : public mlc::any<E>
    {
      typedef oln_type_of(I, value) value_type;

      void read_point()
      {
	this->exact().impl_read_point();
      }

      void run()
      {
	I tmp(reader.size());
	output = tmp;
	unsigned npoints = reader.npoints();

	for (unsigned i = 0; i < npoints; ++i)
	  {
	    read_point();
	    reader.next_point();
	  }
      }

      void assign(abstract::image<I>& ima)
      {
	ima.exact() = output.unbox();
      }

      read_to_oln(F& reader) : reader(reader) {}

      F& reader;
      box<I> output;
    };

    template <typename T, typename G, typename E>
    struct read_from_file : public mlc::any<E>
    {

      typedef oln_grd_type_of(G, size) size_type;
      typedef oln_grd_type_of(G, point) point_type;

      size_type size()
      {
	return this->exact().impl_size();
      }

      T read_value()
      {
	return this->exact().impl_read_value();
      }

      void next_point()
      {
	this->exact().impl_next_point();
      }

      unsigned npoints()
      {
	return this->exact().impl_npoints();
      }

      const point_type& point()
      {
	return this->exact().impl_point();
      }

    };


  }


}


#endif // ! OLENA_CANVAS_IO_HH
