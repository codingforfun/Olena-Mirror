#ifndef PROTO_OLN_CORE_CATS_HH
# define PROTO_OLN_CORE_CATS_HH

# include <mlc/types.hh>


/*! \macro FIXME:doc
*/

# define oln_category_type(T) typename oln::category_type<T>::ret



/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  /*! \class category_type<T>
  **
  ** FIXME: doc
  **
  */
  template <typename T>
  struct category_type
  {
    typedef mlc::undefined_type ret;
  };


  /*! \namespace oln::cat
  ** \brief oln::cat namespace.
  */
  namespace cat {

    struct image;
    struct data_storage;
    struct piter;
    struct point;
    struct size;
    // FIXME:...

  } // end of namespace oln::cat


} // end of namespace oln




#endif // ! PROTO_OLN_CORE_CATS_HH
