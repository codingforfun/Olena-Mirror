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

#ifndef OLENA_CORE_NEIGHBORHOOD3D_HH
# define OLENA_CORE_NEIGHBORHOOD3D_HH

# include <oln/core/abstract/neighborhoodnd.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>
# include <oln/core/window3d.hh>
# include <algorithm>

namespace oln {

  class neighborhood3d; // fwd_decl

  template<>
  struct struct_elt_traits<neighborhood3d>: public
  struct_elt_traits<abstract::neighborhoodnd<neighborhood3d> >
  {
    enum { dim = 3 };
    typedef point3d point_type;
    typedef dpoint3d dpoint_type;
    typedef winiter< neighborhood3d > iter_type;
    typedef winneighb< neighborhood3d > neighb_type;
    typedef window3d win_type;
  };


  class neighborhood3d :
    public abstract::neighborhoodnd< neighborhood3d >
  {
  public:

    typedef abstract::neighborhoodnd< neighborhood3d > super_type;
    typedef neighborhood3d self_type;

    typedef struct_elt_traits< self_type >::iter_type   iter_type;
    typedef struct_elt_traits< self_type >::neighb_type
    neighb_type;
    typedef struct_elt_traits< self_type >::dpoint_type dpoint_type;

    friend class abstract::window_base<abstract::neighborhood<neighborhood3d>, neighborhood3d>;

    neighborhood3d& 
    add(const dpoint_type& dp)
    {
      this->exact().add_(dp);
      return this->exact().add_(-dp);
    }

    neighborhood3d& 
    add(coord slice, coord row, coord col)
    {
      return this->add(dpoint3d(slice, row, col));
    }

    neighborhood3d() : super_type() 
    {}

    neighborhood3d(unsigned size) : super_type(size) 
    {}

    neighborhood3d(unsigned n, const coord crd[]) : super_type()    
    {
      for (unsigned i = 0; i < 3 * n; i += 3)
	add(dpoint_type(crd[i], crd[i+1], crd[i+2]));
    }

    static std::string 
    name() 
    { 
      return std::string("neighborhood3d"); 
    }

  protected:

    coord 
    delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.slice()));
      delta_(abs(dp.row()));
      delta_(abs(dp.col()));
      return delta_;
    }

  };

 // std neighb

  inline const neighborhood3d&
  neighb_c6()
  {
    static const coord crd[] = { 0,  0,  1,
				 0,  1,  0,
				 1,  0,  0};
    static const neighborhood3d neighb(3, crd);
    return neighb;
  }

  inline const neighborhood3d&
  neighb_c18()
  {
    static const coord crd[] = { 0,  0,  1,
				 0,  1, -1,
				 0,  1,  0,
				 0,  1,  1,
				 1, -1,  0,
				 1,  0, -1,
				 1,  0,  0,
				 1,  0,  1,
				 1,  1,  0 };
    static neighborhood3d neighb(9, crd);
    return neighb;
  }

  inline const neighborhood3d&
  neighb_c26()
  {
    static const coord crd[] = { 0,  0,  1,
				 0,  1, -1,
				 0,  1,  0,
				 0,  1,  1,
				 1, -1, -1,
				 1, -1,  0,
				 1, -1,  1,
				 1,  0, -1,
				 1,  0,  0,
				 1,  0,  1,
				 1,  1, -1,
				 1,  1,  0,
				 1,  1,  1 };
    static neighborhood3d neighb(13, crd);
    return neighb;
  }

  // mk_neighb's

  inline neighborhood3d
  mk_neighb_block(unsigned nslices, unsigned nrows, unsigned ncols)
  {
    precondition(nslices >= 3 && (nslices % 2) == 1);
    precondition(nrows >= 3 && (nrows % 2) == 1);
    precondition(ncols >= 3 && (ncols % 2) == 1);
    neighborhood3d neighb(nrows * ncols);
    int half_nslices = nslices / 2;
    int half_nrows = nrows / 2;
    int half_ncols = ncols / 2;
    for (coord slice = - half_nslices; slice <= half_nslices; ++slice)
      for (coord row = - half_nrows; row <= half_nrows; ++row)
	for (coord col = (slice > 0) ? 0 : 1 ; col <= half_ncols; ++col)
	    neighb.add(slice, row, col);
    return neighb;
  }

  inline neighborhood3d
  mk_neighb_cube(unsigned width)
  {
    return mk_neighb_block(width, width, width);
  }

  inline window3d 
  mk_win_from_neighb(const neighborhood3d& n)
  {
    window3d win(n.card());
    for (unsigned i = 0; i < n.card(); ++i)
      win.add(n.dp(i));
    return win;
  }

} // end of oln

#endif // OLENA_CORE_NEIGHBORHOOD3D_HH
