#ifndef OLENA_CORE_VALUE_BOX_HH
# define OLENA_CORE_VALUE_BOX_HH

# include <ostream>

// for mlc::box<T>:

# include <mlc/box.hh>

# include <oln/core/macros.hh>

// the proper macro:
// for use in 'abstract::image::op[](point)'

# define oln_value_box_type(E) typename oln::value_box<E>




/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {



  // fwd decl
  namespace abstract {
    template <typename I> class image;
  }



  /*! \class value_box<I>
  **
  ** This class mimics a value returned by an image via calls like
  ** 'ima[p]'.  It actually represents a box that hides the value from
  ** the client; this box somewhat encloses the value.  This box aims
  ** at controlling whether the value is read or written.  It then
  ** calls the get and set methods provided in abstract::readonly_image
  ** and abstract::readwrite_image.
  **
  ** Parameter I is the exact type of the image from whom the value
  ** comes (the client has thus called abstract::image<I>::operator[]).
  **
  ** When the value comes from a 'const image', the specialization
  ** 'value_box<const I>' is used.
  **
  ** FIXME: a lot more to say...
  */

  template <typename I>
  class value_box
  {

  public:


    /*! \brief op=
    ** FIXME:...
    ** \return (*this)
    */

    template <typename V>
    value_box& operator=(const V& value)
    {
      ima_->set(p_, value);
      return *this;
    }


    /*! \brief op=
    ** FIXME:...
    **
    ** IDEA: why not add a set method...
    **
    ** \return (*this)
    */

    template <typename II>
    value_box& operator=(const value_box<II>& rhs)
    {
      ima_->set(p_, rhs); // automatic conversion from rhs to oln_value_type(I)
      return *this;
    }


    /*! \brief Set
    **
    ** NEW code
    **
    ** FIXME:...
    */

    template <typename A, typename V>
    value_box& set(void (I::*method)(A), const V& value)
    {
      ima_->set(p_, method, value);
      return *this;
    }




    /*! \brief Conversion op
    ** FIXME:...
    ** \return converted
    */

    template <typename V>
    operator const V() const
    {
      const V value = ima_->get(p_);
      return value;
    }

    operator const oln_value_type(I)() const
    {
      return ima_->get(p_);
    }


    /*! \brief hook
    ** FIXME:...
    **
    ** IDEA: provide op-> instead (?)
    **
    ** FIXME: keep in mind that there will be pbs with constness knowledge propagation
    ** ex: input[p]->red = 0
    **
    ** \return a value (const, temp)
    */

    const oln_value_type(I) value() const
    {
      return ima_->get(p_);
    }


  private:

    /*-------*
     ! ctors !
     *-------*/

    friend class abstract::image<I>;

    /// Ctor (restricted access).

    value_box(I& ima, const oln_point_type(I)& p) :
      ima_(ima),
      p_(p)
    {
    }

    /// Cpy ctor (restricted access).

    value_box(const value_box& rhs) :
      ima_(rhs.ima_),
      p_(rhs.p_)
    {
    }

    /*------------*
     ! attributes !
     *------------*/

    mlc::box<I>       ima_;
    oln_point_type(I) p_;

  };





  /*! \class value_box<const I>
  **
  ** Specialization of 'value_box<I>'.
  **
  ** FIXME: some more to say...
  **
  ** \see value_box<I>
  */

  template <typename I>
  struct value_box <const I>
  {

  public:


    // FIXME: no operator= was provided here so 'self = self' was possible...
    // FIXME: below, a single decl has been added; to be tested...

    template <typename V>
    void operator=(const V&) const;


    /*! \brief Conversion op
    ** FIXME:...
    ** \return converted
    */

    template <typename V>
    operator const V() const
    {
      const V value = ima_->get(p_);
      return value;
    }

    operator const oln_value_type(I)() const
    {
      return ima_->get(p_);
    }


    /*! \brief hook
    ** FIXME:...
    **
    ** IDEA: provide op-> instead (?)
    **
    ** FIXME: keep in mind that there will be pbs with constness knowledge propagation
    ** ex: input[p]->red = 0
    **
    ** \return a value (const, temp)
    */

    const oln_value_type(I) value() const
    {
      return ima_->get(p_);
    }

    // IDEA: provide op->

  private:

    /*-------*
     ! ctors !
     *-------*/

    friend class abstract::image<I>;

    /// Ctor (restricted access).

    value_box(const I& ima, const oln_point_type(I)& p) :
      ima_(ima),
      p_(p)
    {
    }

    /// Cpy ctor (restricted access).

    value_box(const value_box& rhs) :
      ima_(rhs.ima_),
      p_(rhs.p_)
    {
    }

    /*------------*
     ! attributes !
     *------------*/

    mlc::box<const I> ima_;
    oln_point_type(I) p_;

  };


} // end of namespace oln



template <typename I>
std::ostream& operator<<(std::ostream& ostr, const oln::value_box<I>& vb)
{
  return ostr << vb.value();
}



#endif // ! OLENA_CORE_VALUE_BOX_HH
