#ifndef PROTO_OLN_CORE_PROPS_HH
# define PROTO_OLN_CORE_PROPS_HH

# include <mlc/types.hh>

# include <oln/core/cats.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  /*! \class default_props
  **
  ** \brief Class that defines properties by default, so properties are
  ** undefined.  // FIXME: this doc should be modified...
  **
  ** Practically all typedefs of default_props are thus set to
  ** mlc::undefined_type.
  **
  ** When props<E> is specialized, the programmer should derive that
  ** specialization from another props<E'> or from default_props.
  ** That ensures that an undefined property is set to mlc::undefined_type.
  **
  ** \see props<E>
  */
  template < typename category >
  struct default_props;


  // FIXME: doc
  template <>
  struct default_props < cat::image >
  {
    typedef mlc::undefined_type delegated_type;

    typedef mlc::undefined_type size_type;
    typedef mlc::undefined_type point_type;
    typedef mlc::undefined_type value_type;

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


  /*! \class props<E>
  **
  ** Declaration of the trait class for properties.
  ** Parameter E is the targeted type.  FIXME: rewrite doc.
  */
  template <typename category, typename type>
  struct props : public default_props <category>
  {};

  template <typename category, typename type>
  struct props <category, const type> : public props <category, type>
  {};



} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_PROPS_HH
