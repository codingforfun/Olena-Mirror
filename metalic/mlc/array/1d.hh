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

#ifndef OLENA_META_ARRAY1D_HH
# define OLENA_META_ARRAY1D_HH

# include <mlc/array/objs.hh>
# include <mlc/contract.hh>
# include <mlc/cmp.hh>
# include <mlc/array/1d.hxx>

# include <iostream>

namespace oln {

  namespace meta {


    template<class _Info, class T_>
    struct array1d
    {
      typedef array1d self;
      typedef T_ T;
      typedef _Info Info;


      //
      // Constructors
      //

      array1d()
      {
      }

      array1d(T* ptr)
      {
	meta::less< 0, _Info::card >::ensure();
	meta::less< _Info::card, internal::_max_card >::ensure();
	for (unsigned i = 0; i < _Info::card; ++i)
	  _buffer[i] = *ptr++;
      }

      // Copy

      array1d(const self& rhs)
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

      typedef array1d<_Info,float> to_float; // FIXME : argh

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

      array1d<array1d_info<_Info::card,
			   _Info::card - _Info::center - 1,
			   _Info::i>, T>
      operator-() const
      {
	enum { new_center =  _Info::card - _Info::center - 1 };
	array1d<array1d_info< _Info::card, new_center, _Info::i>,T> tmp;

	for (unsigned i = 0; i < _Info::card; ++i)
	  tmp[_Info::card - i - 1] = this->operator[](i);
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

      T operator()(int i) const		// Relative position
      {
	precondition(-_Info::center <= i);
	precondition(i <= _Info::card - _Info::center - 1);
	return *(_buffer + _Info::center + i);
      }
      T& operator()(int i)
      {
	precondition(-_Info::center <= i);
	precondition(i <= _Info::card - _Info::center - 1);
	return *(_buffer + _Info::center + i);
      }


      // do not use these methods...

      template<unsigned i>
      T _get_at() const {
	meta::lesseq<i, _Info::card>::ensure();
	return *(_buffer + i);
      }

      template<int i>
      T _get() const {
	meta::lesseq<-_Info::center, i>::ensure();
	meta::lesseq<i, _Info::card - _Info::center - 1>::ensure();
	return *(_buffer + _Info::center + i);
      }


    protected:

      T _buffer[_Info::card];
    };


    // ...but these static accessors:

    template<unsigned i, class Info, class T> inline
    T get_at(const meta::array1d<Info, T>& arr)
    {
      return arr.template _get_at<i>();
    }

    template<int i, class Info, class T> inline
    T get(const meta::array1d<Info, T>& arr)
    {
      return arr.template _get<i>();
    }

    // starter objects

    static internal::_array1d_start<int>   ints_1d   = internal::_array1d_start<int>();
    static internal::_array1d_start<float> floats_1d = internal::_array1d_start<float>();


  } // end of meta

} // end of oln

template<class Info, class T>
std::ostream& operator<<(std::ostream&				ostr,
			 const oln::meta::array1d<Info, T>&	rhs)
{
  for (int i = 0; i < Info::card; ++i)
    if (i == Info::center)
      ostr << "<" << rhs[i] << "> ";
    else
      ostr << rhs[i] << " ";
  ostr << std::endl;

  return ostr;
}


#endif // ! OLENA_META_ARRAY1D_HH
