#ifndef OLENA_CORE_IMAGE3D_HH
# define OLENA_CORE_IMAGE3D_HH

# include <mlc/traits.hh>

# include <oln/core/abstract/image_with_data.hh>
# include <oln/core/3d/array3d.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  // fwd decl
  template <typename T> class image3d;

  // category decl
  template <typename T> 
  struct category_type< image3d<T> > { typedef cat::image ret; };


  /*! \class props< abstract::image, image3d<T> >
  **
  ** Properties of common 3D images.  Specialization of props<abstraction,type>.
  ** Parameter T is the type of pixel values.
  **
  ** \see image3d<T>, props<category,type>
  */

  template <typename T>
  struct props < cat::image, image3d<T> > : public default_props < cat::image >
  {
    typedef mlc::no_type delegated_type;

    typedef size3d  size_type;
    typedef point3d point_type;
    typedef T       value_type;

    // please note that value_storage_type means data_type
    // since image3d is an image_with_data
    typedef mlc_encoding_type(T) value_storage_type;

    // please note that value_container_type means
    // data_container_type (or value_storage_container_type)
    // FIXME: find a better name...
    typedef array3d<value_storage_type> value_container_type;

    // tags
    typedef tag::readwrite constness_tag;

    // functions

    template <typename U>
    struct ch_value_type
    {
      typedef image3d<U> ret;
    };

  };



  /*! \class image3d<T>
  **
  ** Common class for 3D images.  Pixels values are stored in memory.
  ** FIXME: give details about other properties (border, etc.)
  **
  ** Parameter T is the type of pixel values.
  */

  template <typename T>
  class image3d : public abstract::image_with_data< image3d<T> >
  {
  public:

    typedef abstract::image_with_data< image3d<T> > super_type;

    image3d()
    {
      this->exact_ptr = this;
    }

    image3d(coord_t nslices, coord_t nrows, coord_t ncols) :
      super_type(size3d(nslices, nrows, ncols))
    {
      this->exact_ptr = this;
    }

    image3d(image3d& rhs) :
      super_type(rhs)
    {
      this->exact_ptr = this;
    }

  };


} // end of namespace oln


#endif // ! OLENA_CORE_IMAGE3D_HH
