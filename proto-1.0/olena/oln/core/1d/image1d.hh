#ifndef OLENA_CORE_IMAGE1D_HH
# define OLENA_CORE_IMAGE1D_HH

# include <mlc/traits.hh>

# include <oln/core/abstract/image_with_data.hh>
# include <oln/core/1d/array1d.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  // fwd decl
  template <typename T> class image1d;

  // category decl
  template <typename T> struct category_type< image1d<T> > { typedef cat::image ret; };


  /*! \class props< abstract::image, image1d<T> >
  **
  ** Properties of common 1D images.  Specialization of props<abstraction,type>.
  ** Parameter T is the type of pixel values.
  **
  ** \see image1d<T>, props<category,type>
  */

  template <typename T>
  struct props < cat::image, image1d<T> > : public default_props < cat::image >
  {
    typedef mlc::no_type delegated_type;

    typedef size1d  size_type;
    typedef point1d point_type;
    typedef T       value_type;

    // please note that value_storage_type means data_type
    // since image1d is an image_with_data
    typedef mlc_encoding_type(T) value_storage_type;

    // please note that value_container_type means
    // data_container_type (or value_storage_container_type)
    // FIXME: find a better name...
    typedef array1d<value_storage_type> value_container_type;

    // tags
    typedef tag::readwrite constness_tag;

    // functions

    template <typename U>
    struct ch_value_type
    {
      typedef image1d<U> ret;
    };

  };



  /*! \class image1d<T>
  **
  ** Common class for 1D images.  Pixels values are stored in memory.
  ** FIXME: give details about other properties (border, etc.)
  **
  ** Parameter T is the type of pixel values.
  */

  template <typename T>
  class image1d : public abstract::image_with_data< image1d<T> >
  {
  public:

    typedef abstract::image_with_data< image1d<T> > super_type;

    image1d()
    {
      this->exact_ptr = this;
    }

    image1d(coord_t nindices) :
      super_type(size1d(nindices))
    {
      this->exact_ptr = this;
    }

    image1d(image1d& rhs) :
      super_type(rhs)
    {
      this->exact_ptr = this;
    }

  };


} // end of namespace oln


#endif // ! OLENA_CORE_IMAGE1D_HH
