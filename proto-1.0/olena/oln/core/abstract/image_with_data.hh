#ifndef OLENA_CORE_ABSTRACT_IMAGE_WITH_DATA_HH
# define OLENA_CORE_ABSTRACT_IMAGE_WITH_DATA_HH


# include <mlc/tracked_ptr.hh>

# include <oln/core/tags.hh>
# include <oln/core/macros.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {


    /*! \class abstract::image_with_data<E>
    **
    ** Class for images that contains data (a value per image point).
    **
    ** Parameter E is the exact type of image.
    */

    template <typename E>
    class image_with_data : public image_entry<E>
    {

    public:


      /*! \brief Implement abstract::image<E>::size() so return the
      **  size of the current image.
      **
      ** \return An object deriving from abstract::size.  Ex: if the
      ** image is an image2d<something>, the returned object is a
      ** size2d.
      */

      const oln_size_type(E)& impl_size() const
      {
	return this->data_->size();
      }


      /*! \brief Implement abstract::image<E>::npoints() so return the
      **  size of the current image.
      **
      ** \return A positive value.  It can be 0 when the image size is
      ** not yet defined; ex: image2d<int> ima; cout << ima.npoints();
      */

      unsigned long impl_npoints() const
      {
	return this->data_->npoints();
      }


      /*! \brief Implement abstract::image<E>::hold(p) so test if the
      ** point \a p belongs to the current image.  Please note that a
      ** point of the outer boundary of an image does NOT belong to
      ** the image.
      **
      ** \return True if p belongs to the current image, false otherwise.
      */

      bool impl_hold(const oln_point_type(E)& p) const
      {
	return this->data_->hold(p);
      }


      /*! \brief Implement both abstract::readonly_image<E>::get(p)
      ** and abstract::readwrite_image<E>::get(p) so read-only access
      ** to the value stored at \a p in the current image.
      **
      ** \return True if p belongs to the current image, false otherwise.
      */

      const oln_value_type(E) impl_get(const oln_point_type(E)& p) const
      {
	return this->data_->get(p);
      }


      /*! \brief Implement abstract::readwrite_image<E>::set(p, v) so
      ** write the value \a v at \a p in the current image.
      */

      template <typename V>
      void impl_set(const oln_point_type(E)& p, const V& v)
      {
	return this->data_->set(p, v);
      }

    protected:

      /*! \brief Constructor (protected) with no memory allocation for
      ** data.
      */
      image_with_data()
      {
	data_ = 0;
      }

      /*! \brief Constructor (protected) with memory allocation for
      ** data.
      */
      image_with_data(const oln_size_type(E)& size)
      {
	data_ = new oln_value_container_type(E)(size);
      }


      /*! \brief Copy constructor (protected) with shallow data copy.
      */
      image_with_data(image_with_data& rhs)
      {
	this->data_ = rhs.data_;
      }


      /*! \brief Data storage.
      */
      mlc::tracked_ptr<oln_value_container_type(E)> data_;

    };


  } // end of namespace abstract


} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_WITH_DATA_HH
