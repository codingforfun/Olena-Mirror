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

#ifndef METALIC_ARRAY_3D_HH
# define METALIC_ARRAY_3D_HH

# include <mlc/array/objs.hh>
# include <mlc/contract.hh>
# include <mlc/cmp.hh>

# include <iostream>

// impl
# include <mlc/array/3d.hxx>

namespace mlc 
{

  class not_implemented_yet{}; // FIXME

  template<class _Info, class T_>
  struct array3d
  {
    typedef array3d self;
    typedef T_ T;
    typedef _Info Info;

    //
    // Constructors
    //

    array3d()
    {
    }

    array3d(T* ptr)
    {
      less< 0, _Info::nplanes >::ensure();
      less< 0, _Info::nrows >::ensure();
      less< 0, _Info::ncols >::ensure();
      less< _Info::card, internal::_max_card >::ensure();
      for (unsigned i = 0; i < _Info::card; ++i)
	_buffer[i] = *ptr++;
    }

    // Copy

    array3d(const self& rhs)
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

    typedef array3d<_Info, float> to_float; // FIXME : argh

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
      postcondition(std::abs(sum - 1) <= epsilon);
      return tmp;
    }

    // Central symmetry

    array3d<array3d_info<_Info::nplanes,
			 _Info::nrows,
			 _Info::ncols,
			 _Info::card - _Info::center - 1,
			 _Info::i>, T>
    operator-() const
    {
      enum { new_center =  _Info::card - _Info::center - 1 };
      array3d<array3d_info< Info::nplanes, _Info::nrows, _Info::ncols, new_center, _Info::i>,T> tmp;

      for (unsigned i = 0; i < _Info::card; ++i)
	tmp[_Info::card - i - 1] = this->operator[](i);
      return tmp;
    }


    // Transpose


    array3d<Info, T> transpose() const // FIXME
    {
      std::cerr << "[31m===> 3D transposition not implemented yet. <===[0m" << std::endl;
      throw not_implemented_yet();
    }

    //      template<class U> int operator,(U); // FIXME: why this?

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


    T operator()(int plane, int row, int col) const		// Relative position
    {
      precondition(-_Info::center_plane <= plane);
      precondition(plane <= Info::nplanes - _Info::center_plane - 1);
      precondition(-_Info::center_row <= row);
      precondition(row <= _Info::nrows - _Info::center_row - 1);
      precondition(-_Info::center_col <= col);
      precondition(col <= _Info::ncols - _Info::center_col - 1);

      return *(_buffer + _Info::center + (plane * Info::nrows * Info::ncols) + (row * Info::ncols) + col);
    }

    T& operator()(int plane, int row, int col)
    {
      precondition(-_Info::center_plane <= plane);
      precondition(plane <= Info::nplanes - _Info::center_plane - 1);
      precondition(-_Info::center_row <= row);
      precondition(row <= _Info::nrows - _Info::center_row - 1);
      precondition(-_Info::center_col <= col);
      precondition(col <= _Info::ncols - _Info::center_col - 1);

      return *(_buffer + _Info::center + (plane * Info::nrows * Info::ncols) + (row * Info::ncols) + col);
    }


    // do not use these methods...

    template<unsigned i>
    T _get_at() const {
      lesseq<i, _Info::card>::ensure();
      return *(_buffer + i);
    }

    template<int nplane, int nrow, int ncol>
    T _get() const {
      lesseq< -_Info::center_plane, nplane >::ensure();
      lesseq< nplane, Info::nplanes - _Info::center_plane - 1 >::ensure();
      lesseq< -_Info::center_row, nrow >::ensure();
      lesseq< nrow, _Info::nrows - _Info::center_row - 1 >::ensure();
      lesseq< -_Info::center_col, ncol >::ensure();
      lesseq< ncol, _Info::ncols - _Info::center_col - 1 >::ensure();

      //FIXME:      return *(_buffer + _Info::center + (nplane * Info::nrows * Info::ncols) + (row * Info::ncols) + col);
      return *(_buffer + _Info::center + (nplane * Info::nrows * Info::ncols) + (nrow * Info::ncols) + ncol);
    }


  protected:

    T _buffer[internal::_max_card];
  };


  // ...but these static accessors:

  template<unsigned i, class Info, class T> inline
  T get_at(const array3d<Info, T>& arr)
  {
    return arr.template _get_at<i>();
  }

  template<int plane, int row, int col, class Info, class T> inline
  T get(const array3d<Info, T>& arr)
  {
    return arr.template _get<plane, row, col>();
  }

  // starter objects

  static internal::_array3d_start<int>   ints_3d   = internal::_array3d_start<int>();
  static internal::_array3d_start<float> floats_3d = internal::_array3d_start<float>();

  template<class Info, class T>
  std::ostream& operator<<(std::ostream& ostr, const array3d<Info, T>& rhs)
  {
    for (int i = 0; i < Info::card; ++i)
      {
	if (i == Info::center)
	  ostr << "<" << rhs[i] << "> ";
	else
	  ostr << rhs[i] << " ";

	ostr << ((i + 1) % Info::ncols == 0 ? "\n" : "\t");
	ostr << ((i + 1) % (Info::ncols * Info::nrows) == 0 ? "\n" : "");
      }
    ostr << std::flush;

    return ostr;
  }

} // end of mlc

#endif // ! METALIC_ARRAY_3D_HH
