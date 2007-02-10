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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_CORE_NEIGHBORHOOD1D_HH
# define OLENA_CORE_NEIGHBORHOOD1D_HH

# include <oln/core/abstract/neighborhoodnd.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>
# include <oln/core/window1d.hh>
# include <algorithm>

namespace oln {

  class neighborhood1d; // forward declaration

  /*!
  ** \brief Traits for neighborhood1d.
  */
  template<>
  struct struct_elt_traits<neighborhood1d>: public
  struct_elt_traits<abstract::neighborhoodnd<neighborhood1d> >
  {
    enum { dim = 1 }; ///< Dimension.
    typedef point1d point_type; ///< Type of point.
    typedef dpoint1d dpoint_type; ///< Type of dpoint (move).
    typedef winiter< neighborhood1d > iter_type; ///< Type of iterator.
    typedef winneighb< neighborhood1d > neighb_type; ///< Type of neighbor.
    typedef window1d win_type; ///< Type of window.
  };

  /*!
  ** \brief Neighborhood 1 dimension.
  **
  ** It looks like structuring elements but here, when
  ** you add an element, you add its opposite.
  ** Points (dpoint) have 1 dimension.
  **
  */
  class neighborhood1d :
    public abstract::neighborhoodnd< neighborhood1d >
  {
  public:

    typedef abstract::neighborhoodnd< neighborhood1d > super_type;
    ///< Super type.
    typedef neighborhood1d self_type; ///< Self type.

    /*!
    ** \brief The associate image's type of iterator (move point).
    ** \warning Prefer the macros oln_iter_type(Pointable) and
    ** oln_iter_type_(Pointable) (the same without the 'typename' keyword)
    */
    typedef struct_elt_traits< self_type >::iter_type   iter_type;
    typedef struct_elt_traits< self_type >::neighb_type	neighb_type;

    /*!
    ** \brief The associate image's type of dpoint (move point).
    ** \warning Prefer the macros oln_dpoint_type(Pointable) and
    ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
    */
    typedef struct_elt_traits< self_type >::dpoint_type dpoint_type;

    friend class abstract::window_base<abstract::neighborhood<neighborhood1d>, neighborhood1d>;

    /*!
    ** \brief Add a dpoint (move point) to the neighborhood.
    ** \arg dp The new point.
    **
    ** Add a new member to the neighborhood. This point must be of 1
    ** dimension.
    */
    neighborhood1d&
    add(const dpoint_type& dp)
    {
      this->exact().add_(dp);
      return this->exact().add_(-dp);
    }

    /*!
    ** \brief Add a point by coordinates to the neighborhood.
    ** \arg col The coordinate of the new point (1 dimension).
    **
    ** Add a new member by its coordinates to the neighborhood.
    ** The coordinates are only the column number because the neighborhood has
    ** 1 dimension.
    */
    neighborhood1d&
    add(coord col)
    {
      return this->add(dpoint_type(col));
    }

    /*!
    ** \brief Construct a neighborhood of 1 dimension.
    */
    neighborhood1d() : super_type()
    {}

    /*!
    ** \brief Construct a neighborhood of 1 dimension.
    ** \arg size Reserve 'size' elements for the neighborhood.
    */
    neighborhood1d(unsigned size) : super_type(size)
    {}

    /*!
    ** \brief Construct a neighborhood of 1 dimension.
    ** \arg n Add 'n' elements to the neighborhood.
    ** \arg crd Coordinates of the 'n' elements.
    */
    neighborhood1d(unsigned n, const coord crd[]) : super_type()
    {
      for (unsigned i = 0; i < n; ++i)
	add(dpoint_type(crd[i]));
    }

    /// Return the name of the type.
    static std::string
    name()
    {
      return std::string("neighborhood1d");
    }

  protected:

    /*!
    ** \brief Update delta.
    ** \arg dp a move point.
    ** \return Delta.
    **
    ** If the point is the biggest element of the neighborhood,
    ** then this point is assigned to delta.
    */
    coord
    delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.col()));
      return delta_;
    }

  };


  // standard neighborhood

  /*!
  ** \brief Create a neighborhood (1 dimension) with 1 element : 1.
  ** \return The new neighborhood.
  */
  inline const neighborhood1d&
  neighb_c2()
  {
    static const coord crd[] = {  1 };
    static const neighborhood1d neighb(1, crd);
    return neighb;
  }

  /*!
  ** \brief Create a neighborhood (1 dimension).
  ** \arg width The width.
  ** \return The new neighborhood.
  ** \pre width >= 3.
  ** \pre width % 2 == 1.
  **
  ** Add elements of coordinates 1, ..., width / 2.
  */
  inline neighborhood1d
  mk_neighb_segment(unsigned width)
  {
    precondition(width>= 3 && (width % 2) == 1);
    neighborhood1d neighb(width);
    int half_ncols = width / 2;
    for (coord col = 1; col <= half_ncols; ++col)
	neighb.add(col);
    return neighb;
  }

  /*!
  ** \brief Convert a neighborhood (1 dimension) to a window (1 dimension).
  ** \arg n The neighborhood to convert.
  ** \return The new window.
  */
  inline window1d
  mk_win_from_neighb(const neighborhood1d& n)
  {
    window1d win(n.card());
    for (unsigned i = 0; i < n.card(); ++i)
      win.add(n.dp(i));
    return win;
  }


} // end of oln

#endif // OLENA_CORE_NEIGHBORHOOD1D_HH
