// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

# include <mlc/type.hh>
# include <oln/core/coord.hh>
# include <oln/core/macros.hh>

# include <iostream>
# include <sstream>

namespace oln
{

  template<class Exact>
  struct prop; // fwd declaration

  namespace abstract {

    template<class Exact>
    struct dpoint; // fwd declaration



    /*! \class Abstract point.
    **
    ** This class provides an interface.
    ** \see oln::point
    */
    template<class Exact>
    struct point : public mlc_hierarchy::any<Exact>
    {
    public:

      /// Give the value of the nth coordinate of the point.
      coord
      nth(const unsigned			dim) const
      {
	mlc_dispatch(nth)(dim);
      }

      /// Return a reference to the nth coordinate of the point.
      coord&
      nth(const unsigned			dim)
      {
	mlc_dispatch(nth)(dim);
      }

      /// Add a delta point \a dp to the current point
      Exact &
      operator+=(const oln_dpoint_type(Exact)	&dp)
      {
	mlc_dispatch(op_plus_assign)(dp);
      }


      /// Subtract a delta point \a dp from the current point.
      Exact&
      operator-=(const oln_dpoint_type(Exact)	&dp)
      {
	mlc_dispatch(op_minus_assign)(dp);
      }

      /// Subtract a point \a p from the current point.
      oln_dpoint_type(Exact)
      operator-(const Exact			&p) const
      {
	mlc_dispatch(op_minus)(p);
      }


      /// Addition of a delta point \a dp and the current point.
      Exact
      operator+(const oln_dpoint_type(Exact)	&dp) const
      {
	mlc_dispatch(op_plus)(dp);
      }

      /// Subtraction of a delta point \a dp and the current point.
      Exact
      operator-(const oln_dpoint_type(Exact)	&dp) const
      {
	mlc_dispatch(op_minus)(dp);
      }

      /// Opposite of the current point coordinates
      Exact
      operator-() const
      {
	mlc_dispatch(op_minus)();
      }

      /// Points equal.
      bool
      operator==(const Exact			&p) const
      {
	mlc_dispatch(op_equal_p)(p);
      }


      /// Points not equal.
      bool
      operator!=(const Exact			&p) const
      {
	mlc_dispatch(op_not_equal_p)(p);
      }

      static std::string
      name()
      {
	return std::string("point<") + Exact::name() + ">";
      }

    protected:

      point()
      {}
    };


    template<class Exact>
    inline std::ostream&
    operator<<(std::ostream& o, const point<Exact> &p)
    {
      o << "(";
      for (unsigned dim = 0; dim < oln_dim_val(Exact); ++dim)
	{
	  o << p.nth(dim);
	  if (dim + 1 < oln_dim_val(Exact))
	    o << ",";
	}
      o << ")";
      return o;
    }



    /*! \class Abstract point with dimension.
    **
    ** This class provides an implementation.
    ** \see oln::point
    */
    template<class Exact>
    struct point_nd : public point<Exact>
    {
    public:

      /// Give the value of the nth coordinate of the point.
      coord
      nth_impl(const unsigned			dim) const;

      /// Return a reference to the nth coordinate of the point.
      coord&
      nth_impl(const unsigned			dim);

      /// Add a delta point \a dp to the current point
      Exact &
      op_plus_assign_impl(const oln_dpoint_type(Exact)& dp);


      /// Subtract a delta point \a dp from the current point.
      Exact&
      op_minus_assign_impl(const oln_dpoint_type(Exact)	&dp);

      /// Subtract a point \a p from the current point.
      oln_dpoint_type(Exact)
      op_minus_impl(const Exact			&p) const;


      /// Addition of a delta point \a dp and the current point.
      Exact
      op_plus_impl(const oln_dpoint_type(Exact)	&dp) const;


      /// Subtraction of a delta point \a dp and the current point.
      Exact
      op_minus_impl(const oln_dpoint_type(Exact)	&dp) const;


      /// Opposite of the current point coordinates
      Exact
      op_minus_impl() const;


      /// Points equal.
      bool
      op_equal_impl(const Exact			&p) const;


      /// Points not equal.
      bool
      op_not_equal_impl(const Exact			&p) const;



      static std::string
      name()
      {
	return std::string("point_nd<") + Exact::name() + ">";
      }

    protected:

      point_nd()
      {}
      coord coord_[oln_dim_val(Exact)];
    };

  } // end of abstract




//   namespace internal
//   {

//     /*! \class default_less< abstract::point<Exact> >
//     **
//     ** The specialized version for abstract::point.
//     */
//     template<class Exact>
//     struct default_less< abstract::point<Exact> >
//     {

//       /*! \brief Test if the coordinates of a point l
//       ** are not greater than the coordinates of a point r.
//       **
//       ** \arg l A point.
//       **
//       ** \arg r Another point.
//       **
//       ** \return True if the coordinates of l are not greater
//       ** than the coordinates of r.
//       */

//       bool operator()(const abstract::point<Exact>& l,
// 		      const abstract::point<Exact>& r) const
//       {
// 	for (unsigned i = 0; i < oln_dim_val(Exact); ++i)
// 	  if (l.nth(i) < r.nth(i))
// 	    return true;
// 	  else if (l.nth(i) > r.nth(i))
// 	    return false;
// 	return false;
//       }
//     };
//   } // internal
} // end of oln


#include "point.hxx"

#endif // ! OLENA_CORE_ABSTRACT_POINT_HH
