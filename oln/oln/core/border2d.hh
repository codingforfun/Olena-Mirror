// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_BORDER2D_HH
# define OLENA_CORE_BORDER2D_HH

# include <oln/core/image2d.hh>
# include <oln/core/border.hh>

namespace oln {

  namespace border {

    namespace internal {

      using oln::internal::image2d_data;

      template<class T>
      void _reallocate_and_copy(const image2d<T>& input,
				coord new_border, bool copy_border)
      {
	T* buffer = 0;
	T** array = 0;
	// first allocate
	oln::internal::_allocate_2d_data(input.nrows(), input.ncols(),
					 new_border,
					 buffer, array);
	// move data
	coord border = input.border();
	if (border > new_border)
	  border = new_border;
	coord src_min_row = copy_border ? -border : 0;
	coord src_max_row = input.nrows() + (copy_border ? border : 0);
	coord src_min_col = copy_border ? -border : 0;
	coord src_ncols = input.ncols() + (copy_border ? (border * 2) : 0);
	for (coord row = src_min_row; row < src_max_row; ++row)
	  memcpy(array[row],
		 &input.data()->at(row, src_min_col),
		 src_ncols * sizeof(T));

	// then replace
	const_cast<image2d_data<T>*>(input.data())->_replace(new_border, buffer, array);
      }


      template<class T>
      void _replicate_borders(const image2d<T>& input)
      {
	image2d_data<T>& data = *const_cast<image2d_data<T>*>(input.data());
	const coord imax = data.nrows() - 1;
	const coord jmax = data.ncols() - 1;
	// top & bottom
	for (coord i = - data.border(); i; ++i)
	  for (coord j = 0; j <= jmax; ++j)
	    {
	      data.at(i, j) = data.at(0, j);
	      data.at(imax - i, j) = data.at(imax, j);
	    }
	// left & right
	for (coord i = - data.border(); i <= imax + data.border(); ++i)
	  for (coord j = - data.border(); j; ++j)
	    {
	      data.at(i, j) = data.at(i, 0);
	      data.at(i, jmax - j) = data.at(i, jmax);
	    }
      }

      template<class T>
      void _mirror_borders(const image2d<T>& input)
      {
	image2d_data<T>& data = *const_cast<image2d_data<T>*>(input.data());
	// top & bottom
	const coord imax = data.nrows() - 1;
	const coord jmax = data.ncols() - 1;

	for (coord i = - data.border(); i; ++i)
	  for (coord j = 0; j <= jmax; ++j)
	    {
	      data.at(i, j) = data.at(- i, j);
	      data.at(imax - i, j) = data.at(imax + i, j);
	    }
	// left & right
	for (coord i = - data.border(); i <= imax + data.border(); ++i)
	  for (coord j = - data.border(); j; ++j)
	    {
	      data.at(i, j) = data.at(- i, 0);
	      data.at(i, jmax - j) = data.at(i, jmax + j);
	    }
      }


      template<class T>
      void _assign_borders(const image2d<T>& input, T val)
      {
	image2d_data<T>& data = *const_cast<image2d_data<T>*>(input.data());
	// top & bottom
        const coord imax = data.nrows() - 1;
        const coord jmax = data.ncols() - 1;

        for (coord i = - data.border(); i; ++i)
          for (coord j = 0; j <= jmax; ++j)
            {
              data.at(i, j) = val;
              data.at(imax - i, j) = val;
            }
        // left & right
        for (coord i = - data.border(); i <= imax + data.border(); ++i)
          for (coord j = - data.border(); j; ++j)
            {
              data.at(i, j) = val;
              data.at(i, jmax - j) = val;
            }
      }


    } // internal

  } // border

} // oln


#endif // OLENA_CORE_BORDER2D_HH
