#ifndef OLENA_CORE_ABSTRACT_IMAGE_HH
# define OLENA_CORE_ABSTRACT_IMAGE_HH

# include <oln/core/abstract/internal/image_impl.hh>
# include <oln/core/cats.hh>
# include <oln/core/props.hh>
# include <oln/core/macros.hh>
# include <oln/core/value_box.hh>


/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {



  // FIXME: doc
  template <>
  struct default_props < cat::image >
  {
    typedef mlc::undefined_type delegated_type;

    typedef mlc::undefined_type size_type;
    typedef mlc::undefined_type point_type;
    typedef mlc::undefined_type value_type;

    typedef mlc::undefined_type piter_type;
    typedef mlc::undefined_type fwd_piter_type;

    typedef mlc::undefined_type value_storage_type;
    typedef mlc::undefined_type value_container_type;

    // FIXME: etc.

    template <typename T>
    struct ch_value_type
    {
      typedef mlc::undefined_type ret;
    };

  protected:
    default_props() {}
  };



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

      /*------------------*
       ! abstract methods !
       *------------------*/


      /*! \brief Return the size of the current image.  Nota bene: this method is abstract-like.it
      **  is a pseudo-abstract method.
      **
      ** \return An object deriving from abstract::size.  Ex: if the
      ** image is an image2d<something>, the returned object is a
      ** size2d.
      */

      const oln_size_type(E)& size() const
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

      bool hold(const oln_point_type(E)& p) const
      {
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

      bool hold_large(const oln_point_type(E)& p) const
      {
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

      bool impl_hold_large(const oln_point_type(E)& p) const
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

      oln_value_box_type(const E) operator[](const oln_point_type(E)& p) const
      {
	precondition(this->hold_large(p));
	oln_value_box_type(const E) tmp(this->exact(), p);
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

      oln_value_box_type(E) operator[](const oln_point_type(E)& p)
      {
	precondition(this->hold_large(p));
	oln_value_box_type(E) tmp(this->exact(), p);
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

      const oln_value_type(E) get(const oln_point_type(E)& p) const
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
	typedef typename image_impl<E>::D D;

	// delegations are "template methods" (Cf. the GOF's book)

	const oln_size_type(D)& impl_size() const
	{
	  const oln_size_type(D)& s = this->delegate().size();
	  this->exact().impl_size_extra(s);
	  return s;
	}

	unsigned long impl_npoints() const
	{
	  unsigned long n = this->delegate().npoints();
	  this->exact().impl_npoints_extra(n);
	  return n;
	}

	bool impl_hold(const oln_point_type(D)& p) const
	{
	  this->exact().impl_hold_extra(p);
	  return this->delegate().hold(p);
	}

	bool impl_hold_large(const oln_point_type(D)& p) const
	{
	  this->exact().impl_hold_large_extra(p);
	  return this->delegate().hold_large(p);
	}

	oln_value_box_type(D) operator[](const oln_point_type(D)& p) const
	{
	  precondition(this->hold_large(p));
	  return this->delegate().operator[](p);
	}

	oln_value_box_type(const D) operator[](const oln_point_type(D)& p)
	{
	  precondition(this->hold_large(p));
	  return this->delegate().operator[](p);
	}

	const oln_value_type(D) impl_get(const oln_point_type(D)& p) const
	{
	  this->exact().impl_get_extra(p);
	  return this->delegate().impl_get(p);
	}

	// extra code; default is "do nothing"

	void impl_size_extra(const oln_size_type(D)& s) const {}
	void impl_npoints_extra(unsigned long n) const {}

	void impl_hold_extra(const oln_point_type(D)& p) const {}
	void impl_hold_large_extra(const oln_point_type(D)& p) const {}

	void impl_get_extra(const oln_point_type(D)&) const {}
      };

    } // end of namespace oln::abstract::internal

  } // end of namespace oln::abstract

} // end of namespace oln




#endif // ! OLENA_CORE_ABSTRACT_IMAGE_HH
