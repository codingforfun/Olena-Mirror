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

#ifndef OLENA_CORE_W_WINDOW1D_HH
# define OLENA_CORE_W_WINDOW1D_HH

# include <oln/core/abstract/w_windownd.hh>
# include <oln/core/accum.hh>
# include <oln/core/winiter.hh>
# include <oln/core/winneighb.hh>
# include <oln/core/dpoint1d.hh>
# include <oln/core/window1d.hh>
# include <algorithm>

namespace oln {

  template<class T>
  class w_window1d; // fwd_decl


  template<class T>
  struct struct_elt_traits<w_window1d<T> >: public
  struct_elt_traits<abstract::w_windownd<w_window1d<T> > >
  {
    enum { dim = 1 };
    typedef T weight_type;
    typedef point1d point_type;
    typedef dpoint1d dpoint_type;
    typedef winiter< w_window1d<T> > iter_type;
    typedef winneighb< w_window1d<T> > neighb_type;
  };

  template<class T>
  class w_window1d : public abstract::w_windownd<w_window1d<T> >
  {
    typedef abstract::w_windownd< w_window1d<T> > super_type;

  public:
   
    typedef w_window1d<T> self_type;
    typedef typename struct_elt_traits< self_type >::iter_type   iter_type;
    typedef typename struct_elt_traits< self_type >::neighb_type
    neighb_type;
    typedef typename struct_elt_traits< self_type >::dpoint_type dpoint_type;
    typedef typename struct_elt_traits< self_type >::weight_type weight_type;

    w_window1d(): super_type() {}
    w_window1d(unsigned size) : super_type(size) {}

   
    coord delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.col()));
      return this->delta_;
    }

    w_window1d<T>& add(const dpoint_type& dp, const weight_type& w)
    {
      return exact().add_(dp, w);
    }

    w_window1d<T>& add(coord col, const weight_type& weight)
    {
      return add(dpoint_type(col), weight);
    }

    const weight_type& set(const dpoint_type& dp, const weight_type& weight)
    {
      return exact().set_(dp, weight);
    }

    const weight_type& set(coord col, const weight_type& weight)
    {
      return set(dpoint_type(col), weight);
    }

    static std::string name() { return std::string("w_window1d<") 
				       + T::name() + ">"; }
  };


  template<class T>
  w_window1d<T> mk_w_win_from_win(T weight, const window1d& win)
  {
    w_window1d<T> w_win(win.card());
    for (unsigned i = 0; i < win.card(); ++i)
      w_win.add(win.dp(i), weight);
    return w_win;
  }

} // end of oln

#endif // ! OLENA_CORE_W_WINDOW1D_HH
