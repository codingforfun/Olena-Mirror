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

#ifndef OLENA_CORE_IMAGE2D_HXX
#define OLENA_CORE_IMAGE2D_HXX

#include "core/internal/image2d_data.hh"
#include "value/bin.hh"
#include "core/contract.hh"
#include "core/point2d.hh"


namespace oln {


  template<class T, class I> inline
  image2d<T,I>::image2d()
  {
    _data = 0;
  }

  template<class T, class I> inline
  image2d<T,I>::image2d(coord nrows, coord ncols, coord border)
  {
    _data = new internal::image2d_data<T>(nrows, ncols, border);
    _data->ref();
  }

  template<class T, class I> inline
  image2d<T,I>::image2d(const image2d_size& size, coord border)
  {
    _data = new internal::image2d_data<T>(size.nrows(),
					size.ncols(),
					border);
    _data->ref();
  }

  template<class T, class I> inline image2d<T,I>
  image2d<T,I>::clone() const
  {
    assertion(_data != 0);
    image2d<T,I> output(nrows(), ncols(), border());
    assertion(output.data()->len() == _data->len());
    memcpy(output.data()->buffer(),
	   _data->buffer(),
	   _data->len() * sizeof(T));
    return output;
  }

  template<class T, class I> inline
  image2d<T,I>::image2d(image2d<T,I>& rhs)
  {
    assertion(rhs.data() != 0);
    _data = rhs.data();
    _data->ref();
  }

  template<class T, class I> inline image2d<T,I>&
  image2d<T,I>::operator=(image2d<T,I> rhs)
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

  template<class T, class I> inline
  image2d<T,I>::~image2d()
  {
    clear();
  }

  template<class T, class I> inline void
  image2d<T,I>::clear()
  {
    if (_data != 0)
      {
	_data->unref();
	_data = 0; // security
      }
  }

  template<class T, class I> inline coord
  image2d<T,I>::nrows() const
  {
    assertion(_data != 0);
    return _data->nrows();
  }

  template<class T, class I> inline coord
  image2d<T,I>::ncols() const
  {
    assertion(_data != 0);
    return _data->ncols();
  }

  template<class T, class I> inline coord
  image2d<T,I>::border() const
  {
    assertion(_data != 0);
    return _data->border();
  }

  template<class T, class I> inline image2d_size
  image2d<T,I>::size() const
  {
    assertion(_data != 0);
    return image2d_size(_data->nrows(), _data->ncols());
  }

  template<class T, class I> inline T&
  image2d<T,I>::operator()(coord row, coord col)
  {
    assertion(_data != 0);
    return _data->at(row, col);
  }

  template<class T, class I> inline T
  image2d<T,I>::operator()(coord row, coord col) const
  {
    assertion(_data != 0);
    return _data->at(row, col);
  }

  template<class T, class I> inline bool
  image2d<T,I>::hold(const point2d& p) const
  {
    return hold(p.row(), p.col());
  }

  template<class T, class I> inline bool
  image2d<T,I>::hold(coord row, coord col) const
  {
    assertion(_data != 0);
    return _data->hold(row, col);
  }

  template<class T, class I> inline T
  image2d<T,I>::operator[](const point2d& p) const
  {
    assertion(_data != 0);
    return _data->at(p.row(), p.col());
  }

  template<class T, class I> inline T&
  image2d<T,I>::operator[](const point2d& p)
  {
    assertion(_data != 0);
    return _data->at(p.row(), p.col());
  }

  template<class T, class I> inline const internal::image2d_data<T>*
  image2d<T,I>::data() const
  {
    return _data;
  }

  template<class T, class I> inline internal::image2d_data<T>*
  image2d<T,I>::data()
  {
    return _data;
  }


  ////////////////////////////////////    image2d_size


  inline
  image2d_size::image2d_size(coord nrows, coord ncols) :
    _nrows(nrows),
    _ncols(ncols)
  {
  }

  inline coord
  image2d_size::nrows() const
  {
    invariant(_nrows > 0);
    return _nrows;
  }

  inline coord
  image2d_size::ncols() const
  {
    invariant(_ncols > 0);
    return _ncols;
  }

  inline bool
  image2d_size::operator==(const image2d_size& size) const
  {
    return
      this->_nrows == size.nrows() &&
      this->_ncols == size.ncols();
  }

  inline bool
  image2d_size::operator!=(const image2d_size& size) const
  {
    return
      this->_nrows != size.nrows() ||
      this->_ncols != size.ncols();
  }


} // end of oln



template<class T, class I> inline std::ostream&
operator<<(std::ostream& o, const oln::image2d<T,I>& ima)
{
  if (ima.data() == 0)
    return o << "null";
  for (oln::coord row = 0; row < ima.nrows(); ++row)
    {
      for (oln::coord col = 0; col < ima.ncols(); ++col)
	o << ima(row,col) << ' ';
      o << std::endl;
    }
  return o;
}



#endif // ! OLENA_CORE_IMAGE2D_HXX
