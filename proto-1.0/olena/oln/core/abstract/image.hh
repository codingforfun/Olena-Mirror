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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_HH
# define OLENA_CORE_ABSTRACT_IMAGE_HH

# include <mlc/types.hh>

# include <oln/core/abstract/internal/image_impl.hh>
# include <oln/core/properties.hh>
# include <oln/core/value_box.hh>


/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  // fwd decl
  namespace abstract {
    template <typename E> struct image;
    template <typename E> struct readonly_image;
  }

  // category
  template <typename E>
  struct set_category< abstract::image<E> > { typedef category::image ret; };



  /// properties of any type in category::image

  template <typename type>
  struct props_of <category::image, type>
  {
    typedef mlc::true_type user_defined_;
    
    mlc_decl_prop(category::image, value_type);
    mlc_decl_prop(category::image, point_type);
    mlc_decl_prop(category::image, size_type);
    mlc_decl_prop(category::image, fwd_piter_type);
    mlc_decl_prop_with_default(category::image, value_storage_type, mlc::no_type);
    mlc_decl_prop_with_default(category::image, storage_type, mlc::no_type);
    mlc_decl_prop_with_default(category::image, delegated_type, mlc::no_type);

    mlc_decl_prop_with_default(category::image, image_constness_type, is_a<abstract::readonly_image>);
    mlc_decl_prop(category::image, image_dimension_type);
    mlc_decl_prop(category::image, image_vectorialness_type);

    //...

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of(" // FIXME: << typeid(oln::category::image).name()
	   << ", " << typeid(type).name() << ") = {"
	   << "  value_type = " << typeid(value_type).name()
	   << "  point_type = " << typeid(point_type).name()
	   << "  size_type = " << typeid(size_type).name()
	   << "  fwd_piter_type = " << typeid(fwd_piter_type).name()
	   << "  value_storage_type = " << typeid(value_storage_type).name()
	   << "  storage_type = " << typeid(storage_type).name()
	   << "  delegated_type = " << typeid(delegated_type).name()

	   << "  image_constness_type = " << typeid(image_constness_type).name()
	   << "  image_dimension_type = " << typeid(image_dimension_type).name()
	   << "  image_vectorialness_type = " << typeid(image_vectorialness_type).name()

	   << "  }" << std::endl;
    }

  };

  mlc_register_prop(category::image, value_type);
  mlc_register_prop(category::image, point_type); 
  mlc_register_prop(category::image, size_type); 
  mlc_register_prop(category::image, fwd_piter_type); 
  mlc_register_prop(category::image, value_storage_type);
  mlc_register_prop(category::image, storage_type);
  mlc_register_prop(category::image, delegated_type);

  mlc_register_prop(category::image, image_constness_type);
  mlc_register_prop(category::image, image_dimension_type);
  mlc_register_prop(category::image, image_vectorialness_type);




  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {


    /*! \class abstract::image<E>
    **
    ** The abstract::image class is the base class from whom derives
    ** every concrete image class.  Basically, an image is a set of
    ** points and a set of values associated with those points.
    **
    ** Parameter E is the exact type of image.
    */


    template <typename E>
    struct image : public internal::get_image_impl < image<E>, E>
    {

      /// typedefs

      typedef oln_type_of(E, size)  size_type;
      typedef oln_type_of(E, value) value_type;
      typedef oln_type_of(E, point) point_type;


      /*------------------*
       ! abstract methods !
       *------------------*/


      /*! \brief Return the size of the current image.  Nota bene:
      **  this method is abstract-like.it is a pseudo-abstract method.
      **
      ** \return An object deriving from abstract::size.  Ex: if the
      ** image is an image2d<something>, the returned object is a
      ** size2d.
      */

      const size_type& size() const
      {
	return this->exact().impl_size();
      }


      /*! \brief Return the number of points of the current image.
      **  Nota bene: this method is abstract-like.
      **
      ** FIXME: this method should be deferred in an abstract subclass
      ** since its complexity can vary from a concrete image to another...
      **
      ** \return A positive value.  It can be 0 when the image size is
      ** not yet defined; ex: image2d<int> ima; cout << ima.npoints();
      */

      unsigned long npoints() const
      {
	return this->exact().impl_npoints();
      }


      /*! \brief Test if the point \a p belongs to the current image.
      ** Please note that a point of the outer boundary of an image
      ** does NOT belong to the image.  Nota bene: this method is
      ** abstract-like.
      **
      ** \return True if p belongs to the current image, false otherwise.
      **
      ** \see hold_large
      */

      bool hold(const point_type& p) const
      {
	precondition(this->npoints() != 0);
	return this->exact().impl_hold(p);
      }


      /*! \brief Test if \a p is a proper point to access a value of
      ** the current image.  When an image has an outer boundary, the
      ** points of this boundary have values and these values can be
      ** accessed.  Nota bene: this method is polymorphic so it can be
      ** overriden in derived classes; by default, hold_large behaves
      ** like hold.
      **
      ** FIXME: hold_large should come only for images WITH a border!
      **
      ** \return True if (*this)[p] is ok, false otherwise.
      **
      ** \see hold
      */

      bool hold_large(const point_type& p) const
      {
	precondition(this->npoints() != 0);
	return this->exact().impl_hold_large(p);
      }


      /*! \brief Default implementation for hold_large.  If not
      **  overriden in derived class, it just calls hold(p).
      **
      ** FIXME: hold_large should come only for images WITH a border!
      **
      ** \return True if (*this)[p] is ok, false otherwise.
      **
      ** \see hold_large
      */

      bool impl_hold_large(const point_type& p) const
      {
	return this->hold(p);
      }



      /*------------------*
       ! concrete methods !
       *------------------*/


      /*! \brief Give access to the value stored at \a p in the
      ** current image.  Precisely it returns a box that encloses this
      ** value.  FIXME: say more about that box!
      **
      ** \precondition this->hold_large(p)
      **
      ** \see value_box
      */

      value_box<const E> operator[](const point_type& p) const
      {
	precondition(this->npoints() != 0);
	precondition(this->hold_large(p));
	value_box<const E> tmp(this->exact(), p);
	return tmp;
      }


      /*! \brief Gives access to the value stored at \a p in the
      ** current image.  Precisely it returns a box that encloses this
      ** value.
      **
      ** \precondition this->hold_large(p)
      **
      ** \see value_box
      */

      value_box<E> operator[](const point_type& p)
      {
	precondition(this->npoints() != 0);
	precondition(this->hold_large(p));
	value_box<E> tmp(this->exact(), p);
	return tmp;
      }


      /*! \brief Destructor (empty).
      */

      virtual ~image() {}



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

      /*! \brief Read-only access to the value stored at \a p in the
      ** current image.  This method is used in value_box<I>.  Client
      ** should not use this method but
      ** abstract::image<I>::operator[](point) instead.
      **
      ** \return The value stored at \a p.
      **
      ** \see value_box, abstract::image<I>::operator[](point)
      */

      const value_type get(const point_type& p) const
      {
	return this->exact().impl_get(p);
      }

    protected:

      /*! \brief Constructor (protected, empty).
      */
      image() {}

      /*! \brief Cpy constructor (protected, empty).
      */
      image(const image&) {}

      /*! \brief Op= (protected, empty).
      */
      void operator=(const image&) {}

    };



    /*! \namespace oln::abstract::internal
    ** \brief oln::abstract::internal namespace.
    */
    namespace internal {

      template <typename E>
      struct set_image_impl < image<E>, E > : public virtual image_impl<E>
      {

	/// typedefs

	typedef typename image_impl<E>::D D;

	typedef oln_type_of(D, size)  size_type;
	typedef oln_type_of(D, point) point_type;
	typedef oln_type_of(D, value) value_type;


	// delegations are "template methods" (Cf. the GOF's book)

	const size_type& impl_size() const
	{
	  const size_type& s = this->delegate().size();
	  this->exact().impl_size_extra(s);
	  return s;
	}

	unsigned long impl_npoints() const
	{
	  unsigned long n = this->delegate().npoints();
	  this->exact().impl_npoints_extra(n);
	  return n;
	}

	bool impl_hold(const point_type& p) const
	{
	  this->exact().impl_hold_extra(p);
	  return this->delegate().hold(p);
	}

	bool impl_hold_large(const point_type& p) const
	{
	  this->exact().impl_hold_large_extra(p);
	  return this->delegate().hold_large(p);
	}

	value_box<D> operator[](const point_type& p) const
	{
	  precondition(this->hold_large(p));
	  return this->delegate().operator[](p);
	}

	value_box<const D> operator[](const point_type& p)
	{
	  precondition(this->hold_large(p));
	  return this->delegate().operator[](p);
	}

	const value_type impl_get(const point_type& p) const
	{
	  this->exact().impl_get_extra(p);
	  return this->delegate().impl_get(p);
	}

	// extra code; default is "do nothing"

	void impl_size_extra(const size_type& s) const {}
	void impl_npoints_extra(unsigned long n) const {}

	void impl_hold_extra(const point_type& p) const {}
	void impl_hold_large_extra(const point_type& p) const {}

	void impl_get_extra(const point_type&) const {}
      };

    } // end of namespace oln::abstract::internal

  } // end of namespace oln::abstract

} // end of namespace oln




#endif // ! OLENA_CORE_ABSTRACT_IMAGE_HH
