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

#ifndef OLENA_CORE_NEIGHBORHOOD1D_HH
# define OLENA_CORE_NEIGHBORHOOD1D_HH

# include <oln/core/abstract/neighborhoodnd.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>
# include <oln/core/window1d.hh>
# include <algorithm>

namespace oln {

  class neighborhood1d; // fwd_decl

  template<>
  struct struct_elt_traits<neighborhood1d>: public
  struct_elt_traits<abstract::neighborhoodnd<neighborhood1d> >
  {
    enum { dim = 1 };
    typedef point1d point_type;
    typedef dpoint1d dpoint_type;
    typedef winiter< neighborhood1d > iter_type;
    typedef winneighb< neighborhood1d > neighb_type;
    typedef window1d win_type;
  };

  class neighborhood1d :
    public abstract::neighborhoodnd< neighborhood1d >
  {
  public:

    typedef abstract::neighborhoodnd< neighborhood1d > super_type;
    typedef neighborhood1d self_type;

    typedef struct_elt_traits< self_type >::iter_type   iter_type;
    typedef struct_elt_traits< self_type >::neighb_type
    neighb_type;
    typedef struct_elt_traits< self_type >::dpoint_type dpoint_type;
    
    friend class abstract::window_base<abstract::neighborhood<neighborhood1d>, neighborhood1d>;

    neighborhood1d& add(const dpoint_type& dp)
    {
      this->exact().add_(dp);
      return this->exact().add_(-dp);
    }

    neighborhood1d& add(coord col)
    {
      return this->add(dpoint_type(col));
    }

    neighborhood1d() : super_type() {}
    neighborhood1d(unsigned size) : super_type(size) {}
    neighborhood1d(unsigned n, const coord crd[]) : super_type()
    {
      for (unsigned i = 0; i < n; ++i)
	add(dpoint_type(crd[i]));
    }

    static std::string name() { return std::string("neighborhood1d"); }

  protected:

    coord delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.col()));
      return delta_;
    }

  };


  // std neighb

  inline const
  neighborhood1d& neighb_c2()
  {
    static const coord crd[] = {  1 };
    static const neighborhood1d neighb(1, crd);
    return neighb;
  }

  inline
  neighborhood1d mk_neighb_segment(unsigned width)
  {
    precondition(width>= 3 && (width % 2) == 1);
    neighborhood1d neighb(width);
    int half_ncols = width / 2;
    for (coord col = 1; col <= half_ncols; ++col)
	neighb.add(col);
    return neighb;
  }

  inline
  window1d mk_win_from_neighb(const neighborhood1d& n)
  {
    window1d win(n.card());
    for (unsigned i = 0; i < n.card(); ++i)
      win.add(n.dp(i));
    return win;
  }


} // end of oln

#endif // OLENA_CORE_NEIGHBORHOOD1D_HH
