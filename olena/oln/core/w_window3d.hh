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

#ifndef OLENA_CORE_W_WINDOW3D_HH
# define OLENA_CORE_W_WINDOW3D_HH

# include <oln/core/internal/w_window.hh>
# include <oln/core/accum.hh>
# include <oln/core/winiter.hh>
# include <oln/core/winneighb.hh>
# include <oln/core/dpoint3d.hh>
# include <oln/core/window3d.hh>
# include <algorithm>

namespace oln {

  template<class T, class Exact = mlc::final>
  class w_window3d : public internal::_w_window< 3, T, typename mlc::exact_vt<w_window3d<T, Exact>, Exact>::ret >
  {
    typedef internal::_w_window< 3, T, typename mlc::exact_vt<w_window3d<T, Exact>, Exact>::ret > super;
  public:

    typedef w_window3d self;

    typedef winiter< self >   iter;
    typedef winneighb< self > neighb;

    w_window3d<T>& add(coord slice, coord row, coord col, T weight)
    {
      return add(dpoint3d(slice, row, col), weight);
    }

    w_window3d(): super(), _delta(0) {}
    w_window3d(unsigned size) : super(size), _delta(0) {}

    w_window3d<T>& add(const dpoint3d& dp, T weight)
    {
      if (weight == 0)		// Don't add 0 weighted entries
	return *this;

      super::add(dp, weight);
      _delta(abs(dp.slice()));
      _delta(abs(dp.row()));
      _delta(abs(dp.col()));
      return *this;
    }

    T& set(const dpoint3d& dp, T weight)
    {
      // if the dp exists, return a ref to the existing entry
      for (unsigned i = 0; i < card(); ++i)
	if (_dp[i] == dp)
	  {
	    _w[i] = weight;
	    return _w[i];
	  }

      // otherwise, create new entry
      add(dp, weight);
      return _w.back();
    }

    T& set(coord slice, coord row, coord col)
    {
      return set(dpoint3d(slice, row, col));
    }

    coord delta() const
    {
      return _delta;
    }

    self operator-() const
    {
      self win(*this);
      win.sym();
      return win;
    }

    static std::string name() { return std::string("w_window3d")
				       + T::name() + ","
				       + Exact::name() + ">"; }
  private:
    max_accumulator<coord> _delta;
  };


  template<class T>
  w_window3d<T> mk_w_win_from_win(T weight, const window3d& win)
  {
    w_window3d<T> w_win(win.card());
    for (unsigned i = 0; i < win.card(); ++i)
      w_win.add(win.dp(i), weight);
    return w_win;
  }

} // end of oln


#endif // ! OLENA_CORE_W_WINDOW3D_HH
