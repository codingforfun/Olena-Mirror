// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_INTERNAL_IMAGE1D_DATA_HH
# define OLENA_CORE_INTERNAL_IMAGE1D_DATA_HH

# include <oln/core/internal/fake.hh>
# include <oln/core/contract.hh>
# include <oln/core/coord.hh>
# include <iostream>

namespace oln {


  namespace internal {


    template<class T>
    void _allocate_1d_data(coord ncols, coord border, T*& buffer)
    {
      precondition(ncols > 0 && border >= 0);
      coord ncols_eff = ncols + 2 * border;
      buffer = new T[size_t(ncols_eff)] + border;
    }

    template<class T>
    void _desallocate_1d_data(coord border, T*& buffer)
    {
      precondition(buffer != 0);
      delete[] (buffer - border);
      buffer = 0; // security
    }

    template<class T>
    struct image1d_data
    {
    public:

      image1d_data(coord ncols, coord border) :
	_ncols(ncols),
	_border(border),
	_buffer(0),
	_refcount(0)
      {
	//std::cout << "ctor " << this << std::endl;
	_allocate_1d_data(ncols, border, _buffer);
      }

      image1d_data(const image1d_data&);     // cpy ctor  w/o impl
      void operator=(const image1d_data&); // assign op w/o impl

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
	size_t ncols_eff = ncols() + 2 * border();
	return ncols_eff;
      }

      const T* buffer() const
      {
	invariant(_buffer != 0);
	return _buffer - border();
      }

      T* buffer()
      {
	invariant(_buffer != 0);
	return _buffer - border();
      }

      bool hold(coord col) const
      {
	return
	  col >= 0 &&
	  col < _ncols;
      }

      bool hold_large(coord col) const
      {
	return
	  col >= - _border &&
	  col < _ncols + _border;
      }

      void precondition_hold_large(coord col) const
      {
#ifndef NDEBUG
	if (! hold_large(col))
	  std::cerr << "image1d overflow at (" << col << ")" << std::endl;
#else
	(void) col;
#endif
      }

      // Although this is a "const" accessor, it really needs
      // to return a reference.  The I/O routines know how images
      // are stored and uses this returned address to output raw rows.
      const T& at(coord col) const
      {
	invariant(_buffer != 0);
	precondition_hold_large(col);
	return _buffer[col];
      }

      T& at(coord col)
      {
	invariant(_buffer != 0);
	precondition_hold_large(col);
	return _buffer[col];
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

      void _replace(coord new_border, T* new_buffer)
      {
	_desallocate_1d_data(_border, _buffer);
	_border = new_border;
	_buffer = new_buffer;
      }

    private:

      const coord _ncols;
      mutable coord _border;
      T*  _buffer;
      mutable unsigned _refcount;

      ~image1d_data()
      {
	_desallocate_1d_data(_border, _buffer);
      }

      friend class internal::_fake;

    };

  } // end of internal

} // end of oln


#endif // ! OLENA_CORE_INTERNAL_IMAGE1D_DATA_HH
