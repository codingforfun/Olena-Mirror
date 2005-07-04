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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_RAWNESS_HH
# define OLENA_CORE_ABSTRACT_IMAGE_RAWNESS_HH

# include <oln/core/abstract/image.hh>


//              image
//
//                ^
//                |                
//         ---------------
//        |               |
//
//    raw_image          ...



/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {




    /*! \class abstract::raw_image<E>
    **
    ** Class of images whose data are read-only.
    */


    template <typename E>
    struct raw_image : public virtual image<E>,
		       public internal::get_image_impl < raw_image<E>, E >
    {
    public:

      typedef oln_type_of(E, point) point_type;
      typedef oln_type_of(E, value_storage) value_storage_type;
      
      value_storage_type& at(const point_type& p)
      {
# ifdef OLNTRACE
 	inc_ncalls("set", *this);
# endif // ! OLNTRACE
	return this->exact().impl_at(p);
      }
      
      const value_storage_type& at(const point_type& p) const
      {
# ifdef OLNTRACE
 	inc_ncalls("get", *this);
# endif // ! OLNTRACE
	return this->exact().impl_at(p);
      }

    protected:

      /*! \brief Constructor (protected, empty).
      */
      raw_image() {}
    };


    template <typename E>
    struct not_raw_image : public virtual image<E>
    {
    protected:
      not_raw_image() {}
    };



    namespace internal {

      template <typename E>
      struct set_image_impl < raw_image<E>, E> : public virtual image_impl<E>
      {
	/// typedefs
	typedef typename image_impl<E>::D D;
	typedef oln_type_of(D, point) point_type;
	typedef oln_type_of(D, value_storage) value_storage_type;

	value_storage_type& at(const point_type& p)
	{
	  return this->delegate().at(p);
	}
      
	const value_storage_type& at(const point_type& p) const
	{
	  return this->delegate().at(p);
	}
      };

    } // end of namespace oln::abstract::internal



  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_RAWNESS_HH
