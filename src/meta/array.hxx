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

#ifndef OLENA_META_ARRAY_HXX
# define OLENA_META_ARRAY_HXX


namespace oln {

  namespace meta {


    // fwd decl
    template<int, int, int, int, class> struct array2d;


    namespace internal {


      //
      //  meta::internal misc
      //
      ////////////////////////////////////////

      static const unsigned _nada = 666666;
      static const unsigned _max_card = 4096;  // 64 x 64

      // _get_n_from

      template<unsigned i, unsigned cur_nrows, unsigned cur_ncols>
      struct _get_n_from {
	enum { nrows = i / cur_ncols,
	       ncols = cur_ncols,
	       ok    = ((i % cur_ncols) == 0)
	};
      };

      template<unsigned i>
      struct _get_n_from<i,_nada,_nada> {
	enum { nrows = 1,
	       ncols = i,
	       ok    = 1
	};
      };

      template<unsigned i, unsigned cur_nrows>
      struct _get_n_from<i,cur_nrows,_nada>;

      // _get_x_from

      template<unsigned cur_xi, unsigned nrows, unsigned ncols>
      struct _get_x_from {
	enum { xrow = cur_xi / ncols,
	       xcol = cur_xi - xrow * ncols,
	       ok   = 2
	};
      };

      template<unsigned nrows, unsigned ncols>
      struct _get_x_from<_nada,nrows,ncols> {
	enum { ok   = (nrows % 2) * (ncols % 2),
	       xrow = ok * (nrows / 2),
	       xcol = ok * (ncols / 2)
	};
      };

      // fwd decl

      template<class T, unsigned i, unsigned cur_xi, unsigned cur_nrows, unsigned cur_ncols>
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

	_array2d_elt<T,1,_nada,_nada,_nada> operator=(T val);
	_array2d_elt<T,1,    0,_nada,_nada> operator=(oln::internal::_x<T> val);
	_array2d_elt<T,1,    0,_nada,_nada> operator=(oln::internal::_x<void> val);

	T ptr[_max_card]; // could be static
      };



      //
      //  meta::internal::_array2d_elt
      //
      ////////////////////////////////////////

      template<class T,
	       unsigned i,
	       unsigned cur_xi = _nada,
	       unsigned cur_nrows = _nada, unsigned cur_ncols = _nada>
      struct _array2d_elt
      {
	typedef _array2d_elt<T,i+1,cur_xi,cur_nrows,cur_ncols> _next_elt_t;

	enum {
	  _nrows = _get_n_from<i,cur_nrows,cur_ncols>::nrows,
	  _ncols = _get_n_from<i,cur_nrows,cur_ncols>::ncols,
	  _xrow  = _get_x_from<cur_xi,_nrows,_ncols>::xrow,
	  _xcol  = _get_x_from<cur_xi,_nrows,_ncols>::xcol,

	  b_row = - int(_xrow),
	  e_row = int(_nrows) - int(_xrow),
	  b_col = - int(_xcol),
	  e_col = int(_ncols) - int(_xcol)
	};

	typedef array2d<b_row, e_row,
			b_col, e_col, T> _array2d_t;

      public:

	_array2d_elt(T* ptr, _array2d_start<T>* arr) : ptr(ptr), arr(arr)
	{
	}

	_next_elt_t operator,(T val)
	{
	  meta::eq<cur_nrows,_nada>::ensure();
	  *ptr = val;
	  return _next_elt_t(ptr+1,arr);
	}

	_array2d_elt<T,i+1,i,cur_nrows,cur_ncols>
	operator,(oln::internal::_x<T> val)
	{
	  meta::eq<cur_xi,_nada>::ensure();
	  *ptr = val.ue;
	  // cur_xi <- i
	  return _array2d_elt<T,i+1,i,cur_nrows,cur_ncols>(ptr+1,arr);
	}

	_array2d_elt<T,i+1,i,cur_nrows,cur_ncols>
	operator,(oln::internal::_x<void>)
	{
	  meta::eq<cur_xi,_nada>::ensure();
	  *ptr = T(0);
	  // cur_xi <- i
	  return _array2d_elt<T,i+1,i,cur_nrows,cur_ncols>(ptr+1,arr);
	}

	_array2d_elt<T,i,cur_xi,cur_nrows,i>
	operator,(oln::internal::_lbrk)
	{
	  meta::eq<cur_ncols,_nada>::ensure();
	  meta::neq<i,0>::ensure();
	  // cur_ncols <- i
	  return _array2d_elt<T,i,cur_xi,cur_nrows,i>(ptr,arr);
	}

	_array2d_t
	operator,(oln::internal::_end)
	{
	  // array is well-formed:
	  meta::eq<_get_n_from<i,cur_nrows,cur_ncols>::ok,1>::ensure();
	  // centering is automatic (ok == 1) or user-defined (ok == 2):
	  meta::neq<_get_x_from<cur_xi,_nrows,_ncols>::ok,0>::ensure();

	  return _array2d_t(arr->ptr);
	}

	template<unsigned br, unsigned er, unsigned bc, unsigned ec, class U>
	void operator,(array2d<br,er,bc,ec,U>);

	template<class U>
	here_a_value_is_not_of_type_<T> operator,(U u) const;

	T* ptr;
	_array2d_start<T>* arr;
      };


      //
      //  meta::internal::_array2d_start  impl
      //
      ////////////////////////////////////////

      template<class T> inline _array2d_elt<T,1,_nada,_nada,_nada>
      _array2d_start<T>::operator=(T val)
      {
	ptr[0] = val;
	return _array2d_elt<T,1,_nada,_nada,_nada>(ptr+1,this);
      }

      template<class T> inline _array2d_elt<T,1,0,_nada,_nada>
      _array2d_start<T>::operator=(oln::internal::_x<T> val)
      {
	ptr[0] = val.ue;
	// cur_xi <- 0
	return _array2d_elt<T,1,0,_nada,_nada>(ptr+1,this);
      }

      template<class T> inline _array2d_elt<T,1,0,_nada,_nada>
      _array2d_start<T>::operator=(oln::internal::_x<void> val)
      {
	ptr[0] = T(0);
	// cur_xi <- 0
	return _array2d_elt<T,1,0,_nada,_nada>(ptr+1,this);
      }


    } // end of internal

  } // end of meta

} // end of oln


#endif // ! OLENA_META_ARRAY_HXX
