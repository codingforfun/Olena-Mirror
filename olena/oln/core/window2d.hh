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

#ifndef OLENA_CORE_WINDOW2D_HH
# define OLENA_CORE_WINDOW2D_HH

# include <oln/core/point2d.hh>
# include <oln/core/dpoint2d.hh>
# include <oln/core/abstract/windownd.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>
# include <oln/io/readable.hh>
# include <algorithm>

namespace oln {

  class window2d; // forward declaration

  /*!
  ** \brief Traits for window2d.
  */
  template<>
  struct struct_elt_traits<window2d>: public
  struct_elt_traits<abstract::windownd<window2d> >
  {
    enum { dim = 2 }; ///< Dimension.
    typedef point2d point_type; ///< Type of point.
    typedef dpoint2d dpoint_type; ///< Type of dpoint.
    typedef winiter< window2d > iter_type; ///< Type of iterator.
    typedef winneighb< window2d > neighb_type; ///< Type of neighbor.
  };

  /*!
  ** \brief Window 2 dimensions.
  **
  ** A window is a set of points. This class
  ** defines how to deal with. These points have 2 dimension.
  */
  class window2d : public abstract::windownd< window2d >
  {

  public:

    typedef abstract::windownd<window2d > super_type;
    ///< The super type.
    typedef window2d self_type; ///< The self type.

    /*!
    ** \brief The associate image's type of iterator.
    ** \warning Prefer the macros oln_iter_type(Iterable) and
    ** oln_iter_type_(Iterable) (the same without the 'typename' keyword)
    */
    typedef struct_elt_traits< self_type >::iter_type   iter_type;

    typedef struct_elt_traits< self_type >::neighb_type neighb_type;
    typedef struct_elt_traits< self_type >::neighb_type neighb_type;
    ///< Type of neighbor.

    /*!
    ** \brief The associate image's type of dpoint (move point).
    ** \warning Prefer the macros oln_dpoint_type(Pointable) and
    ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
    */
    typedef struct_elt_traits< self_type >::dpoint_type dpoint_type;

    friend class abstract::window_base<abstract::window<window2d>, window2d>;

    /*!
    ** \brief Add a dpoint (move point) to the window.
    ** \arg dp The new point.
    **
    ** Add a new member to the window. This point must be of 2
    ** dimensions.
    */
    window2d&
    add(const dpoint_type& dp)
    {
      return this->exact().add_(dp);
    }

    /*!
    ** \brief Add a point by coordinates to the window.
    ** \arg row The coordinate (row) of the new point.
    ** \arg col The coordinate (col) of the new point.
    **
    ** Add a new member by its coordinates to the window.
    ** The coordinates are the row number and the column number because the
    ** window has 2 dimensions.
    */
    window2d&
    add(coord row, coord col)
    {
      dpoint_type dp(row, col);
      return add(dp);
    }

    /*!
    ** \brief Construct a window of 2 dimensions.
    */
    window2d() : super_type()
    {}

    /*!
    ** \brief Construct a window of 2 dimensions.
    ** \arg size The number of element.
    */
    window2d(unsigned size) : super_type(size)
    {}

    /*!
    ** \brief Construct a window of 2 dimensions from several points.
    ** \arg n The number of element.
    ** \arg crd The coordinates of the elements
    */
    window2d(unsigned n, const coord crd[]) : super_type(n)
    {
      for (unsigned i = 0; i < 2 * n; i += 2)
	add(dpoint_type(crd[i], crd[i+1]));
    }

    // io
    /*!
    ** \todo FIXME: it doesn't seem useful. We may remove it
    */
    window2d(const io::internal::anything& r) : super_type()
    {
      r.assign(*this);
    }

    /*!
    ** \todo FIXME: it doesn't seem useful. We may remove it
    */
    window2d&
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
      return std::string("window2d");
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
      delta_(abs(dp.row()));
      delta_(abs(dp.col()));
      return delta_;
    }

  };


  // std win

  /*!
  ** \brief Create a window (2 dimensions) of 4 elements.
  ** \return The new window.
  */
  inline const window2d&
  win_c4_only()
  {
    static const coord crd[] = { -1,0,  0,-1, 0,1,  1,0 };
    static const window2d win(4, crd);
    return win;
  }

  /*!
  ** \brief Create a window (2 dimensions) of 5 elements.
  ** \return The new window.
  **
  ** It's the same than win_c4_only() plus the 0,0 point.
  */
  inline const window2d&
  win_c4p()
  {
    static const coord crd[] = { -1,0,  0,-1, 0,0, 0,1,  1,0 };
    static const window2d win(5, crd);
    return win;
  }

  /*!
  ** \brief Create a window (2 dimensions) of 8 elements.
  ** \return The new window.
  */
  inline const window2d&
  win_c8_only()
  {
    static const coord crd[] = { -1,-1, -1,0, -1,1,  0,-1, 0,1,  1,-1,  1,0,  1,1 };
    static const window2d win(8, crd);
    return win;
  }

  /*!
  ** \brief Create a window (2 dimensions) of 9 elements.
  ** \return The new window.
  **
  ** It's the same than win_c8_only more the 0,0 point.
  */
  inline const window2d&
  win_c8p()
  {
    static const coord crd[] = { -1,-1, -1,0, -1,1,  0,-1, 0,0, 0,1,  1,-1,  1,0,  1,1 };
    static const window2d win(9, crd);
    return win;
  }

  // mk_win's

  /*!
  ** \brief Create a rectangular window (2 dimensions).
  ** \arg nrows Number of row.
  ** \arg ncols Number of column.
  ** \return The new window (2d).
  ** \pre nrows >= 3.
  ** \pre nrows % 2 == 1.
  ** \pre ncols >= 3.
  ** \pre ncols % 2 == 1.
  */
  inline window2d
  mk_win_rectangle(unsigned nrows, unsigned ncols)
  {
    precondition(nrows >= 3 && (nrows % 2) == 1);
    precondition(ncols >= 3 && (ncols % 2) == 1);
    window2d win(nrows * ncols);
    int half_nrows = nrows / 2, half_ncols = ncols / 2;
    for (coord row = - half_nrows; row <= half_nrows; ++row)
      for (coord col = - half_ncols; col <= half_ncols; ++col)
	win.add(row, col);
    return win;
  }

  /*!
  ** \brief Create an ellipse window (2 dimensions).
  ** \return The new window.
  **
  ** The ellipse formula is :
  ** \f$$\frac{x^2}{xradius^2} + \frac{y^2}{yradius^2} = 1$\f$
  */
  inline window2d
  mk_win_ellipse(float yradius, float xradius)
  {
    precondition(yradius > 0);
    precondition(xradius > 0);

    window2d win;
    coord ymax = (coord)roundf(yradius);
    float yr2 = yradius * yradius;
    for (coord y = -ymax; y <= ymax; ++y)
      {
	/*
	      x^2         y^2
	   --------- + --------- = 1
	   xradius^2   yradius^2

	*/
	float v = 1 - y * y / yr2;
	if (v < 0) v = 0;	// Can happen because ymax has been rounded.
	coord xmax = (coord)roundf(xradius * sqrtf(v));
	for (coord x = -xmax; x <= xmax; ++x)
	  win.add(y, x);
      }
    return win;
  }

  /*!
  ** \brief Create a square window (2 dimensions).
  ** \arg width Number of column and row.
  ** \return The new window (2d).
  */
  inline window2d
  mk_win_square(unsigned width)
  {
    return mk_win_rectangle(width, width);
  }

  /*!
  ** \brief Create a disc window (2 dimensions).
  ** \arg radius  Radius of the disc.
  ** \return The new window (2d).
  */
  inline window2d
  mk_win_disc(float radius)
  {
    return mk_win_ellipse(radius, radius);
  }

} // end of oln

#endif // OLENA_CORE_WINDOW2D_HH
