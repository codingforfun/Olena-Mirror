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

#ifndef OLENA_CORE_IMAGE2D_BIN_HXX
#define OLENA_CORE_IMAGE2D_BIN_HXX

#include <oln/core/internal/image2d_data.hh>
#include <oln/value/bin.hh>
#include <oln/core/contract.hh>
#include <oln/core/point2d.hh>


namespace oln {


  template<class I> inline
  image2d<bin,I>::image2d()
  {
    _data = 0;
  }

  template<class I> inline
  image2d<bin,I>::image2d(coord nrows, coord ncols, coord border)
  {
    _data = new internal::image2d_data<bin>(nrows, ncols, border);
    _data->ref();
  }

  template<class I> inline
  image2d<bin,I>::image2d(const image2d_size& size, coord border)
  {
    _data = new internal::image2d_data<bin>(size.nrows(),
					size.ncols(),
					border);
    _data->ref();
  }

  template<class I> inline image2d<bin,I>
  image2d<bin,I>::clone() const
  {
    assertion(_data != 0);
    image2d<bin,I> output(nrows(), ncols(), border());
    assertion(output.data()->len() == _data->len());
    memcpy(output.data()->buffer(),
	   _data->buffer(),
	   _data->len() * sizeof(bin));
    return output;
  }

  template<class I> inline
  image2d<bin,I>::image2d(image2d<bin,I>& rhs)
  {
    assertion(rhs.data() != 0);
    _data = rhs.data();
    _data->ref();
  }

  template<class I> inline image2d<bin,I>&
  image2d<bin,I>::operator=(image2d<bin,I> rhs)
  {
    if ( &rhs == this )
      return *this;
    if (_data != 0)
      _data->unref();
    _data = rhs.data();
    if (_data != 0)
      _data->ref();
    return *this;
  }

  template<class I> inline
  image2d<bin,I>::~image2d()
  {
    clear();
  }

  template<class I> inline void
  image2d<bin,I>::clear()
  {
    if (_data != 0)
      {
	_data->unref();
	_data = 0; // security
      }
  }

  template<class I> inline coord
  image2d<bin,I>::nrows() const
  {
    assertion(_data != 0);
    return _data->nrows();
  }

  template<class I> inline coord
  image2d<bin,I>::ncols() const
  {
    assertion(_data != 0);
    return _data->ncols();
  }

  template<class I> inline coord
  image2d<bin,I>::border() const
  {
    assertion(_data != 0);
    return _data->border();
  }

  template<class I> inline image2d_size
  image2d<bin,I>::size() const
  {
    assertion(_data != 0);
    return image2d_size(_data->nrows(), _data->ncols());
  }

  template<class I> inline bin&
  image2d<bin,I>::operator()(coord row, coord col)
  {
    assertion(_data != 0);
    return _data->at(row, col);
  }

  template<class I> inline bin
  image2d<bin,I>::operator()(coord row, coord col) const
  {
    assertion(_data != 0);
    return _data->at(row, col);
  }

  template<class I> inline bool
  image2d<bin,I>::hold(const point2d& p) const
  {
    return hold(p.row(), p.col());
  }

  template<class I> inline bool
  image2d<bin,I>::hold(coord row, coord col) const
  {
    assertion(_data != 0);
    return _data->hold(row, col);
  }

  template<class I> inline bin
  image2d<bin,I>::operator[](const point2d& p) const
  {
    assertion(_data != 0);
    return _data->at(p.row(), p.col());
  }

  template<class I> inline bin&
  image2d<bin,I>::operator[](const point2d& p)
  {
    assertion(_data != 0);
    return _data->at(p.row(), p.col());
  }

  template<class I> inline const internal::image2d_data<bin>*
  image2d<bin,I>::data() const
  {
    return _data;
  }

  template<class I> inline internal::image2d_data<bin>*
  image2d<bin,I>::data()
  {
    return _data;
  }


} // end of oln



#endif // ! OLENA_CORE_IMAGE2D_BIN_HXX
