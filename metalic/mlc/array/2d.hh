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

#ifndef OLENA_META_ARRAY2D_HH
# define OLENA_META_ARRAY2D_HH

# include <mlc/array/objs.hh>
# include <mlc/contract.hh>
# include <mlc/cmp.hh>
# include <mlc/array/2d.hxx>

# include <iostream>

namespace oln {

  namespace meta {


    template<class _Info, class T_>
    struct array2d
    {
      typedef array2d self;
      typedef T_ T;
      typedef _Info Info;



      //
      // Constructors
      //

      array2d()
      {
      }

      array2d(T* ptr)
      {
	meta::less< 0, _Info::nrows >::ensure();
	meta::less< 0, _Info::ncols >::ensure();
	meta::less< _Info::card, internal::_max_card >::ensure();
	for (unsigned i = 0; i < _Info::card; ++i)
	  _buffer[i] = *ptr++;
      }

      // Copy

      array2d(const self& rhs)
      {
	for (unsigned i = 0; i < _Info::card; ++i)
	  _buffer[i] = rhs[i];
      }
      self& operator=(const self& rhs)
      {
	for (unsigned i = 0; i < _Info::card; ++i)
	  _buffer[i] = rhs[i];
	return *this;
      }



      //
      // Operations on array
      //

      typedef array2d<_Info, float> to_float; // FIXME : argh

      // Normalize (absolute values -> relative values)

      to_float normalize()
      {
	to_float tmp;
	float sum = 0.f; // FIXME: float only?
	const float epsilon = 0.01f; // FIXME : epsilon should be global
	unsigned i;
	for (i = 0; i < _Info::card; ++i)
	  sum += this->_buffer[i];
	for (i = 0; i < _Info::card; ++i)
	  tmp[i] = this->_buffer[i] / sum;
	// security
	sum = 0.f;
	for (i = 0; i < _Info::card; ++i)
	  sum += tmp[i];
	postcondition(abs(sum - 1) <= epsilon);
	return tmp;
      }

      // Central symmetry

      array2d<array2d_info<_Info::nrows,
			   _Info::ncols,
			   _Info::card - _Info::center - 1,
			   _Info::i>, T>
      operator-() const
      {
	enum { new_center =  _Info::card - _Info::center - 1 };
	array2d<array2d_info< _Info::nrows, _Info::ncols, new_center, _Info::i>,T> tmp;

	for (unsigned i = 0; i < _Info::card; ++i)
	  tmp[_Info::card - i - 1] = this->operator[](i);
	return tmp;
      }


      // Transpose

      typedef array2d<array2d_info<
	_Info::ncols,
	_Info::nrows,
	(_Info::center * _Info::nrows + _Info::center / _Info::ncols) % _Info::card,
	_Info::i
	>, T> transposed_array_t;

      transposed_array_t transpose() const
      {
	transposed_array_t tmp;
	for (int i = 0; i < Info::card; ++i)
	  tmp[i] = this->operator[]((i * _Info::ncols + i / _Info::nrows) % _Info::card);
	return tmp;
      }



      //
      // Accessors
      //

      unsigned size() const
      {
	return _Info::card;
      }

      const T* buffer() const
      {
	return _buffer;
      }

      // dynamic accessors:

      T operator[](unsigned i) const	// Absolute position
      {
	precondition(i < _Info::card);
	return *(_buffer + i);
      }
      T& operator[](unsigned i)
      {
	precondition(i < _Info::card);
	return *(_buffer + i);
      }


      T operator()(int row, int col) const		// Relative position
      {
	precondition(-_Info::center_row <= row);
	precondition(row <= _Info::nrows - _Info::center_row - 1);
	precondition(-_Info::center_col <= col);
	precondition(col <= _Info::ncols - _Info::center_col - 1);

	return *(_buffer + _Info::center + (row * _Info::ncols) + col);
      }
      T& operator()(int row, int col)
      {
	precondition(-_Info::center_row <= row);
	precondition(row <= _Info::nrows - _Info::center_row - 1);
	precondition(-_Info::center_col <= col);
	precondition(col <= _Info::ncols - _Info::center_col - 1);

	return *(_buffer + _Info::center + (row * _Info::ncols) + col);
      }


      // do not use these methods...

      template<unsigned i>
      T _get_at() const {
	meta::lesseq<i, _Info::card>::ensure();
	return *(_buffer + i);
      }

      template<int nrow, int ncol>
      T _get() const {
	meta::lesseq< -_Info::center_row, nrow >::ensure();
	meta::lesseq< nrow, _Info::nrows - _Info::center_row - 1 >::ensure();
	meta::lesseq< -_Info::center_col, ncol >::ensure();
	meta::lesseq< ncol, _Info::ncols - _Info::center_col - 1 >::ensure();

	return *(_buffer + _Info::center + (row * _Info::ncols) + col);
      }


    protected:

      T _buffer[internal::_max_card];
    };


    // ...but these static accessors:

    template<unsigned i, class Info, class T> inline
    T get_at(const meta::array2d<Info, T>& arr)
    {
      return arr.template _get_at<i>();
    }

    template<int row, int col, class Info, class T> inline
    T get(const meta::array2d<Info, T>& arr)
    {
      return arr.template _get<row, col>();
    }

    // starter objects

    static internal::_array2d_start<int>   ints_2d   = internal::_array2d_start<int>();
    static internal::_array2d_start<float> floats_2d = internal::_array2d_start<float>();


  } // end of meta

} // end of oln

template<class Info, class T>
std::ostream& operator<<(std::ostream&				ostr,
			 const oln::meta::array2d<Info, T>&	rhs)
{
  for (int i = 0; i < Info::card; ++i)
  {
    if (i == Info::center)
      ostr << "<" << rhs[i] << ">";
    else
      ostr << rhs[i];

    ostr << ((i + 1) %  Info::ncols == 0 ? "\n" : "\t");
  }
  ostr << std::flush;
  return ostr;
}


#endif // ! OLENA_META_ARRAY2D_HH
