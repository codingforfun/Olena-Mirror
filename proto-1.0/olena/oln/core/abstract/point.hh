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
# include <mlc/if.hh>
# include <mlc/cmp.hh>

# include <oln/core/coord.hh>
# include <oln/core/typedefs.hh>
# include <oln/core/abstract/grid.hh>


// fwd decl
namespace oln {
  struct any_point;
}



# define oln_point_type_from_2(P1, P2) \
typename mlc::if_< mlc::eq< P2, oln::any_point >, P1, P2 >::ret


# define oln_pt_type_of(PointType, Alias) \
mlc_type_of(oln, oln::category::point, PointType, Alias)



/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  namespace category
  {
    struct point;
  }


  template <>
  struct set_default_props < category::point >
  {
    typedef mlc::undefined_type dpoint_type;
    typedef mlc::undefined_type grid_type;
  };


  template <typename P>
  struct get_props < category::point, P >
  {
    typedef oln_pt_type_of(P, dpoint) dpoint_type;
    typedef oln_pt_type_of(P, grid)   grid_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::point, " << mlc_to_string(P) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t dpoint_type = " << mlc_to_string(dpoint_type) << std::endl
	   << "\t grid_type   = " << mlc_to_string(grid_type)   << std::endl
	   << "}" << std::endl;
    }
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
    struct point : public mlc::any<E>
    {

      typedef E exact_type;


      /// Conversion to any_point (implemented in oln/core/any/point.hh).
      operator any_point() const;


      /*! \brief Test equality of two points.  Nota bene: this method
      ** is abstract-like.
      **
      ** \return True if both points are the same, false otherwise.
      */
      bool operator==(const exact_type& rhs) const
      {
	return this->exact().impl_eq(rhs.exact());
      }

//       // FIXME: compiler error (cannot be overloaded)
//       bool operator==(const any_point& rhs) const
//       {
// 	return true;
//       }

      /*! \brief Test difference of two points.  Nota bene: this method
      ** is concrete (and based on abstract::point::operator==).
      **
      ** \return True if both points are different, false otherwise.
      */
      bool operator!=(const exact_type& rhs) const
      {
	return ! this->operator==(rhs);
      }

      // FIXME: doc...

      typedef oln_pt_type_of(E, dpoint) dpoint_type;

      const exact_type operator+(const dpoint_type& dp) const
      {
	return this->exact().impl_plus(dp);
      }

      const dpoint_type operator-(const exact_type& rhs) const
      {
	return this->exact().impl_minus(rhs);
      }

      typedef oln_pt_type_of(E, grid)              grid_type;
      typedef oln_grd_type_of(grid_type, dimvalue) dimvalue_type;
      typedef oln_grd_type_of(grid_type, coord)    coord_type;

      const coord_type nth(unsigned i) const
      {
	precondition(i < dimvalue_type::val);
	return this->exact().impl_nth(i);
      }

      coord_type& nth(unsigned i)
      {
	precondition(i < dimvalue_type::val);
	return this->exact().impl_nth(i);
      }

    protected:

      point() {}

      ~point()
      {
	{ // impl_eq
	  typedef bool (E::*meth)(const exact_type&) const;
	  meth adr = &E::impl_eq;
	  adr = 0;
	}
	{ // impl_plus
	  typedef const exact_type (E::*meth)(const dpoint_type&) const;
	  meth adr = &E::impl_plus;
	  adr = 0;
	}
	{ // impl_minus
	  typedef const dpoint_type (E::*meth)(const exact_type&) const;
	  meth adr = &E::impl_minus;
	  adr = 0;
	}
	{ // impl_nth const
	  typedef const coord_type (E::*meth)(unsigned) const;
	  meth adr = &E::impl_nth;
	  adr = 0;
	}
	{ // impl_nth
	  typedef coord_type& (E::*meth)(unsigned);
	  meth adr = &E::impl_nth;
	  adr = 0;
	}
      }

    };


  } // end of namespace abstract

} // end of namespace oln



#endif // ! OLENA_CORE_ABSTRACT_POINT_HH
