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

#ifndef OLENA_CORE_INTERNAL_IMAGE2D_DATA_HH
# define OLENA_CORE_INTERNAL_IMAGE2D_DATA_HH

# include <oln/core/internal/fake.hh>
# include <oln/core/contract.hh>
# include <oln/core/coord.hh>
# include <iostream>

namespace oln {


  namespace internal {


    template<class T>
    void _allocate_2d_data(coord nrows, coord ncols, coord border,
			   T*& buffer, T**& array)
    {
      precondition(nrows > 0 &&
		   ncols > 0 &&
		   border >= 0);
      coord nrows_eff = nrows + 2 * border;
      coord ncols_eff = ncols + 2 * border;
      array = new T*[size_t(nrows_eff)];
      buffer = new T[size_t(nrows_eff) * size_t(ncols_eff)];
      T* buf = buffer + border;
      for (coord row = 0; row < nrows_eff; ++row)
	{
	  array[row] = buf;
	  buf += ncols_eff;
	}
      array += border;
    }

    template<class T>
    void _desallocate_2d_data(coord border, T*& buffer, T**& array)
    {
      precondition(buffer != 0 && array != 0);
      delete[] buffer;
      buffer = 0; // security
      array -= border;
      delete[] array;
      array = 0;  // security
    }




    template<class T>
    struct image2d_data
    {
    public:

      image2d_data(coord nrows, coord ncols, coord border) :
	_nrows(nrows),
	_ncols(ncols),
	_border(border),
	_buffer(0),
	_array(0),
	_refcount(0)
      {
	//std::cout << "ctor " << this << std::endl;
	_allocate_2d_data(nrows, ncols, border, _buffer, _array);
      }

      image2d_data(const image2d_data&);     // cpy ctor  w/o impl
      void operator=(const image2d_data&); // assign op w/o impl

      coord nrows() const
      {
	invariant(_nrows > 0);
	return _nrows;
      }

      coord ncols() const
      {
	invariant(_ncols > 0);
	return _ncols;
      }

      coord border() const
      {
	invariant(_border >= 0);
	return _border;
      }

      size_t len() const
      {
	size_t nrows_eff = nrows() + 2 * border();
	size_t ncols_eff = ncols() + 2 * border();
	return nrows_eff * ncols_eff;
      }

      const T* buffer() const
      {
	invariant(_buffer != 0);
	return _buffer;
      }

      T* buffer()
      {
	invariant(_buffer != 0);
	return _buffer;
      }

      bool hold(coord row, coord col) const
      {
	return
	  row >= 0 &&
	  row < _nrows &&
	  col >= 0 &&
	  col < _ncols;
      }

      bool hold_large(coord row, coord col) const
      {
	return
	  row >= - _border &&
	  row < _nrows + _border &&
	  col >= - _border &&
	  col < _ncols + _border;
      }


      void precondition_hold_large(coord row, coord col) const
      {
# ifndef NDEBUG
	if (! hold_large(row, col))
	  std::cerr << "image2d overflow at ("
		    << row << "," << col << ")" << std::endl;
# else
	(void) row;
	(void) col;
# endif
      }

      // Although this is a "const" accessor, it really needs
      // to return a reference.  The I/O routines know how images
      // are stored and uses this returned address to output raw rows.
      const T& at(coord row, coord col) const
      {
	invariant(_buffer != 0);
	precondition_hold_large(row, col);
	return _array[row][col];
      }

      T& at(coord row, coord col)
      {
	invariant(_buffer != 0);
	precondition_hold_large(row, col);
	return _array[row][col];
      }

      void ref() const
      {
	++_refcount;
      }

      void unref() const
      {
	invariant(_refcount > 0);
	--_refcount;
	if (_refcount == 0)
	  delete this;
      }

      // hook:

      void _replace(coord new_border,
		    T* new_buffer, T** new_array)
      {
	_desallocate_2d_data(_border, _buffer, _array);
	_border = new_border;
	_buffer = new_buffer;
	_array  = new_array;
      }

    private:

      const coord _nrows;
      const coord _ncols;
      mutable coord _border;
      T*  _buffer;
      T** _array;
      mutable unsigned _refcount;

      ~image2d_data()
      {
	_desallocate_2d_data(_border, _buffer, _array);
      }

      friend class internal::_fake;

    };


  } // end of internal

} // end of oln


#endif // ! OLENA_CORE_INTERNAL_IMAGE2D_DATA_HH
