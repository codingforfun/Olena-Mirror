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

#ifndef OLENA_CORE_WINDOW1D_HH
# define OLENA_CORE_WINDOW1D_HH

# include <oln/core/point1d.hh>
# include <oln/core/dpoint1d.hh>
# include <oln/core/abstract/windownd.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>
# include <algorithm>

namespace oln {

  class window1d; // forward declaration

  /*!
  ** \brief Traits for window1d.
  */
  template<>
  struct struct_elt_traits<window1d>: public
  struct_elt_traits<abstract::windownd<window1d> >
  {
    enum { dim = 1 }; ///< Dimension.
    typedef point1d point_type; ///< Type of point.
    typedef dpoint1d dpoint_type; ///< Type of dpoint.
    typedef winiter< window1d > iter_type; ///< Type of iterator.
    typedef winneighb< window1d > neighb_type; ///< Type of neighbor.
  };


  /*!
  ** \brief Window 1 dimension.
  **
  ** A window is a set of points. This class
  ** defines how to deal with. These points have 1 dimension.
  */
  class window1d : public abstract::windownd< window1d >
  {

  public:

    typedef abstract::windownd< window1d > super_type;
    ///< The super type.
    typedef window1d self_type; ///< The self type.

    /*!
    ** \brief The associate image's type of iterator.
    ** \warning Prefer the macros oln_iter_type(Iterable) and
    ** oln_iter_type_(Iterable) (the same without the 'typename' keyword)
    */
    typedef struct_elt_traits< self_type >::iter_type   iter_type;

    typedef struct_elt_traits< self_type >::neighb_type neighb_type;
    ///< Type of neighbor.

    /*!
    ** \brief The associate image's type of dpoint (move point).
    ** \warning Prefer the macros oln_dpoint_type(Pointable) and
    ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
    */
    typedef struct_elt_traits< self_type >::dpoint_type dpoint_type;

    friend class abstract::window_base<abstract::window<window1d>, window1d>;

    /*!
    ** \brief Add a dpoint (move point) to the window.
    ** \arg dp The new point.
    **
    ** Add a new member to the neighborhood. This point must be of 1
    ** dimension.
    */
    window1d&
    add(const dpoint_type& dp)
    {
      return this->exact().add_(dp);
    }

    /*!
    ** \brief Add a point by coordinates to the window.
    ** \arg col The coordinate of the new point (1 dimension).
    **
    ** Add a new member by its coordinates to the window.
    ** The coordinates are only the column number because the window is
    ** of 1 dimension.
    */
    window1d&
    add(coord col)
    {
      return this->add(dpoint_type(col));
    }

    /*!
    ** \brief Construct a window of 1 dimension.
    */
    window1d() : super_type()
    {}

    /*!
    ** \brief Construct a window of 1 dimension.
    ** \arg size The number of element.
    */
    window1d(unsigned size) : super_type(size)
    {}

    /*!
    ** \brief Construct a window of 1 dimension from several points.
    ** \arg n The number of element.
    ** \arg crd The coordinates of the elements
    */
    window1d(unsigned n, const coord crd[]) : super_type(n)
    {
      for (unsigned i = 0; i < n; ++i)
	add(dpoint_type(crd[i]));
    }

    /*!
    ** \brief Return its type in a string.
    ** \return The type in a string.
    **
    ** Very useful to debug.
    */
    static std::string
    name()
    {
      return std::string("window1d");
    }

  protected:

    /*!
    ** \brief Update delta.
    ** \arg dp a move point.
    ** \return Delta.
    **
    ** If the point is the biggest element of the window.
    ** then this point is assigned to delta.
    */
    coord
    delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.col()));
      return delta_;
    }

  };

  // std win

  /*!
  ** \brief Create a window (1 dimension) of 2 elements (-1, 1).
  ** \return The new neighborhood.
  */
  inline const window1d&
  win_c2_only()
  {
    static const coord crd[] = { -1, 1 };
    static const window1d win(2, crd);
    return win;
  }

  /*!
  ** \brief Create a window (1 dimension) of 3 elements (-1, 0, 1).
  ** \return The new neighborhood.
  */
  inline const window1d&
  win_c2p()
  {
    static const coord crd[] = { -1, 0, 1 };
    static const window1d win(3, crd);
    return win;
  }

  /*!
  ** \brief Create a window (1 dimension) with width elements :
  ** -width / 2, ..., 1, 2, ...,  width / 2
  ** \arg width The width.
  ** \return The new neighborhood.
  ** \pre width >= 3.
  ** \pre width % 2 == 1.
  */
  inline window1d
  mk_win_segment(unsigned width)
  {
    precondition(width>= 3 && (width % 2) == 1);
    window1d win(width);
    int half_ncols = width / 2;
    for (coord col = - half_ncols; col <= half_ncols; ++col)
      win.add(col);
    return win;
  }

} // end of oln

#endif // OLENA_CORE_WINDOW1D_HH
