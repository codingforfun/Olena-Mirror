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

#ifndef OLENA_CORE_NEIGHBORHOOD1D_HH
# define OLENA_CORE_NEIGHBORHOOD1D_HH

# include <oln/core/internal/neighborhood.hh>
# include <oln/core/winiter.hh>
# include <oln/core/accum.hh>
# include <oln/core/window1d.hh>
# include <algorithm>

namespace oln {

  class neighborhood1d :
    public internal::_neighborhood< 1, neighborhood1d >
  {
  public:
    typedef type::bottom inferior;
    typedef internal::_neighborhood< 1, neighborhood1d > super;
    typedef neighborhood1d self;

    typedef winiter< self >   iter;
    typedef winneighb< self > neighb;

    neighborhood1d& add(const dpoint1d& dp)
    {
      precondition( !dp.is_centered() );
      super::add(dp);
      super::add(-dp);
      _delta(abs(dp.col()));
      return *this;
    }

    neighborhood1d& add(coord col)
    {
      return this->add(dpoint1d(col));
    }

    neighborhood1d() : super(), _delta(0) {}
    neighborhood1d(unsigned size) : super(size), _delta(0) {}
    neighborhood1d(unsigned n, const coord crd[]) : super(), _delta(0)
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

    static std::string name() { return std::string("neighborhood1d"); }

    // obsolete
    self operator-() const
    {
      return *this;
    }

  private:
    max_accumulator<coord> _delta;
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
