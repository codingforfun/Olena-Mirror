#ifndef OLENA_CORE_ABSTRACT_IMAGE_CONSTNESS_HH
# define OLENA_CORE_ABSTRACT_IMAGE_CONSTNESS_HH

# include <oln/core/abstract/internal/image_impl.hh>
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


      /*! \brief Write the value \a v at \a p in the current image.
      ** This method is used in value_box<I>.  Client should not use
      ** this method but abstract::image<I>::operator[](point)
      ** instead.
      **
      ** \see value_box, abstract::image<I>::operator[](point)
      */

      template <typename V>
      void set(const oln_point_type(E)& p, const V& v)
      {
	return this->exact().impl_set(p, v);
      }

      // FIXME: NEW:

//       template <typename A, typename V>
//       void& set(const oln_point_type(E)& p,
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

    };


    namespace internal {

      template <typename E>
      struct set_image_impl < readwrite_image<E>, E> : public virtual image_impl<E>
      {
	typedef typename image_impl<E>::D D;

	template <typename V>
	void impl_set(const oln_point_type(D)& p, const V& v)
	{
	  this->exact().impl_set_ante(p, v);
	  this->delegate().impl_set(p, v);
	  this->exact().impl_set_post(p, v);
	}

	// extra code; default is 'do nothing':

	template <typename V>
	void impl_set_ante(const oln_point_type(D)&, const V&) {}

	template <typename V>
	void impl_set_post(const oln_point_type(D)&, const V&) {}
      };

    } // end of namespace oln::abstract::internal



  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_CONSTNESS_HH
