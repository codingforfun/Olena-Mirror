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

#ifndef OLENA_CORE_POINT1D_HXX
# define OLENA_CORE_POINT1D_HXX

# include <oln/core/dpoint1d.hh>
# include <iostream>

namespace oln {

  inline
  point1d::point1d()
  {
    nth(0) = 0;
  }

  inline
  point1d::point1d(coord col)
  {
    nth(0) = col;
  }

  inline coord
  point1d::col() const
  {
    return nth(0);
  }

  inline coord&
  point1d::col()
  {
    return nth(0);
  }

  inline point1d&
  point1d::plus_assign_dp(const dpoint1d& dp)
  {
    col() += dp.col();
    return *this;
  }

  inline point1d&
  point1d::minus_assign_dp(const dpoint1d& dp)
  {
    col() -= dp.col();
    return *this;
  }

  inline dpoint1d
  point1d::minus_p(const point1d& p) const
  {
    dpoint1d dp(col() - p.col());
    return dp;
  }

  inline point1d
  point1d::plus_dp(const dpoint1d& dp) const
  {
    point1d p = *this;
    p += dp;
    return p;
  }

  inline point1d
  point1d::minus_dp(const dpoint1d& dp) const
  {
    point1d p = *this;
    p -= dp;
    return p;
  }

  inline point1d
  point1d::minus() const
  {
    point1d p(-col());
    return p;
  }


} // end of oln

inline std::ostream&
operator<<(std::ostream& o, const oln::point1d& p)
{
  return o << '(' << p.col() << ')';
}

#endif // ! OLENA_CORE_POINT1D_HXX
