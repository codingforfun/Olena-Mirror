#ifndef OLENA_CORE_IMAGE2D_HH
# define OLENA_CORE_IMAGE2D_HH

# include <mlc/traits.hh>

# include <oln/core/abstract/image_with_data.hh>
# include <oln/core/2d/array2d.hh>
# include <oln/core/2d/fwd_piter2d.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  // fwd decl
  template <typename T> class image2d;

  // category decl
  template <typename T>
  struct category_type< image2d<T> > { typedef cat::image ret; };


  /*! \class props< abstract::image, image2d<T> >
  **
  ** Properties of common 2D images.  Specialization of props<abstraction,type>.
  ** Parameter T is the type of pixel values.
  **
  ** \see image2d<T>, props<category,type>
  */

  template <typename T>
  struct props < cat::image, image2d<T> > : public default_props < cat::image >
  {
    typedef mlc::no_type delegated_type;

    typedef size2d  size_type;
    typedef point2d point_type;
    typedef T       value_type;

    typedef fwd_piter2d piter_type;
    typedef fwd_piter2d fwd_piter_type;

    // please note that value_storage_type means data_type
    // since image2d is an image_with_data
    typedef mlc_encoding_type(T) value_storage_type;

    // please note that value_container_type means
    // data_container_type (or value_storage_container_type)
    // FIXME: find a better name...
    typedef array2d<value_storage_type> value_container_type;

    // tags
    typedef tag::readwrite constness_tag;
    typedef tag::dimension2 dimension_tag;

    // functions

    template <typename U>
    struct ch_value_type
    {
      typedef image2d<U> ret;
    };

  };



  /*! \class image2d<T>
  **
  ** Common class for 2D images.  Pixels values are stored in memory.
  ** FIXME: give details about other properties (border, etc.)
  **
  ** Parameter T is the type of pixel values.
  */

  template <typename T>
  class image2d : public abstract::image_with_data< image2d<T> >
  {
  public:

    typedef abstract::image_with_data< image2d<T> > super_type;

    image2d()
    {
      this->exact_ptr = this;
    }

    image2d(coord_t nrows, coord_t ncols) :
      super_type(size2d(nrows, ncols))
    {
      this->exact_ptr = this;
    }

    image2d(image2d& rhs) :
      super_type(rhs)
    {
      this->exact_ptr = this;
    }

//     image2d& operator=(image2d& rhs)
//     {
//       if (&rhs == this)
// 	return *this;
//       super_type::operator=(rhs);
//       return *this;
//     };

//     template <typename I>
//     image2d& operator=(const I& rhs)
//     {
//       assign(*this, rhs);
//       return *this;
//     }

    // FIXME: idem with abstract::image2d<E> (?)

  };


} // end of namespace oln


#endif // ! OLENA_CORE_IMAGE2D_HH
