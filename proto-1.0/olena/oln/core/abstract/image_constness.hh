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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_CONSTNESS_HH
# define OLENA_CORE_ABSTRACT_IMAGE_CONSTNESS_HH

# include <oln/core/abstract/image.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {




    /*! \class abstract::readonly_image<E>
    **
    ** Class of images whose data are read-only.
    */


    template <typename E>
    struct readonly_image : public virtual image<E>,
			    public internal::get_image_impl < readonly_image<E>, E >
    {
    protected:

      /*! \brief Constructor (protected, empty).
      */
      readonly_image() {}
    };



    /*! \class abstract::readonly_image<E>
    **
    ** Class of images whose data are mutable (both readable and writable).
    **
    ** Parameter E is the exact type of image.
    */

    template <typename E>
    struct readwrite_image : public virtual image<E>,
			     public internal::get_image_impl < readwrite_image<E>, E >
    {
      // explanation:
      // ------------
      // though it works (uncommented) with g++-3.3, it is commented
      // for bwd compatibility purpose because of g++-2.95 that does not accept
      // "partial specialization declared friend"...
      /*
      friend class oln::value_box<E>;
      friend class oln::value_box<const E>;
      private:
      */


      /// typedef

      typedef oln_type_of(E, point) point_type;
      typedef oln_type_of(E, value) value_type;


      /*! \brief Write the value \a v at \a p in the current image.
      ** This method is used in value_box<I>.  Client should not use
      ** this method but abstract::image<I>::operator[](point)
      ** instead.
      **
      ** \see value_box, abstract::image<I>::operator[](point)
      */

      void set(const point_type& p, const value_type& v)
      {
	return this->exact().impl_set(p, v);
      }

      // FIXME: NEW:

//       template <typename A, typename V>
//       void& set(const point_type& p,
// 		void (I::*method)(A),
// 		const V& value)
//       {
// 	ima_->impl_set(p_, method, value);
// 	return *this;
//       }

    protected:

      /*! \brief Constructor (protected, empty).
      */
      readwrite_image() {}

      ~readwrite_image()
      {
// 	mlc_check_method_impl_2(E, void, set, const point_type&, const value_type&, );
      }
    };


    namespace internal {

      template <typename E>
      struct set_image_impl < readwrite_image<E>, E> : public virtual image_impl<E>
      {
	/// typedefs
	typedef typename image_impl<E>::D D;
	typedef oln_type_of(D, point) point_type;
	typedef oln_type_of(D, value) value_type;

	void impl_set(const point_type& p, const value_type& v)
	{
	  this->exact().impl_set_ante(p, v);
	  this->delegate().impl_set(p, v);
	  this->exact().impl_set_post(p, v);
	}

	// extra code; default is 'do nothing':

	void impl_set_ante(const point_type&, const value_type&) {}
	void impl_set_post(const point_type&, const value_type&) {}
      };

    } // end of namespace oln::abstract::internal



  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_CONSTNESS_HH
