// Copyright (C) 2001, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_MACROS_HH
# define OLENA_CORE_MACROS_HH

# include <oln/core/cats.hh>
# include <oln/core/props.hh>



// FIXME: some crucial (meta-)work to be done here!
//
// first, introduce a piece of meta-code to get the proper abstraction
// from parameter; e.g., if I is an abstract::image then abstraction
// is abstract::image else etc.
//
// second, in macro below, there should be an exact_type from I somewhere
// cause we cannot be sure that I is an exact type; just think about it...





// for oln::abstract::image
//--------------------------

// unary macros

# define oln_delegated_type(T) typename oln::props<oln_category_type(T),T>::delegated_type

# define oln_size_type(T)  typename oln::props<oln_category_type(T),T>::size_type
# define oln_point_type(T) typename oln::props<oln_category_type(T),T>::point_type
# define oln_dpoint_type(T) typename oln::props<oln_category_type(T),T>::dpoint_type
# define oln_value_type(T) typename oln::props<oln_category_type(T),T>::value_type

# define oln_iter_type(T) typename oln::props<oln_category_type(T),T>::iter_type

# define oln_value_storage_type(T)  typename oln::props<oln_category_type(T),T>::value_storage_type
# define oln_value_container_type(T)  typename oln::props<oln_category_type(T),T>::value_container_type

# define oln_piter_type(T) typename oln::props<oln_category_type(T),T>::piter_type
# define oln_fwd_piter_type(T) typename oln::props<oln_category_type(T),T>::fwd_piter_type

// binary macros

# define oln_ch_value_type(I,V) typename oln::props<oln_category_type(I),I>::ch_value_type<V>::ret


// for oln::abstract::data_storage

# define oln_data_type(T)  typename oln::props<oln_category_type(T),T>::data_type



// generic macros
//----------------

# define oln_typeof( FROM_TYPE, WHAT_TYPE ) typename oln::props<oln_category_type(FROM_TYPE),FROM_TYPE>::WHAT_TYPE##_type



#endif // ! OLENA_CORE_MACROS_HH
