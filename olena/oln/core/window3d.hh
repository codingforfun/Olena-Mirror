// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

  class window3d; // fwd_decl

  template<>
  struct struct_elt_traits<window3d>: public
  struct_elt_traits<abstract::windownd<window3d> >
  {
    enum { dim = 3 };
    typedef point3d point_type;
    typedef dpoint3d dpoint_type;
    typedef winiter< window3d > iter_type;
    typedef winneighb< window3d > neighb_type;
  };

  class window3d : public abstract::windownd< window3d >
  {

  public:

    typedef abstract::windownd< window3d > super_type;
    typedef window3d self_type;

    typedef struct_elt_traits< self_type >::iter_type   iter_type;
    typedef struct_elt_traits< self_type >::neighb_type
    neighb_type;
    typedef struct_elt_traits< self_type >::dpoint_type dpoint_type;

    friend class abstract::window_base<abstract::window<window3d>, window3d>;

    window3d& add(const dpoint_type& dp)
    {
      return this->exact().add_(dp);
    }

    window3d& add(coord slice, coord row, coord col)
    {
      return this->add(dpoint_type(slice, row, col));
    }

    window3d() : super_type() 
    {}

    window3d(unsigned size) : super_type(size) 
    {}
    
    window3d(unsigned n, const coord crd[]) : super_type(n)
    {
      for (unsigned i = 0; i < 3 * n; i += 3)
	add(dpoint_type(crd[i], crd[i+1], crd[i+2]));
    }

    static std::string 
    name() 
    { 
      return std::string("window3d"); 
    }

  protected:

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

  inline window3d
  mk_win_cube(unsigned width)
  {
    return mk_win_block(width, width, width);
  }

  inline window3d
  mk_win_ball(float radius)
  {
    return mk_win_ellipsoid(radius, radius, radius);
  }

} // end of oln

#endif // OLENA_CORE_WINDOW3D_HH
