// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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
# include <oln/core/internal/window.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>
# include <algorithm>

namespace oln {

  class window1d : public internal::_window< 1, window1d >
  {
  public:
    typedef type::bottom inferior;
    typedef internal::_window< 1, window1d > super;
    typedef window1d self;

    typedef winiter< self >   iter;
    typedef winneighb< self > neighb;

    window1d& add(const dpoint1d& dp)
    {
      super::add(dp);
      _delta(abs(dp.col()));
      return *this;
    }

    window1d& add(coord col)
    {
      return this->add(dpoint1d(col));
    }

    window1d() : super(), _delta(0) {}
    window1d(unsigned size) : super(size), _delta(0) {}
    window1d(unsigned n, const coord crd[]) : super(), _delta(0)
    {
      _dp.reserve(n);
      _centered = false;
      for (unsigned i = 0; i < n; ++i)
	add(dpoint1d(crd[i]));
    }

    coord delta() const
    {
      return _delta;
    }

    static std::string name() { return std::string("window1d"); }

    self operator-() const
    {
      self win(*this);
      win.sym();
      return win;
    }

  private:
    max_accumulator<coord> _delta;
  };


  inline
  window1d inter(const window1d& lhs, const window1d& rhs)
  {
    window1d win;
    for (unsigned i = 0; i < lhs.card(); ++i)
      if (rhs.has(lhs.dp(i)))
	win.add(lhs.dp(i));
    for (unsigned j = 0; j < rhs.card(); ++j)
      if (! win.has(rhs.dp(j)) && lhs.has(rhs.dp(j)))
	win.add(rhs.dp(j));
    return win;
  }

  inline
  window1d uni(const window1d& lhs, const window1d& rhs)
  {
    window1d win;
    for (unsigned i = 0; i < lhs.card(); ++i)
      win.add(lhs.dp(i));
    for (unsigned j = 0; j < rhs.card(); ++j)
      if (! win.has(rhs.dp(j)))
	win.add(rhs.dp(j));
    return win;
  }


  // std win

  inline const window1d&
  win_c2_only()
  {
    static const coord crd[] = { -1, 1 };
    static const window1d win(2, crd);
    return win;
  }

  inline const window1d&
  win_c2p()
  {
    static const coord crd[] = { -1, 0, 1 };
    static const window1d win(3, crd);
    return win;
  }

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

  struct get_se<1>
  {
    typedef window1d ret;
  };

} // end of oln

#endif // OLENA_CORE_WINDOW1D_HH
