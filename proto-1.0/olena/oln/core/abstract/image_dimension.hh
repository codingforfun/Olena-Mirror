#ifndef OLENA_CORE_ABSTRACT_IMAGE_DIMENSION_HH
# define OLENA_CORE_ABSTRACT_IMAGE_DIMENSION_HH

# include <oln/core/abstract/image.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {

  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {

    /*! \class abstract::image2d<E>
    **
    ** Class of 1d images.
    */


    template <typename E>
    struct image1d : public virtual image<E>
    {
    protected:

      /*! \brief Constructor (protected, empty).
      */
      image1d() {}
    };

    template <typename E>
    struct image2d : public virtual image<E>
    {
    protected:

      /*! \brief Constructor (protected, empty).
      */
      image2d() {}
    };
 
    template <typename E>
    struct image3d : public virtual image<E>
    {
    protected:

      /*! \brief Constructor (protected, empty).
      */
      image3d() {}
    };
 
 
  } // end of namespace oln::abstract

} // end of namespace oln
 

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_DIMENSION_HH
