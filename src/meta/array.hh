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

#ifndef OLENA_META_ARRAY_HH
# define OLENA_META_ARRAY_HH

# include <iostream>

# include "core/objs.hh"
# include "meta/cmp.hh"
# include "meta/array.hxx"



namespace oln {

  namespace meta {



    template<int _b_row, int _e_row, int _b_col, int _e_col,
	     class _T>
    struct array2d
    {
      typedef array2d self;
      typedef _T T;
      enum {
	// id
	b_row = _b_row,
	e_row = _e_row,
	b_col = _b_col,
	e_col = _e_col,
	// deduced
	nrows = e_row - b_row,
	ncols = e_col - b_col,
	card  = nrows * ncols,
	delta = max< max<-b_row, e_row-1>::ret,
	             max<-b_col, e_col-1>::ret >::ret
      };

      array2d()
      {
      }

      array2d(T* ptr)
      {
	meta::less<0,nrows>::ensure();
	meta::less<0,ncols>::ensure();
	meta::less<card, internal::_max_card>::ensure();
	for (unsigned i = 0; i < card; ++i)
	  _buffer[i] = *ptr++;
      }

      array2d(const self& rhs)
      {
	for (unsigned i = 0; i < card; ++i)
	  _buffer[i] = rhs[i];
      }

      self& operator=(const self& rhs)
      {
	for (unsigned i = 0; i < card; ++i)
	  _buffer[i] = rhs[i];
	return *this;
      }

      unsigned size() const
      {
	return card;
      }

      typedef array2d<_b_row,_e_row,_b_col,_e_col,float> to_float;

      to_float normalize()
      {
	to_float tmp;
	float sum = 0.f; // FIXME: float only?
	const float epsilon = 0.01f; // FIXME : epsilon should be global
	unsigned i;
	for (i = 0; i < card; ++i)
	  sum += this->_buffer[i];
	for (i = 0; i < card; ++i)
	  tmp[i] = this->_buffer[i] / sum;
	// security
	sum = 0.f;
	for (i = 0; i < card; ++i)
	  sum += tmp[i];
	postcondition(abs(sum - 1) <= epsilon);
	return tmp;
      }

      array2d<1-e_row,1-b_row,1-e_col,1-b_col,T>
      operator-() const
      {
	array2d<1-e_row,1-b_row,1-e_col,1-b_col,T> tmp;
	for (int row = b_row; row < e_row; ++row)
	  for (int col = b_col; col < e_col; ++col)
	    tmp(-row, -col) = this->operator()(row, col);
	return tmp;
      }

      template<class U> int operator,(U); // FIXME: why this?

      const T* buffer() const { return _buffer; } // hook

      // dynamic accessors:

      T operator[](unsigned i) const
      {
	precondition(i < card);
	return *(_buffer + i);
      }

      T& operator[](unsigned i)
      {
	precondition(i < card);
	return *(_buffer + i);
      }

      T operator()(int row, int col) const
      {
	precondition(b_row <= row && row < e_row);
	precondition(b_col <= col && col < e_col);
	return *(_buffer + ((row-b_row) * ncols + (col-b_col)));
      }

      T& operator()(int row, int col)
      {
	precondition(b_row <= row && row < e_row);
	precondition(b_col <= col && col < e_col);
	return *(_buffer + ((row-b_row) * ncols + (col-b_col)));
      }

      // do not use these methods...

      template<unsigned i>
      T _get_at() const {
	meta::less<i,card>::ensure();
	return *(_buffer + i);
      }

      template<int row, int col>
      T _get() const {
	meta::lesseq<0, row - b_row>::ensure(); // FIXME: why not lesseq<b_row,row> etc.?
	meta::lesseq<0, col - b_col>::ensure();
	meta::less  <0, e_row - row>::ensure();
	meta::less  <0, e_col - col>::ensure();
	return *(_buffer + (row-b_row) * ncols + col-b_col);
      }

    protected:

      T _buffer[card];
    };



    // ...but these static accessors:

    template<unsigned i,
	     int b_row, int e_row, int b_col, int e_col,
	     class T>
    inline T
    get_at(const array2d<b_row,e_row,b_col,e_col,T>& arr)
    {
      return arr.template _get_at<i>();
    }

    template<int row, int col,
	     int b_row, int e_row, int b_col, int e_col,
	     class T>
    inline T
    get(const array2d<b_row,e_row,b_col,e_col,T>& arr)
    {
      return arr.template _get<row,col>();
    }


    # define array array2d


    // procs

    template<int b_row, int e_row, int b_col, int e_col, class T>
    array2d<b_col,e_col,b_row,e_row,T>
    transpose(const array2d<b_row,e_row,b_col,e_col,T>& arr)
    {
      array2d<b_col,e_col,b_row,e_row,T> tmp;
      for (int row = b_col; row < e_col; ++row)
	for (int col = b_row; col < e_row; ++col)
	  tmp(row, col) = arr(col, row);
      return tmp;
    }


    // starter objects

    static internal::_array2d_start<int>   ints   = internal::_array2d_start<int>();
    static internal::_array2d_start<float> floats = internal::_array2d_start<float>();


  } // end of meta


} // end of oln



template<int b_row, int e_row, int b_col, int e_col,
	 class T>
std::ostream& operator<<(std::ostream& ostr,
			 const oln::meta::array2d<b_row,e_row,b_col,e_col,T>& rhs)
{
  for (int row = b_row; row < e_row; ++row)
    {
      for (int col = b_col; col < e_col; ++col)
	if (col == 0 && row == 0)
	  ostr << "<" << rhs(row,col) << "> ";
	else
	  ostr << rhs(row,col) << " ";
      ostr << endl;
    }
  return ostr;
}


#endif // ! OLENA_META_ARRAY_HH
