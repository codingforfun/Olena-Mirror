#ifndef PROTO_OLN_CORE_ABSTRACT_DPOINT_HH
# define PROTO_OLN_CORE_ABSTRACT_DPOINT_HH

# include <mlc/any.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {

  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {

    /*! \class abstract::dpoint<E>
    **
    ** The abstract::dpoint class is the base class from whom derives
    ** every concrete dpoint.  A dpoint is a difference between a couple
    ** of points.
    **
    ** Parameter E is the exact type of dpoint.
    */
    template <typename E>
    struct dpoint : public mlc::any__best_memory<E>
    {

      /// Test equality of two dpoints.
      // FIXME: doc...
      bool operator==(const dpoint& rhs) const
      {
	return this->exact().impl_eq(rhs.exact());
      }

      /// Test difference between two dpoints.
      bool operator!=(const dpoint& rhs) const
      {
	return not this->operator==(rhs);
      }

    protected:

      dpoint() {}
    };

  } // end of namespace abstract

} // end of namespace oln



#endif // ! PROTO_OLN_CORE_ABSTRACT_DPOINT_HH
