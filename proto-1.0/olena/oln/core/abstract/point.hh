// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_POINT_HH
# define OLENA_CORE_ABSTRACT_POINT_HH

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
	return ! this->operator==(rhs);
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



#endif // ! OLENA_CORE_ABSTRACT_POINT_HH
