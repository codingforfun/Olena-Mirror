// Copyright 2001  EPITA Research and Development Laboratory
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

# include <algorithm>
# include "core/internal/window.hh"
# include "core/winiter.hh"
# include "core/winneighb.hh"
# include "core/accum.hh"

namespace oln {

  class window2d : public internal::_window< 2, window2d >
  {
  public:
    typedef type::bottom inferior;
    typedef internal::_window< 2, window2d > super;
    typedef window2d self;

    typedef winiter< self >   iter;
    typedef winneighb< self > neighb;

    window2d& add(const dpoint2d& dp)
    {
      super::add(dp);
      _delta(abs(dp.row()));
      _delta(abs(dp.col()));
      return *this;
    }

    window2d& add(coord row, coord col)
    {
      return this->add(dpoint2d(row, col));
    }

    window2d() : super(), _delta(0) {}
    window2d(unsigned size) : super(size), _delta(0) {}
    window2d(unsigned n, const coord crd[]) : super(), _delta(0)
    {
      _dp.reserve(n);
      _centered = false;
      for (unsigned i = 0; i < 2 * n; i += 2)
	add(dpoint2d(crd[i], crd[i+1]));
    }

    coord delta() const
    {
      return _delta;
    }

    static std::string name() { return std::string("window2d"); }

    self operator-() const
    {
      self win(*this);
      win.sym();
      return win;
    }

  private:
    max_accumulator<coord> _delta;
  };


  window2d inter(const window2d& lhs, const window2d& rhs)
  {
    window2d win;
    for (unsigned i = 0; i < lhs.card(); ++i)
      if (rhs.has(lhs.dp(i)))
	win.add(lhs.dp(i));
    for (unsigned j = 0; j < rhs.card(); ++j)
      if (! win.has(rhs.dp(j)) && lhs.has(rhs.dp(j)))
	win.add(rhs.dp(j));
    return win;
  }

  window2d uni(const window2d& lhs, const window2d& rhs)
  {
    window2d win;
    for (unsigned i = 0; i < lhs.card(); ++i)
      win.add(lhs.dp(i));
    for (unsigned j = 0; j < rhs.card(); ++j)
      if (! win.has(rhs.dp(j)))
	win.add(rhs.dp(j));
    return win;
  }


  // std win

  const window2d&
  win_c4_only()
  {
    static const coord crd[] = { -1,0,  0,-1, 0,1,  1,0 };
    static const window2d win(4, crd);
    return win;
  }

  const window2d&
  win_c4p()
  {
    static const coord crd[] = { -1,0,  0,-1, 0,0, 0,1,  1,0 };
    static const window2d win(5, crd);
    return win;
  }

  const window2d&
  win_c8_only()
  {
    static const coord crd[] = { -1,-1, -1,0, -1,1,  0,-1, 0,1,  1,-1,  1,0,  1,1 };
    static const window2d win(8, crd);
    return win;
  }

  const window2d&
  win_c8p()
  {
    static const coord crd[] = { -1,-1, -1,0, -1,1,  0,-1, 0,0, 0,1,  1,-1,  1,0,  1,1 };
    static const window2d win(9, crd);
    return win;
  }

  // mk_win's

  window2d
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

  window2d
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

  window2d
  mk_win_square(unsigned width)
  {
    return mk_win_rectangle(width, width);
  }

  window2d
  mk_win_disc(float radius)
  {
    return mk_win_ellipse(radius, radius);
  }

  struct get_se<2>
  {
    typedef window2d ret;
  };


} // end of oln

#endif // OLENA_CORE_WINDOW2D_HH
