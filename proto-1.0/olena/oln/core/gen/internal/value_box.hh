// Copyright (C) 2005 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_CORE_GEN_INTERNAL_VALUE_BOX_HH
# define OLENA_CORE_GEN_INTERNAL_VALUE_BOX_HH

# include <iostream>
# include <mlc/cmp.hh>
# include <oln/core/abstract/image.hh>




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

    /// typedefs

    typedef oln_type_of(I, value) value_type;
    typedef oln_type_of(I, point) point_type;


    /// Operator == (rhs is a value_box).
    template <typename II>
    bool operator==(const value_box<II>& rhs) const
    {
      return this->value() == rhs.value();
    }

    /// Operator == (rhs is a value).
    template <typename V>
    bool operator==(const V& rhs) const
    {
      return this->value() == rhs;
    }

    /// Operator !=.
    template <typename V>
    bool operator!=(const V& rhs) const
    {
      return ! this->operator==(rhs);
    }

    /// Operator < (rhs is a value_box).
    template <typename II>
    bool operator<(const value_box<II>& rhs) const
    {
      return this->value() < rhs.value();
    }

    /// Operator < (rhs is a value).
    template <typename V>
    bool operator<(const V& rhs) const
    {
      return this->value() < rhs;
    }


    /*! \brief op=
    ** FIXME:...
    ** \return (*this)
    */

    template <typename V>
    value_box& operator=(const V& value)
    {
      this->ima_->set(this->p_, value);
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
      ima_->set(p_, rhs.value()); // automatic conversion from rhs to value_type
      return *this;
    }

    value_box& operator=(const value_box<I>& rhs)
    {
      ima_->set(p_, rhs.value()); // automatic conversion from rhs to value_type
      return *this;
    }


    /*! \brief Set
    **
    ** NEW code
    **
    ** FIXME:...
    */

//     template <typename A, typename V>
//     value_box& set(void (I::*method)(A), const V& value)
//     {
//       ima_->set(p_, method, value);
//       return *this;
//     }




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

    operator const value_type() const
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

    const value_type value() const
    {
      return ima_->get(p_);
    }

    template <typename R, typename T>
    R call(R (T::*method)() const) const
    {
      return ima_->call(p_, method);
    }

    template <typename T, typename A, typename V>
    void call(void (T::*method)(A),
	      V arg)
    {
      ima_->call(p_, method, arg);
    }

    /// Cpy ctor.

    value_box(const value_box& rhs) :
      ima_(rhs.ima_),
      p_(rhs.p_)
    {
    }


  private:

    /*-------*
     ! ctors !
     *-------*/

    friend class abstract::image<I>;

    /// Ctor (restricted access).

    value_box(abstract::image<I>& ima, const point_type& p) :
      ima_(&ima.exact()),
      p_(p)
    {
    }

    /*------------*
     ! attributes !
     *------------*/

    I* ima_;
    point_type p_;

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

    /// typedefs

    typedef oln_type_of(I, value) value_type;
    typedef oln_type_of(I, point) point_type;


    /// Operator == (rhs is a value_box).
    template <typename II>
    bool operator==(const value_box<II>& rhs) const
    {
      return this->value() == rhs.value();
    }

    /// Operator == (rhs is a value).
    template <typename V>
    bool operator==(const V& rhs) const
    {
      return this->value() == rhs;
    }

    /// Operator !=.
    template <typename V>
    bool operator!=(const V& rhs) const
    {
      return ! this->operator==(rhs);
    }

    /// Operator < (rhs is a value_box).
    template <typename II>
    bool operator<(const value_box<II>& rhs) const
    {
      return this->value() < rhs.value();
    }

    /// Operator < (rhs is a value).
    template <typename V>
    bool operator<(const V& rhs) const
    {
      return this->value() < rhs;
    }


    /*! \brief Assignment (op=) is declared but undefined.
    */

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

    operator const value_type() const
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

    const value_type value() const
    {
      return ima_->get(p_);
    }

    // IDEA: provide op->

    template <typename R, typename T>
    R call(R (T::*method)() const) const
    {
      return ima_->call(p_, method);
    }


    /// Cpy ctor.

    value_box(const value_box& rhs) :
      ima_(rhs.ima_),
      p_(rhs.p_)
    {
    }


  private:

    /*-------*
     ! ctors !
     *-------*/

    friend class abstract::image<I>;

    /// Ctor (restricted access).

    value_box(const abstract::image<I>& ima, const point_type& p) :
      ima_(&ima.exact()),
      p_(p)
    {
    }

    /*------------*
     ! attributes !
     *------------*/

    const I* ima_;
    point_type p_;

  };


  template <typename I, typename II>
  oln_type_of(I, value) min(const value_box<I>& lhs,
			    const value_box<II>& rhs)
  {
    // FIXME: uncomment?
//     mlc::eq< oln_type_of(I, value), oln_type_of(II, value) >::ensure();
    return lhs.value() < rhs.value() ? lhs.value() : rhs.value();
  }

  template <typename I, typename II>
  oln_type_of(I, value) max(const value_box<I>& lhs,
			    const value_box<II>& rhs)
  {
    // FIXME: uncomment?
//     mlc::eq< oln_type_of(I, value), oln_type_of(II, value) >::ensure();
    return lhs.value() < rhs.value() ? rhs.value() : lhs.value();
  }


} // end of namespace oln


/// Operator <<.
template <typename I>
std::ostream& operator<<(std::ostream& ostr, const oln::value_box<I>& vb)
{
  return ostr << vb.value();
}


#endif // ! OLENA_CORE_GEN_INTERNAL_VALUE_BOX_HH
