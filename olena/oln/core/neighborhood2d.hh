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

#ifndef OLENA_CORE_NEIGHBORHOOD2D_HH
# define OLENA_CORE_NEIGHBORHOOD2D_HH

# include <oln/core/abstract/neighborhoodnd.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>
# include <oln/core/window2d.hh>
# include <oln/io/readable.hh>
# include <algorithm>

namespace oln {

  class neighborhood2d; // fwd_decl

  template<>
  struct struct_elt_traits<neighborhood2d>: public
  struct_elt_traits<abstract::neighborhoodnd<neighborhood2d> >
  {
    enum { dim = 2 };
    typedef point2d point_type;
    typedef dpoint2d dpoint_type;
    typedef winiter< neighborhood2d > iter_type;
    typedef winneighb< neighborhood2d > neighb_type;
    typedef window2d win_type;
  };


  class neighborhood2d :
    public abstract::neighborhoodnd< neighborhood2d >
  {
  public:

    typedef abstract::neighborhoodnd< neighborhood2d > super_type;
    typedef neighborhood2d self_type;

    typedef struct_elt_traits< self_type >::iter_type   iter_type;
    typedef struct_elt_traits< self_type >::neighb_type
    neighb_type;
    typedef struct_elt_traits< self_type >::dpoint_type dpoint_type;

    coord delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.row()));
      delta_(abs(dp.col()));
      return delta_;
    }

    neighborhood2d& add(const dpoint_type& dp)
    {
      this->exact().add_(dp);
      return this->exact().add_(-dp);
    }

    neighborhood2d& add(coord row, coord col)
    {
      return this->add(dpoint_type(row, col));
    }

    neighborhood2d() : super_type() {}
    neighborhood2d(unsigned size) : super_type(size) {}
    neighborhood2d(unsigned n, const coord crd[]) : super_type(n)
    {
      for (unsigned i = 0; i < 2 * n; i += 2)
	add(dpoint_type(crd[i], crd[i+1]));
    }

    // io
    neighborhood2d(const io::internal::anything& r) : super_type()
    {
      r.assign(*this);
    }
    neighborhood2d& operator=(const io::internal::anything& r)
    {
      return r.assign(*this);
    }

    static std::string name() { return std::string("neighborhood2d"); }

  };

  // std neighbs

  inline const neighborhood2d&
  neighb_c4()
  {
    static const coord crd[] = { 0,1,  1,0 };
    static const neighborhood2d neighb(2, crd);
    return neighb;
  }

  inline const neighborhood2d&
  neighb_c8()
  {
    static const coord crd[] = { 0,1,  1,1,  1,0,  1,-1 };
    static const neighborhood2d neighb(4, crd);
    return neighb;
  }

  // mk_neighb's

  inline neighborhood2d
  mk_neighb_rectangle(unsigned nrows, unsigned ncols)
  {
    precondition(nrows >= 3 && (nrows % 2) == 1);
    precondition(ncols >= 3 && (ncols % 2) == 1);
    neighborhood2d neighb(nrows * ncols);
    int half_nrows = nrows / 2, half_ncols = ncols / 2;
    for (coord row = - half_nrows; row <= half_nrows; ++row)
      for (coord col = (row <= 0)? 1 : 0; col <= half_ncols; ++col)
	  neighb.add(row, col);
    return neighb;
  }

  inline neighborhood2d
  mk_neighb_square(unsigned width)
  {
    return mk_neighb_rectangle(width, width);
  }

  inline
  window2d mk_win_from_neighb(const neighborhood2d& n)
  {
    window2d win(n.card());
    for (unsigned i = 0; i < n.card(); ++i)
      win.add(n.dp(i));
    return win;
  }

} // end of oln

#endif // OLENA_CORE_NEIGHBORHOOD2D_HH
