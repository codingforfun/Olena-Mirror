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

#ifndef OLENA_CORE_NEIGHBORHOOD2D_HH
# define OLENA_CORE_NEIGHBORHOOD2D_HH

# include <oln/core/abstract/neighborhoodnd.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>
# include <oln/core/window2d.hh>
# include <oln/io/readable.hh>
# include <algorithm>

namespace oln {

  class neighborhood2d; // forward declaration

  /*!
  ** \brief Traits for neighborhood2d.
  */
  template<>
  struct struct_elt_traits<neighborhood2d>: public
  struct_elt_traits<abstract::neighborhoodnd<neighborhood2d> >
  {
    enum { dim = 2 }; ///< Dimension.
    typedef point2d point_type; ///< Type of point.
    typedef dpoint2d dpoint_type; ///< Type of dpoint (move).
    typedef winiter< neighborhood2d > iter_type; ///< Type of iterator.
    typedef winneighb< neighborhood2d > neighb_type; ///< Type of neighbor.
    typedef window2d win_type; ///< Type of window.
  };

  /*!
  ** \brief Neighborhood 2 dimensions.
  **
  ** It looks like structuring elements but here, when
  ** you add an element, you add his opposite.
  ** Points have 2 dimensions.
  **
  */
  class neighborhood2d :
    public abstract::neighborhoodnd< neighborhood2d >
  {
  public:

    typedef abstract::neighborhoodnd< neighborhood2d > super_type;
    ///< Super type.
    typedef neighborhood2d self_type; ///< Self type.

    /*!
    ** \brief The associate image's type of iterator.
    ** \warning Prefer the macros oln_iter_type(Iterable) and
    ** oln_iter_type_(Iterable) (the same without the 'typename' keyword)
    */
    typedef struct_elt_traits< self_type >::iter_type   iter_type;

    typedef struct_elt_traits< self_type >::neighb_type	neighb_type;

    /*!
    ** \brief The associate image's type of dpoint (move point).
    ** \warning Prefer the macros oln_dpoint_type(Pointable) and
    ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
    */
    typedef struct_elt_traits< self_type >::dpoint_type dpoint_type;

    friend class abstract::window_base<abstract::neighborhood<neighborhood2d>, neighborhood2d>;

    /*!
    ** \brief Add a dpoint (move point) to the neighborhood.
    ** \arg dp The new point.
    **
    ** Add a new member to the neighborhood. This point must be of 2
    ** dimensions.
    */
    neighborhood2d&
    add(const dpoint_type& dp)
    {
      this->exact().add_(dp);
      return this->exact().add_(-dp);
    }

    /*!
    ** \brief Add a point by coordinates to the neighborhood.
    ** \arg row The coordinates of the new point.
    ** \arg col The coordinates of the new point.
    **
    ** Add a new member by its coordinates to the neighborhood.
    ** The coordinates have 2 dimensions.
    */
    neighborhood2d&
    add(coord row, coord col)
    {
      return this->add(dpoint_type(row, col));
    }

    /*!
    ** \brief Construct a neighborhood of 2 dimensions.
    */
    neighborhood2d() : super_type()
    {}

    /*!
    ** \brief Construct a neighborhood of 2 dimensions.
    ** \arg size Reserve 'size' elements for the neighborhood.
    */
    neighborhood2d(unsigned size) : super_type(size)
    {}

    /*!
    ** \brief Construct a neighborhood of 2 dimensions.
    ** \arg n Add 'n' elements to the neighborhood.
    ** \arg crd Coordinates of the 'n' elements.
    */
    neighborhood2d(unsigned n, const coord crd[]) : super_type(n)
    {
      for (unsigned i = 0; i < 2 * n; i += 2)
	add(dpoint_type(crd[i], crd[i+1]));
    }

    // io
    /*!
    ** \todo FIXME: it doesn't seem useful. We may remove it
    */
    neighborhood2d(const io::internal::anything& r) : super_type()
    {
      r.assign(*this);
    }

    /*!
    ** \todo FIXME: it doesn't seem useful. We may remove it
    */
    neighborhood2d&
    operator=(const io::internal::anything& r)
    {
      return r.assign(*this);
    }

    /*!
    ** \brief Return his type in a string.
    ** \return The type in a string.
    **
    ** Very useful to debug.
    */
    static std::string
    name()
    {
      return std::string("neighborhood2d");
    }

  protected:

    /*!
    ** \brief Update delta.
    ** \arg dp a deplacement point.
    ** \return Delta.
    **
    ** If the point is the biggest element of the neighborhood,
    ** then this point is assigned to delta.
    */
    coord
    delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.row()));
      delta_(abs(dp.col()));
      return delta_;
    }

  };

  // std neighbs

  /*!
  ** \brief Create a neighborhood (2 dimension) with 0,1, 1,0.
  ** \return The new neighborhood.
  */
  inline const neighborhood2d&
  neighb_c4()
  {
    static const coord crd[] = { 0,1,  1,0 };
    static const neighborhood2d neighb(2, crd);
    return neighb;
  }


  /*!
  ** \brief Create a neighborhood (2 dimension) with 4 coordinates:
  ** 0,1, 1,1, 1,0, 1,-1.
  ** \return The new neighborhood.
  */
  inline const neighborhood2d&
  neighb_c8()
  {
    static const coord crd[] = { 0,1,  1,1,  1,0,  1,-1 };
    static const neighborhood2d neighb(4, crd);
    return neighb;
  }

  // mk_neighb's

  /*!
  ** \brief Create a rectangular neighborhood (2 dimensions).
  ** \arg nrows Number of row.
  ** \arg ncols Number of column.
  ** \return The new neighborhood (2d).
  ** \pre nrows >= 3.
  ** \pre nrows % 2 == 1.
  ** \pre ncols >= 3.
  ** \pre ncols % 2 == 1.
  */
  inline neighborhood2d
  mk_neighb_rectangle(unsigned nrows, unsigned ncols)
  {
    precondition(nrows >= 3 && (nrows % 2) == 1);
    precondition(ncols >= 3 && (ncols % 2) == 1);
    neighborhood2d neighb(nrows * ncols);
    int half_nrows = nrows / 2, half_ncols = ncols / 2;
    for (coord row = - half_nrows; row <= half_nrows; ++row)
      for (coord col = (row <= 0)? 1 : 0; col <= half_ncols; ++col)
	  neighb.add(row, col);
    return neighb;
  }

  /*!
  ** \brief Create a square neighborhood (2 dimensions).
  ** \arg width Number of colunm and row.
  ** \return The new neighborhood (2d).
  */
  inline neighborhood2d
  mk_neighb_square(unsigned width)
  {
    return mk_neighb_rectangle(width, width);
  }

  /*!
  ** \brief Convert a window (2 dimensions) to a neighborhood (2 dimensions).
  ** \arg n The neighborhood to convert.
  ** \return The new window.
  */
  inline window2d
  mk_win_from_neighb(const neighborhood2d& n)
  {
    window2d win(n.card());
    for (unsigned i = 0; i < n.card(); ++i)
      win.add(n.dp(i));
    return win;
  }

} // end of oln

#endif // OLENA_CORE_NEIGHBORHOOD2D_HH
