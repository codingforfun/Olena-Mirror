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

#ifndef OLENA_IO_SCALAR_HH
# define OLENA_IO_SCALAR_HH

# include <oln/core/abstract/image.hh>
# include <oln/canvas/io.hh>

namespace oln {


  namespace io {


    template <typename I, typename F>
    struct read_scalar : public canvas::read_to_oln<I, F, read_scalar<I, F> >
    {
      typedef oln_type_of(I, value) value_type;
      typedef canvas::read_to_oln<I, F, read_scalar<I, F> > super_type;

      void impl_read_point()
      {
	this->output[this->reader.point()] =
	  (ntg_storage_type(value_type))this->reader.read_value();
      }

      read_scalar(F& reader) :
	super_type(reader) {}

    };

    template <typename I, typename F>
    struct write_scalar :
      public canvas::write_from_oln<I, F, write_scalar<I, F> >
    {
      typedef canvas::write_from_oln<I, F, write_scalar<I, F> > super_type;

      void impl_write_point()
      {
	this->writer.write_value(this->input[this->writer.point()]);
      }

      write_scalar(const abstract::image<I>& input, F& writer) :
	super_type(input, writer) {}

    };

  }


}

#endif // ! OLENA_IO_SCALAR_HH
