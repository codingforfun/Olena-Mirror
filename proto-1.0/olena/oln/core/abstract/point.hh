#ifndef PROTO_OLN_CORE_ABSTRACT_POINT_HH
# define PROTO_OLN_CORE_ABSTRACT_POINT_HH

# include <mlc/any.hh>

# include <oln/core/cats.hh>
# include <oln/core/props.hh>
# include <oln/core/macros.hh>


/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  /// fwd decl
  namespace abstract {
    template <typename E> struct point;
  }


  /*! \class default_props< cat::point >
  **
  ** Default properties for points.  Specialization of
  ** default_props<category>.
  */
  template <>
  struct default_props < cat::point >
  {
    typedef mlc::undefined_type dpoint_type;
  };
  

  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {

    /*! \class abstract::point<E>
    **
    ** The abstract::point class is the base class from whom derives
    ** every concrete point.
    **
    ** Parameter E is the exact type of point.
    */
    template <typename E>
    struct point : public mlc::any__best_memory<E>
    {

      /*! \brief Test equality of two points.  Nota bene: this method
      ** is abstract-like.
      **
      ** \return True if both points are the same, false otherwise.
      */
      bool operator==(const point& rhs) const
      {
	return this->exact().impl_eq(rhs.exact());
      }

      /*! \brief Test difference of two points.  Nota bene: this method
      ** is concrete (and based on abstract::point::operator==).
      **
      ** \return True if both points are different, false otherwise.
      */
      bool operator!=(const point& rhs) const
      {
	return not this->operator==(rhs);
      }

      typedef oln_dpoint_type(E) dpoint_type;

      // FIXME: doc
      const point operator+(const dpoint_type& dp) const
      {
	return this->exact().impl_plus(dp);
      }

      const dpoint_type operator-(const point& rhs) const
      {
	return this->exact().impl_minus(rhs);
      }

    protected:

      point() {}
    };

  } // end of namespace abstract

} // end of namespace oln



#endif // ! PROTO_OLN_CORE_ABSTRACT_POINT_HH
