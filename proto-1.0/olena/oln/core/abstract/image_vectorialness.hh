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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_VECTORIALNESS_HH
# define OLENA_CORE_ABSTRACT_IMAGE_VECTORIALNESS_HH

# include <mlc/switch.hh>
# include <ntg/basics.hh>
# include <oln/core/abstract/image.hh>


/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {

  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {

    template <typename E>
    struct data_type_image : public virtual image<E>
    {
    protected:
      data_type_image() {}
    };   
 
    template <typename E>
    struct vectorial_image : public data_type_image<E>
    {
    protected:
      vectorial_image() {}
    };

    template <typename E>
    struct non_vectorial_image : public data_type_image<E>
    {
    protected:
      non_vectorial_image() {}
    };

    template <typename E>
    struct decimal_image : public non_vectorial_image<E>
    {
    protected:
      decimal_image() {}
    }; 

    template <typename E>
    struct integer_image : public non_vectorial_image<E>
    {
    protected:
      integer_image() {}
    };  

    template <typename E>
    struct binary_image : public non_vectorial_image<E>
    {
    protected:
      binary_image() {}
    };  


  } // end of namespace oln::abstract

  template <typename T>
  struct vectorialness_from_valuetype
  {
    typedef typename mlc::bool_switch_<

        mlc::bool_case_<ntg_is_a(T, ntg::binary)::val,
			is_a<abstract::binary_image>,

        mlc::bool_case_<ntg_is_a(T, ntg::integer)::val,
			is_a<abstract::integer_image>,

        mlc::bool_case_<ntg_is_a(T, ntg::decimal)::val,
			is_a<abstract::decimal_image>,

        mlc::bool_case_<ntg_is_a(T, ntg::vectorial)::val,
			is_a<abstract::vectorial_image>,

        mlc::bool_case_<ntg_is_a(T, ntg::non_vectorial)::val,
			is_a<abstract::non_vectorial_image>,
			
        mlc::bool_case_<true,
	      	        is_a<abstract::data_type_image> >

      > > > > > >::ret ret;
  };

  #define vectorialness_from_valuetype(T) \
  typename vectorialness_from_valuetype< T >::ret
  #define vectorialness_from_valuetype_(T) \
  vectorialness_from_valuetype< T >::ret

} // end of namespace oln
 

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_VECTORIALNESS_HH
