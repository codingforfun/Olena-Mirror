// Copyright 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_BORDER1D_HH
# define OLENA_CORE_BORDER1D_HH

# include "core/image1d.hh"
# include "core/border.hh"

namespace oln {

  namespace border {

    namespace internal {

      using oln::internal::image1d_data;

      template<class T>
      void _reallocate_and_copy(const image1d<T>& input,
				coord new_border, bool copy_border)
      {
	T* buffer = 0;
	// first allocate
	oln::internal::_allocate_1d_data(input.ncols(), new_border, buffer);
	// move data
	coord border = input.border();
	if (border > new_border)
	  border = new_border;
	coord src_min_col = copy_border ? -border : 0;
	coord src_ncols = input.ncols() + (copy_border ? (border * 2) : 0);
	memcpy(buffer, &input.data()->at(src_min_col),
	       src_ncols * sizeof(T));

	// then replace
	const_cast<image1d_data<T>*>(input.data())->_replace(new_border, buffer);
      }

      template<class T>
      void _replicate_borders(const image1d<T>& input)
      {
	image1d_data<T>& data = *const_cast<image1d_data<T>*>(input.data());
	for (coord j = - data.border(); j; ++j)
	  {
	    data.at(j) = data.at(0);
	    data.at(data.ncols() - j - 1) = data.at(data.ncols() - 1);
	  }
      }

      template<class T>
      void _mirror_borders(const image1d<T>& input)
      {
	image1d_data<T>& data = *const_cast<image1d_data<T>*>(input.data());
	for (coord j = - data.border(); j; ++j)
	  {
	    data.at(j) = data.at(-j);
	    data.at(data.ncols() - j - 1) = data.at(data.ncols() + j - 1);
	  }
      }
    } // internal

  } // border

} // oln


#endif // OLENA_CORE_BORDER1D_HH
