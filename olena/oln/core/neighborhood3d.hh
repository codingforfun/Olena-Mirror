// Copyright (C) 2001, 2002, 2003, 2004 EPITA Research and Development
// Laboratory
//
// This  file is  part of  the Olena  Library.  This  library  is free
// software; you can redistribute it  and/or modify it under the terms
// of the  GNU General  Public License version  2 as published  by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with  this library; see the  file COPYING.  If  not, write to
// the Free Software Foundation, 59  Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a  special exception, you  may use this  file as part of  a free
// software library without restriction.  Specifically, if other files
// instantiate templates  or use macros or inline  functions from this
// file, or  you compile  this file  and link it  with other  files to
// produce  an executable,  this file  does  not by  itself cause  the
// resulting  executable  to be  covered  by  the  GNU General  Public
// License.   This exception  does  not however  invalidate any  other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_CORE_NEIGHBORHOOD3D_HH
# define OLENA_CORE_NEIGHBORHOOD3D_HH


# include <oln/core/neighborhood_nd.hh>
# include <oln/core/dpoint2d.hh>

namespace oln {


  template <>
  struct neighborhood<3> : public abstract::neighborhood_nd< neighborhood<3> >
  {
    neighborhood& add(coord slice, coord row, coord col)
    {
      return this->add(dpoint3d(slice, row, col));
    }
    /*!
    ** \brief Construct a neighborhood of 3 dimensions.
    */
    neighborhood()
    {}

    /*!
    ** \brief Construct a neighborhood of 3 dimension.
    ** \arg n Add 'n' elements to the neighborhood.
    ** \arg crd Coordinates of the 'n' elements.
    */
    neighborhood(unsigned n, const coord crd[]) : super_type()
    {
      for (unsigned i = 0; i < 3 * n; i += 3)
	add(crd[i], crd[i+1], crd[i+2]);
    }
  };

  typedef neighborhood<3>	neighborhood3d;

  // std neighb

  /*!
  ** \brief Create a neighborhood (3 dimension) with 3 coordinates.
  ** \return The new neighborhood.
  */
  inline const neighborhood3d&
  neighb_c6()
  {
    static const coord crd[] = { 0,  0,  1,
				 0,  1,  0,
				 1,  0,  0};
    static const neighborhood3d neighb(3, crd);
    return neighb;
  }

  /*!
  ** \brief Create a neighborhood (3 dimension) with 9 coordinates.
  ** \return The new neighborhood.
  */
  inline const neighborhood3d&
  neighb_c18()
  {
    static const coord crd[] = { 0,  0,  1,
				 0,  1, -1,
				 0,  1,  0,
				 0,  1,  1,
				 1, -1,  0,
				 1,  0, -1,
				 1,  0,  0,
				 1,  0,  1,
				 1,  1,  0 };
    static neighborhood3d neighb(9, crd);
    return neighb;
  }

  /*!
  ** \brief Create a neighborhood (3 dimension) with 13 coordinates.
  ** \return The new neighborhood.
  */
  inline const neighborhood3d&
  neighb_c26()
  {
    static const coord crd[] = { 0,  0,  1,
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
    static neighborhood3d neighb(13, crd);
    return neighb;
  }

  // mk_neighb's

  /*!
  ** \brief Create a block neighborhood (3 dimension).
  ** \arg nslices Number of slice.
  ** \arg nrows Number of row.
  ** \arg ncols Number of column.
  ** \return The new neighborhood (3d).
  ** \pre nslices >= 3.
  ** \pre nslices % 2 == 1.
  ** \pre nrows >= 3.
  ** \pre nrows % 2 == 1.
  ** \pre ncols >= 3.
  ** \pre ncols % 2 == 1.
  */
  inline neighborhood3d
  mk_neighb_block(unsigned nslices, unsigned nrows, unsigned ncols)
  {
    precondition(nslices >= 3 && (nslices % 2) == 1);
    precondition(nrows >= 3 && (nrows % 2) == 1);
    precondition(ncols >= 3 && (ncols % 2) == 1);
    neighborhood3d neighb(nrows * ncols);
    int half_nslices = nslices / 2;
    int half_nrows = nrows / 2;
    int half_ncols = ncols / 2;
    for (coord slice = - half_nslices; slice <= half_nslices; ++slice)
      for (coord row = - half_nrows; row <= half_nrows; ++row)
	for (coord col = (slice > 0) ? 0 : 1 ; col <= half_ncols; ++col)
	    neighb.add(slice, row, col);
    return neighb;
  }


  /*!
  ** \brief Create an ellipsoid neighborhood (3 dimension).
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
  inline neighborhood3d
  mk_neighb_ellipsoid(float zradius, float yradius, float xradius)
  {
    precondition(zradius > 0);
    precondition(yradius > 0);
    precondition(xradius > 0);

    neighborhood3d neighb;
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
	    for (coord x = z > 0 ? 0 : 1; x <= xmax; ++x)
	      neighb.add(z, y, x);
	  }
      }
    return neighb;
  }


  /*!
  ** \brief Create a cube neighborhood (3 dimension).
  ** \arg width Number of slice, colunm and row.
  ** \return The new neighborhood (3d).
  */
  inline neighborhood3d
  mk_neighb_cube(unsigned width)
  {
    return mk_neighb_block(width, width, width);
  }

  /*!
  ** \brief Convert a neighborhood (3 dimensions) to a window (3 dimensions).
  ** \arg n The neighborhood to convert.
  ** \return The new window.
  */
  inline window3d
  mk_win_from_neighb(const neighborhood3d& n)
  {
    window3d win(n.card());
    for (unsigned i = 0; i < n.card(); ++i)
      win.add(n.dp(i));
    return win;
  }

  /*!
  ** \brief Create a ball neighborhood (3 dimension).
  ** \arg radius The radius.
  ** \return The new neighborhood (3d).
  */
  inline neighborhood3d
  mk_neighb_ball(float radius)
  {
    return mk_neighb_ellipsoid(radius, radius, radius);
  }
} // end of oln

#endif // OLENA_CORE_NEIGHBORHOOD3D_HH
