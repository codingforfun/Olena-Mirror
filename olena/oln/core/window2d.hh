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

  class window2d; // fwd_decl

  template<>
  struct struct_elt_traits<window2d>: public
  struct_elt_traits<abstract::windownd<window2d> >
  {
    enum { dim = 2 };
    typedef point2d point_type;
    typedef dpoint2d dpoint_type;
    typedef winiter< window2d > iter_type;
    typedef winneighb< window2d > neighb_type;
  };

  class window2d : public abstract::windownd< window2d >
  {
  public:
    typedef abstract::windownd<window2d > super_type;
    typedef window2d self_type;

    typedef struct_elt_traits< self_type >::iter_type   iter_type;
    typedef struct_elt_traits< self_type >::neighb_type
    neighb_type;
    typedef struct_elt_traits< self_type >::dpoint_type
    dpoint_type;

    coord delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.row()));
      delta_(abs(dp.col()));
      return delta_;
    }

    window2d& add(const dpoint_type& dp)
    {
      return exact().add_(dp);
    }

    window2d& add(coord row, coord col)
    {
      dpoint_type dp(row, col);
      return add(dp);
    }

    window2d() : super_type() {}
    window2d(unsigned size) : super_type(size) {}
    window2d(unsigned n, const coord crd[]) : super_type(n)
    {
      for (unsigned i = 0; i < 2 * n; i += 2)
	add(dpoint_type(crd[i], crd[i+1]));
    }

    // io
    window2d(const io::internal::anything& r) : super_type()
    {
      r.assign(*this);
    }

    window2d& operator=(const io::internal::anything& r)
    {
      return r.assign(*this);
    }

    static std::string name() { return std::string("window2d"); }

  };


  // std win

  inline const window2d&
  win_c4_only()
  {
    static const coord crd[] = { -1,0,  0,-1, 0,1,  1,0 };
    static const window2d win(4, crd);
    return win;
  }

  inline const window2d&
  win_c4p()
  {
    static const coord crd[] = { -1,0,  0,-1, 0,0, 0,1,  1,0 };
    static const window2d win(5, crd);
    return win;
  }

  inline const window2d&
  win_c8_only()
  {
    static const coord crd[] = { -1,-1, -1,0, -1,1,  0,-1, 0,1,  1,-1,  1,0,  1,1 };
    static const window2d win(8, crd);
    return win;
  }

  inline const window2d&
  win_c8p()
  {
    static const coord crd[] = { -1,-1, -1,0, -1,1,  0,-1, 0,0, 0,1,  1,-1,  1,0,  1,1 };
    static const window2d win(9, crd);
    return win;
  }

  // mk_win's

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

  inline window2d
  mk_win_square(unsigned width)
  {
    return mk_win_rectangle(width, width);
  }

  inline window2d
  mk_win_disc(float radius)
  {
    return mk_win_ellipse(radius, radius);
  }

  template <>
  struct get_se<2>
  {
    typedef window2d ret;
  };


} // end of oln

#endif // OLENA_CORE_WINDOW2D_HH
