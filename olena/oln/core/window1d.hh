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

#ifndef OLENA_CORE_WINDOW1D_HH
# define OLENA_CORE_WINDOW1D_HH

# include <oln/core/point1d.hh>
# include <oln/core/dpoint1d.hh>
# include <oln/core/abstract/windownd.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>
# include <algorithm>

namespace oln {

  class window1d; // fwd_decl

  template<>
  struct struct_elt_traits<window1d>: public
  struct_elt_traits<abstract::windownd<window1d> >
  {
    enum { dim = 1 };
    typedef point1d point_type;
    typedef dpoint1d dpoint_type;
    typedef winiter< window1d > iter_type;
    typedef winneighb< window1d > neighb_type;
  };

  class window1d : public abstract::windownd< window1d >
  {
  public:
    typedef abstract::windownd< window1d > super_type;
    typedef window1d self_type;

    typedef struct_elt_traits< self_type >::iter_type   iter_type;
    typedef struct_elt_traits< self_type >::neighb_type
    neighb_type;
    typedef struct_elt_traits< self_type >::dpoint_type dpoint_type;

    coord delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.col()));
      return delta_;
    }

    window1d& add(const dpoint_type& dp)
    {
      return to_exact(this)->add_(dp);
    }

    window1d& add(coord col)
    {
      return this->add(dpoint_type(col));
    }

    window1d() : super_type() {}
    window1d(unsigned size) : super_type(size) {}
    window1d(unsigned n, const coord crd[]) : super_type(n)
    {
      for (unsigned i = 0; i < n; ++i)
	add(dpoint_type(crd[i]));
    }

    static std::string name() { return std::string("window1d"); }

  };

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
