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
#ifndef OLENA_CORE_ABSTRACT_DPOINT_HH
# define OLENA_CORE_ABSTRACT_DPOINT_HH

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
    struct point; // fwd declaration



    /*! \class Abstract dpoint.
    **
    ** This class provides an interface.
    ** \see oln::dpoint
    */
    template<class Exact>
    struct dpoint : public mlc_hierarchy::any<Exact>
    {
    public:

      /// Give the value of the nth coordinate of the dpoint.
      coord
      nth(const unsigned			dim) const
      {
	mlc_dispatch(nth)(dim);
      }

      /// Return a reference to the nth coordinate of the dpoint.
      coord&
      nth(const unsigned			dim)
      {
	mlc_dispatch(nth)(dim);
      }

      /// Add a delta dpoint \a dp to the current dpoint
      Exact &
      operator+=(const Exact	&dp)
      {
	mlc_dispatch(op_plus_assign)(dp);
      }


      /// Subtract a delta dpoint \a dp from the current dpoint.
      Exact&
      operator-=(const Exact	&dp)
      {
	mlc_dispatch(op_minus_assign)(dp);
      }

      /// Subtract a dpoint \a p from the current dpoint.
      Exact
      operator-(const Exact			&p) const
      {
	mlc_dispatch(op_minus)(p);
      }


      /// Addition of a delta dpoint \a dp and the current dpoint.
      Exact
      operator+(const Exact	&dp) const
      {
	mlc_dispatch(op_plus)(dp);
      }
      /// Opposite of the current dpoint coordinates
      Exact
      operator-() const
      {
	mlc_dispatch(op_minus)();
      }

      /// Dpoints equal.
      bool
      operator==(const Exact			&p) const
      {
	mlc_dispatch(op_equal_p)(p);
      }


      /// Dpoints not equal.
      bool
      operator!=(const Exact			&p) const
      {
	mlc_dispatch(op_not_equal_p)(p);
      }

      static std::string
      name()
      {
	return std::string("dpoint<") + Exact::name() + ">";
      }

    protected:

      dpoint()
      {}
    };


    template<class Exact>
    inline std::ostream&
    operator<<(std::ostream& o, const dpoint<Exact> &p)
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



    /*! \class Abstract dpoint with dimension.
    **
    ** This class provides an implementation.
    ** \see oln::dpoint
    */
    template<class Exact>
    struct dpoint_nd : public dpoint<Exact>
    {
    public:

      /// Give the value of the nth coordinate of the dpoint.
      coord
      nth_impl(const unsigned			dim) const;

      /// Return a reference to the nth coordinate of the dpoint.
      coord&
      nth_impl(const unsigned			dim);

      /// Add a delta dpoint \a dp to the current dpoint
      Exact &
      op_plus_assign_impl(const Exact& dp);


      /// Subtract a delta dpoint \a dp from the current dpoint.
      Exact&
      op_minus_assign_impl(const Exact	&dp);

      /// Subtract a dpoint \a p from the current dpoint.
      Exact
      op_minus_impl(const Exact			&p) const;


      /// Addition of a delta dpoint \a dp and the current dpoint.
      Exact
      op_plus_impl(const Exact	&dp) const;


      /// Opposite of the current dpoint coordinates
      Exact
      op_minus_impl() const;


      /// Dpoints equal.
      bool
      op_equal_impl(const Exact			&p) const;


      /// Dpoints not equal.
      bool
      op_not_equal_impl(const Exact			&p) const;



      static std::string
      name()
      {
	return std::string("dpoint_nd<") + Exact::name() + ">";
      }

    protected:

      dpoint_nd()
      {}
      coord coord_[oln_dim_val(Exact)];
    };

  } // end of abstract




  namespace internal
  {

    /*! \class default_less< abstract::dpoint<Exact> >
    **
    ** The specialized version for abstract::dpoint.
    */
    template<class Exact>
    struct default_less< abstract::dpoint<Exact> >
    {

      /*! \brief Test if the coordinates of a dpoint l
      ** are not greater than the coordinates of a dpoint r.
      **
      ** \arg l A dpoint.
      **
      ** \arg r Another dpoint.
      **
      ** \return True if the coordinates of l are not greater
      ** than the coordinates of r.
      */

      bool operator()(const abstract::dpoint<Exact>& l,
		      const abstract::dpoint<Exact>& r) const
      {
	for (unsigned i = 0; i < oln_dim_val(Exact); ++i)
	  if (l.nth(i) < r.nth(i))
	    return true;
	  else if (l.nth(i) > r.nth(i))
	    return false;
	return false;
      }
    };
  } // internal
} // end of oln


#include "dpoint.hxx"

#endif // ! OLENA_CORE_ABSTRACT_DPOINT_HH
