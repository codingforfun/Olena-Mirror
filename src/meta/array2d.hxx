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

#ifndef OLENA_META_ARRAY2D_HXX
# define OLENA_META_ARRAY2D_HXX

#include "meta/arraynd.hh"

namespace oln {

  namespace meta {
    
    //
    //  meta::array2d_info
    //
    ////////////////////////////////////////
    

    // _nrows	-> total number of rows (1-indexed)
    // _ncols	-> total number of columns (1-indexed)

    // _center	-> position of the central element (0-indexed)
    //		   domain : [ 0, card [

    // _i	-> number of elements that have been eaten yet (0-indexed)
    //		   domain : [ 0, card ] -> watch out !!



    // Center is user-defined

    template < unsigned _nrows, 
	       unsigned _ncols,
	       unsigned _center = _ncols * (_nrows / 2) + (_ncols / 2),
	       unsigned _i = _nrows * _ncols>
    struct array2d_info
    {
      enum {
	nrows = _nrows,
	ncols = _ncols,
	center = _center,
	center_row = _center / _ncols,
	center_col = _center % _ncols,
	card = _nrows * _ncols,
	i = _i,
	well_formed = ((_i % _ncols) == 0),
	get_real_center = _center
      };

      typedef array2d_info< _nrows, _ncols, _center, i + 1 > next_elt;

    };
    
    // Center is defined automatically, if nrows and ncols are both odd

    template < unsigned _nrows, unsigned _ncols, unsigned _i >
    struct array2d_info <_nrows, _ncols, internal::_unknown, _i >
    {
      enum {
	nrows = _nrows,
	ncols = _ncols,
	center = internal::_unknown,
	i = _i,
	card = _nrows * _ncols,
	well_formed = ((_i % _ncols) == 0),
	get_real_center = _i / 2
      };

      typedef array2d_info< _nrows, _ncols, internal::_unknown, i + 1 > next_elt;

    };

    // fwd decl
    template<class, class> struct array2d;


    namespace internal {

      // fwd decl

      template<class T, class Info>
      struct _array2d_elt;

      // for error messages

      template<class U>
      struct here_a_value_is_not_of_type_;


      //
      //  meta::internal::_array2d_start decl
      //
      ////////////////////////////////////////

      template<class T>
      struct _array2d_start {

	_array2d_elt< T, array2d_info< _unknown, _unknown, _unknown, 1 > > 
	operator=(T val);
	
	_array2d_elt< T, array2d_info< _unknown, _unknown, 0, 1 > > 
	operator=(oln::internal::_x<T> val);
	
	_array2d_elt< T, array2d_info< _unknown, _unknown, 0, 1 > > 
	operator=(oln::internal::_x<void> val);

	T ptr[_max_card]; // could be static
      };



      //
      //  meta::internal::_array2d_elt
      //
      ////////////////////////////////////////

      template<class T, class Info>
      struct _array2d_elt
      {
	typedef _array2d_elt< T, typename Info::next_elt >
	_next_elt_t;

	typedef _array2d_elt< T, array2d_info< Info::nrows, Info::ncols, Info::i, Info::i + 1> >
	_eat_center_t;

	typedef _array2d_elt< T, array2d_info< Info::nrows, Info::i, Info::center, Info::i > >
	_eat_lbrk_t;

	typedef array2d< array2d_info< Info::i / Info::ncols, Info::ncols, Info::get_real_center, Info::i > , T>
	_array2d_t;

      public:
	
	// Constructor

	_array2d_elt(T* ptr, _array2d_start<T>* arr) : ptr(ptr), arr(arr)
	{
	}


	//
	// Overloading "operator,"
	//


	// elt, elt

	_next_elt_t operator,(T val)
	{
	  meta::eq<Info::nrows, _unknown>::ensure();
	  *ptr = val;
	  return _next_elt_t(ptr + 1, arr);
	}


	// elt, x(elt)		-> center

	_eat_center_t operator,(oln::internal::_x<T> val)
	{
	  meta::eq<Info::center, _unknown>::ensure();
	  *ptr = val.ue; // FIXME : give a *name* to this variable !!
	  return _eat_center_t(ptr + 1, arr);
	}


	// elt, x()		-> center

	_eat_center_t operator,(oln::internal::_x<void>)
	{
	  meta::eq<Info::center, _unknown>::ensure();
	  *ptr = T(0);
	  return _eat_center_t(ptr + 1, arr);
	}


	// elt, lbrk

	_eat_lbrk_t operator,(oln::internal::_lbrk)
	{
	  meta::eq<Info::ncols, _unknown>::ensure();
	  meta::neq<Info::ncols, 0>::ensure();
	  return _eat_lbrk_t(ptr, arr);
	}


	// elt, end
	
	_array2d_t operator,(oln::internal::_end)
	{
	  enum { nrows = Info::i / Info::ncols };
	  
	  // array is well-formed :
	  meta::eq< Info::well_formed, true >::ensure();
	  // centering is automatic or user-defined :
	  
	  meta::neq< Info::ncols, _unknown>::ensure();

	  
	  meta::logical_or<
	    // both nrows and ncols are odd...
	    meta::logical_and<
	    meta::eq< Info::ncols % 2, 1 >::ret,
	    meta::eq< nrows % 2, 1 >::ret
	    >::ret,
	    // ... or the center is user-defined
	    meta::neq< Info::center, _unknown >::ret
	    
	    >::ensure();
	  
	  return _array2d_t(arr->ptr);
	}
	

	// else -> error

	template<class U>
	void operator,(array2d< Info, U >);

	template<class U>
	here_a_value_is_not_of_type_<T> operator,(U u) const;

	T* ptr;
	_array2d_start<T>* arr;
      };



      //
      //  meta::internal::_array2d_start  impl
      //
      ////////////////////////////////////////

      template<class T> inline 
      _array2d_elt< T, array2d_info< _unknown, _unknown, _unknown, 1 > >
      _array2d_start<T>::operator=(T val)
      {
	ptr[0] = val;
	return _array2d_elt< T, array2d_info< _unknown, _unknown, _unknown, 1 > >(ptr+1,this);
      }

      template<class T> inline 
      _array2d_elt< T, array2d_info< _unknown, _unknown, 0, 1 > >
      _array2d_start<T>::operator=(oln::internal::_x<T> val)
      {
	ptr[0] = val.ue;
	// center <- 0
	return _array2d_elt< T, array2d_info< _unknown, _unknown, 0, 1 > >(ptr+1,this);
      }

      template<class T> inline 
      _array2d_elt< T, array2d_info< _unknown, _unknown, 0, 1 > >
      _array2d_start<T>::operator=(oln::internal::_x<void> val)
      {
	ptr[0] = T(0);
	// center <- 0
	return _array2d_elt< T, array2d_info< _unknown, _unknown, 0, 1 > >(ptr+1,this);
      }


    } // end of internal

  } // end of meta

} // end of oln


#endif // ! OLENA_META_ARRAY2D_HXX
