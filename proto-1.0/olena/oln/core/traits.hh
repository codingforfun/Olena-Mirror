// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_TRAITS_HH
# define OLENA_CORE_TRAITS_HH

# include <ntg/core/type_traits.hh>
# include <oln/core/typedefs.hh>


namespace oln
{


  namespace traits
  {
    
    template <typename T>
    struct storage_type
    {
      typedef T ret;
    };

    namespace internal {

      template <typename T,
		typename is_an_ntg_type>
      struct storage_type_helper
      {
	typedef typename ntg::type_traits<T>::storage_type ret;
      };

      template <typename T>
      struct storage_type_helper <T,
				  mlc::internal::not_found>
      // so when it is not an ntg type, use client traits
      {
	typedef typename traits::storage_type<T>::ret ret;
      };

    } // end of namespace oln::traits::internal
    
  } // end of namespace oln::traits


  template <typename T>
  struct f_storage_type
  {
    typedef typename
    traits::internal::storage_type_helper< T,
					   mlc_typedef_of(ntg::type_traits<T>,
							  storage_type )
      >::ret
    ret;
  };


} // end of namespace oln


# define oln_storage_type(T) typename oln::f_storage_type<T>::ret


#endif // ! OLENA_CORE_TRAITS_HH
