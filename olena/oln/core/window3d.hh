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

#ifndef OLENA_CORE_WINDOW3D_HH
# define OLENA_CORE_WINDOW3D_HH

# include <oln/core/point3d.hh>
# include <oln/core/dpoint3d.hh>
# include <oln/core/abstract/windownd.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>

# include <algorithm>

namespace oln {

  class window3d; // forward declaration

  /*!
  ** \brief Traits for window3d.
  */
  template<>
  struct struct_elt_traits<window3d>: public
  struct_elt_traits<abstract::windownd<window3d> >
  {
    enum { dim = 3 }; ///< Dimension.
    typedef point3d point_type; ///< Type of point.
    typedef dpoint3d dpoint_type; ///< Type of dpoint.
    typedef winiter< window3d > iter_type; ///< Type of iterator.
    typedef winneighb< window3d > neighb_type; ///< Type of neighbor.
  };

  /*!
  ** \brief Window 3 dimensions.
  **
  ** A window is a set of points. This class
  ** defines how to deal with. These points have 3 dimensions.
  */
  class window3d : public abstract::windownd< window3d >
  {

  public:

    typedef abstract::windownd< window3d > super_type;
    ///< The super type.
    typedef window3d self_type; ///< The self type.

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

    friend class abstract::window_base<abstract::window<window3d>, window3d>;

    /*!
    ** \brief Add a dpoint (move point) to the window.
    ** \arg dp The new point.
    **
    ** Add a new member to the window. This point must be of 3
    ** dimensions.
    */
    window3d& add(const dpoint_type& dp)
    {
      return this->exact().add_(dp);
    }

    /*!
    ** \brief Add a point by coordinates to the window.
    ** \arg slice The coordinate (slice) of the new point.
    ** \arg row The coordinate (row) of the new point.
    ** \arg col The coordinate (col) of the new point.
    **
    ** Add a new member by its coordinates to the window.
    ** The coordinates are only the column number, row number and column
    ** number because the window has 3 dimensions.
    */
    window3d& add(coord slice, coord row, coord col)
    {
      return this->add(dpoint_type(slice, row, col));
    }

    /*!
    ** \brief Construct a window of 3 dimensions.
    */
    window3d() : super_type()
    {}

    /*!
    ** \brief Construct a window of 3 dimensions.
    ** \arg size The number of element.
    */
    window3d(unsigned size) : super_type(size)
    {}

    /*!
    ** \brief Construct a window of 3 dimensions from several points.
    ** \arg n The number of element.
    ** \arg crd The coordinates of the elements
    */
    window3d(unsigned n, const coord crd[]) : super_type(n)
    {
      for (unsigned i = 0; i < 3 * n; i += 3)
	add(dpoint_type(crd[i], crd[i+1], crd[i+2]));
    }

    /// Return the name of the type.
    static std::string
    name()
    {
      return std::string("window3d");
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
      delta_(abs(dp.slice()));
      delta_(abs(dp.row()));
      delta_(abs(dp.col()));
      return delta_;
    }

  };

  // std win

  /*!
  ** \brief Create a window (3 dimensions) of 6 elements.
  ** \return The new window.
  */
  inline const window3d&
  win_c6_only()
  {
    static const coord crd[] = { -1,  0,  0,
				 0, -1,  0,
				 0,  0, -1,
				 0,  0,  1,
				 0,  1,  0,
				 1,  0,  0};
    static const window3d win(6, crd);
    return win;
  }

  /*!
  ** \brief Create a window (3 dimensions) of 7 elements.
  ** \return The new window.
  **
  ** It's the same than win_c6_only() plus the 0,0,0 point.
  */
  inline const window3d&
  win_c6p()
  {
    static const coord crd[] = { -1,  0,  0,
				 0, -1,  0,
				 0,  0, -1,
				 0,  0,  0,
				 0,  0,  1,
				 0,  1,  0,
				 1,  0,  0};
    static const window3d win(7, crd);
    return win;
  }

  /*!
  ** \brief Create a window (3 dimensions) of 18 elements.
  ** \return The new window.
  */
  inline const window3d&
  win_c18_only()
  {
    static const coord crd[] = { -1, -1,  0,
				 -1,  0, -1,
				 -1,  0,  0,
				 -1,  0,  1,
				 -1,  1,  0,
				 0, -1, -1,
				 0, -1,  0,
				 0, -1,  1,
				 0,  0, -1,
				 0,  0,  1,
				 0,  1, -1,
				 0,  1,  0,
				 0,  1,  1,
				 1, -1,  0,
				 1,  0, -1,
				 1,  0,  0,
				 1,  0,  1,
				 1,  1,  0 };
    static window3d win(18, crd);
    return win;
  }

  /*!
  ** \brief Create a window (3 dimensions) of 19 elements.
  ** \return The new window.
  **
  ** It's the same than win_c18_only() plus the 0,0,0 point.
  */
  inline const window3d&
  win_c18p()
  {
    static const coord crd[] = { -1, -1,  0,
				 -1,  0, -1,
				 -1,  0,  0,
				 -1,  0,  1,
				 -1,  1,  0,
				 0, -1, -1,
				 0, -1,  0,
				 0, -1,  1,
				 0,  0, -1,
				 0,  0,  0,
				 0,  0,  1,
				 0,  1, -1,
				 0,  1,  0,
				 0,  1,  1,
				 1, -1,  0,
				 1,  0, -1,
				 1,  0,  0,
				 1,  0,  1,
				 1,  1,  0 };
    static window3d win(19, crd);
    return win;
  }

  /*!
  ** \brief Create a window (3 dimensions) of 26 elements.
  ** \return The new window.
  */
  inline const window3d&
  win_c26_only()
  {
    static const coord crd[] = { -1, -1, -1,
				 -1, -1,  0,
				 -1, -1,  1,
				 -1,  0, -1,
				 -1,  0,  0,
				 -1,  0,  1,
				 -1,  1, -1,
				 -1,  1,  0,
				 -1,  1,  1,
				 0, -1, -1,
				 0, -1,  0,
				 0, -1,  1,
				 0,  0, -1,
				 0,  0,  1,
				 0,  1, -1,
				 0,  1,  0,
				 0,  1,  1,
				 1, -1, -1,
				 1, -1,  0,
				 1, -1,  1,
				 1,  0, -1,
				 1,  0,  0,
				 1,  0,  1,
				 1,  1, -1,
				 1,  1,  0,
				 1,  1,  1 };
    static window3d win(26, crd);
    return win;
  }

  /*!
  ** \brief Create a window (3 dimensions) of 27 elements.
  ** \return The new window.
  **
  ** It's the same than win_c26_only() plus the 0,0,0 point.
  */
  inline const window3d&
  win_c26p()
  {
    static const coord crd[] = { -1, -1, -1,
				 -1, -1,  0,
				 -1, -1,  1,
				 -1,  0, -1,
				 -1,  0,  0,
				 -1,  0,  1,
				 -1,  1, -1,
				 -1,  1,  0,
				 -1,  1,  1,
				 0, -1, -1,
				 0, -1,  0,
				 0, -1,  1,
				 0,  0, -1,
				 0,  0,  0,
				 0,  0,  1,
				 0,  1, -1,
				 0,  1,  0,
				 0,  1,  1,
				 1, -1, -1,
				 1, -1,  0,
				 1, -1,  1,
				 1,  0, -1,
				 1,  0,  0,
				 1,  0,  1,
				 1,  1, -1,
				 1,  1,  0,
				 1,  1,  1 };
    static window3d win(27, crd);
    return win;
  }

  // mk_win's

  /*!
  ** \brief Create a block window (3 dimension).
  ** \arg nslices Number of slice.
  ** \arg nrows Number of row.
  ** \arg ncols Number of column.
  ** \return The new window (3d).
  ** \pre nslices >= 3.
  ** \pre nslices % 2 == 1.
  ** \pre nrows >= 3.
  ** \pre nrows % 2 == 1.
  ** \pre ncols >= 3.
  ** \pre ncols % 2 == 1.
  */
  inline window3d
  mk_win_block(unsigned nslices, unsigned nrows, unsigned ncols)
  {
    precondition(nslices >= 3 && (nslices % 2) == 1);
    precondition(nrows >= 3 && (nrows % 2) == 1);
    precondition(ncols >= 3 && (ncols % 2) == 1);
    window3d win(nrows * ncols);
    int half_nslices = nslices / 2;
    int half_nrows = nrows / 2;
    int half_ncols = ncols / 2;
    for (coord slice = - half_nslices; slice <= half_nslices; ++slice)
      for (coord row = - half_nrows; row <= half_nrows; ++row)
	for (coord col = - half_ncols; col <= half_ncols; ++col)
	  win.add(slice, row, col);
    return win;
  }

  /*!
  ** \brief Create an ellipsoid window (3 dimension).
  ** \arg zradius radius Z.
  ** \arg yradius radius Y.
  ** \arg xradius radius X.
  ** \pre zradius > 0
  ** \pre yradius > 0
  ** \pre xradius > 0
  **
  ** The ellipsoid formula is :
  ** \f$$\frac{x^2}{xradius^2} + \frac{y^2}{yradius^2} + \frac{z^2}{zradius^2} = 1$\f$
  */
  inline window3d
  mk_win_ellipsoid(float zradius, float yradius, float xradius)
  {
    precondition(zradius > 0);
    precondition(yradius > 0);
    precondition(xradius > 0);

    window3d win;
    coord zmax = (coord)roundf(zradius);
    float zr2 = zradius * zradius;
    float yr2 = yradius * yradius;
    for (coord z = -zmax; z <= zmax; ++z)
      {
	/*
	      x^2         y^2         z^2
	   --------- + --------- + --------- = 1
	   xradius^2   yradius^2   zradius^2
	*/

	/* Set x to 0 in the above formula to find ymax.  */
	float v = 1 - z * z / zr2;
	if (v < 0) v = 0;	// Can happen because zmax has been rounded.
	coord ymax = (coord)roundf(yradius * sqrtf(v));
	for (coord y = -ymax; y <= ymax; ++y)
	  {
	    float w = v - y * y / yr2;
	    if (w < 0) w = 0;	// Can happen because ymax has been rounded.
	    coord xmax = (coord)roundf(xradius * sqrtf(w));
	    for (coord x = -xmax; x <= xmax; ++x)
	      win.add(z, y, x);
	  }
      }
    return win;
  }

  /*!
  ** \brief Create a cube neighborhood (3 dimension).
  ** \arg width Number of slice, colunm and row.
  ** \return The new neighborhood (3d).
  */
  inline window3d
  mk_win_cube(unsigned width)
  {
    return mk_win_block(width, width, width);
  }

  /*!
  ** \brief Create a ball neighborhood (3 dimension).
  ** \arg radius The radius.
  ** \return The new neighborhood (3d).
  */
  inline window3d
  mk_win_ball(float radius)
  {
    return mk_win_ellipsoid(radius, radius, radius);
  }

} // end of oln

#endif // OLENA_CORE_WINDOW3D_HH
