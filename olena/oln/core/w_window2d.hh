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

#ifndef OLENA_CORE_W_WINDOW2D_HH
# define OLENA_CORE_W_WINDOW2D_HH

# include <oln/core/abstract/w_windownd.hh>
# include <mlc/array/all.hh>
# include <oln/core/accum.hh>
# include <oln/core/winiter.hh>
# include <oln/core/winneighb.hh>
# include <oln/core/dpoint2d.hh>
# include <oln/core/window2d.hh>
# include <algorithm>

namespace oln {

  template<class T>
  class w_window2d; // fwd_decl


  template<class T>
  struct struct_elt_traits<w_window2d<T> >: public
  struct_elt_traits<abstract::w_windownd<w_window2d<T> > >
  {
    enum { dim = 2 };
    typedef T weight_type;
    typedef point2d point_type;
    typedef dpoint2d dpoint_type;
    typedef winiter< w_window2d<T> > iter_type;
    typedef winneighb< w_window2d<T> > neighb_type;
  };

  template<class T>
  class w_window2d : public abstract::w_windownd<w_window2d<T> >
  {
    typedef abstract::w_windownd< w_window2d<T> > super_type;
  public:
  
    typedef w_window2d<T> self_type;
    typedef typename struct_elt_traits< self_type >::iter_type   iter_type;
    typedef typename struct_elt_traits< self_type >::neighb_type
    neighb_type;
    typedef typename struct_elt_traits< self_type >::dpoint_type dpoint_type;
    typedef typename struct_elt_traits< self_type >::weight_type weight_type;

    w_window2d(): super_type() {}
    w_window2d(unsigned size) : super_type(size) {}

    coord delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.row()));
      delta_(abs(dp.col()));
      return delta_;
    }

    template<class I, class T2>
    w_window2d(const mlc::array2d<I, T2 >& arr) :
      super_type(I::card)
    {
      unsigned i = 0;
      for (coord row = -I::center_row; row <= I::nrows - I::center_row - 1; ++row)
	for (coord col = -I::center_col; col <= I::ncols - I::center_col - 1; ++col)
          add(row, col, arr[i++]);
    }

    w_window2d<T>& add(coord row, coord col, const weight_type& weight)
    {
      return add(dpoint_type(row, col), weight);
    }

    w_window2d<T>& add(const dpoint_type& dp, const weight_type& w)
    {
      return to_exact(this)->add_(dp, w);
    }

    const weight_type& set(const dpoint_type& dp, const weight_type& weight)
    {
      return to_exact(this)->set_(dp, weight);
    }

    const weight_type& set(coord row, coord col, const weight_type& weight)
    {
      return set(dpoint_type(row, col), weight);
    }

    static std::string name() { return std::string("w_window2d<")
				       + T::name() +  ">"; }

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
