// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_INTERNAL_REAL_IMAGE1D_HH
# define OLENA_CORE_INTERNAL_REAL_IMAGE1D_HH

# include <oln/core/internal/image1d.hh>
# include <oln/core/internal/image1d_data.hh>
# include <iostream>

namespace oln {

  namespace internal
  {

    // _real_image1d is a base class for every 1d image type that really
    // holds data, that is, every 1d image type which is not a proxy.


    template<class T, class Exact>
    class _real_image1d : public _image1d< T, Exact >
    {
    public:

      typedef _real_image1d<T, Exact> self;
      typedef _image1d< T, Exact > super;

      // ctors are protected; see below

      _real_image1d(self& rhs) : super(rhs) // shallow copy
      {
	assertion(rhs.data() != 0);
	_data = rhs.data();
	_data->ref();
      }

      self& operator=(self rhs) // shallow assignment
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

      ~_real_image1d()
      {
	clear();
      }

      void clear()
      {
	if (_data != 0)
	  {
	    _data->unref();
	    _data = 0; // security
	  }
      }

      const internal::image1d_data<T>* data() const
      {
	return _data;
      }

      internal::image1d_data<T>* data()
      {
	return _data;
      }

      static std::string name()
      {
	return
	  std::string("_real_image1d<")
	  + T::name() + ","
	  + Exact::name() + ">";
      }

    protected:

      _real_image1d()
      {
	_data = 0;
      }

      _real_image1d(coord ncols, coord border)
      {
	_data = new internal::image1d_data<T>(ncols, border);
	_data->ref();
      }

      _real_image1d(const image1d_size& size, coord border)
      {
	_data = new internal::image1d_data<T>(size.ncols(),
					      border);
	_data->ref();
      }

      void _clone_to(internal::image1d_data<T>* output_data) const // deep copy
      {
	assertion(_data != 0);
	precondition(output_data != 0);
	precondition(output_data->len() == _data->len());
	memcpy(output_data->buffer(),
	       _data->buffer(),
	       _data->len() * sizeof(T));
      }

    private:

      internal::image1d_data<T>* _data;

      _real_image1d(const self& rhs); // w/o impl
    };


  } // end of internal

} // end of oln

template<class T, class I> inline std::ostream&
operator<<(std::ostream& o, const oln::internal::_real_image1d<T,I>& ima)
{
  if (ima.data() == 0)
    return o << "null";
  for (oln::coord col = 0; col < ima.ncols(); ++col)
    o << ima(col) << ' ';
  o << std::endl;
  return o;
}

#endif // ! OLENA_CORE_INTERNAL_REAL_IMAGE1D_HH
