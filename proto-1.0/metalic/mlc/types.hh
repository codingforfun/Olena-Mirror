#ifndef METALIC_TYPES_HH
# define METALIC_TYPES_HH


/*! \namespace mlc
** \brief mlc namespace.
*/
namespace mlc {


  /*! \class undefined_type
  **
  ** Type by default.  If the programmer forgets to define a type,
  ** actually this type should however exists and should be set to
  ** undefined_type.
  */
  class undefined_type { private: undefined_type(); };

  // FIXME: document.
  class unknown_type { private: unknown_type(); };
  class no_type      { private: no_type(); };


} // end of namespace mlc


#endif // ! METALIC_TYPES_HH
