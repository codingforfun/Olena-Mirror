// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_W_WINDOW2D_HH
# define OLENA_CORE_W_WINDOW2D_HH

# include <algorithm>
# include <oln/core/internal/w_window.hh>
# include <oln/meta/array.hh>
# include <oln/core/accum.hh>

namespace oln {

  template<class T>
  class w_window2d : public internal::_w_window< 2, T, w_window2d<T> >
  {
    typedef internal::_w_window< 2, T, w_window2d<T> > super;
  public:
    typedef w_window2d self;

    typedef winiter< self >   iter;
    typedef winneighb< self > neighb;

    w_window2d<T>& add(coord row, coord col, T weight)
    {
      return add(dpoint2d(row, col), weight);
    }

    w_window2d(): super(), _delta(0) {}
    w_window2d(unsigned size) : super(size), _delta(0) {}

    w_window2d<T>& add(const dpoint2d& dp, T weight)
    {
      if (weight == 0)		// Don't add 0 weighted entries
	return *this;

      super::add(dp, weight);
      _delta(abs(dp.row()));
      _delta(abs(dp.col()));
      return *this;
    }

    template<class I, class T2>
    w_window2d(const meta::array2d<I, T2 >& arr) :
      super(I::card), _delta(0)
    {
      unsigned i = 0;
      for (coord row = -I::center_row; I::row < I::nrows - I::center_row - 1; ++row)
	for (coord col = -I::center_col; col < I::ncols - I::center_col - 1; ++col)
          add(row, col, arr[i++]);
    }
    

    T& set(const dpoint2d& dp)
    {
      // if the dp exists, return a ref to the existing entry
      for (unsigned i = 0; i < card(); ++i)
	if (_dp[i] == dp)
	  return _w[i];

      // otherwise, create new entry
      super::add(dp);
      _w.push_back(T());
      return _w.back();
    }

    T& set(coord row, coord col)
    {
      return set(dpoint2d(row, col));
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

    static std::string name() { return std::string("w_window2d"); }
  private:
    max_accumulator<coord> _delta;
  };


  template<class T>
  w_window2d<T> mk_w_win_from_win(T weight, const window2d& win)
  {
    w_window2d<T> w_win(win.card());
    for (unsigned i = 0; i < win.card(); ++i)
      w_win.add(win.dp(i), weight);
    return w_win;
  }

} // end of oln

#endif // ! OLENA_CORE_W_WINDOW2D_HH
