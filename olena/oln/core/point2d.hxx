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

#ifndef OLENA_CORE_POINT2D_HXX
# define OLENA_CORE_POINT2D_HXX

# include <oln/core/dpoint2d.hh>
# include <iostream>


namespace oln {

  inline
  point2d::point2d()
  {
    nth(0) = 0;
    nth(1) = 0;
  }

  inline
  point2d::point2d(coord row, coord col)
  {
    nth(0) = row;
    nth(1) = col;
  }

  inline const point2d&
  point2d::point_ref() const
  {
    return *this;
  }

  inline coord
  point2d::row() const
  {
    return nth(0);
  }

  inline coord&
  point2d::row()
  {
    return nth(0);
  }

  inline coord
  point2d::col() const
  {
    return nth(1);
  }

  inline coord&
  point2d::col()
  {
    return nth(1);
  }

  inline point2d&
  point2d::plus_assign_dp(const dpoint2d& dp)
  {
    row() += dp.row();
    col() += dp.col();
    return *this;
  }

  inline point2d&
  point2d::minus_assign_dp(const dpoint2d& dp)
  {
    row() -= dp.row();
    col() -= dp.col();
    return *this;
  }

  inline dpoint2d
  point2d::minus_p(const point2d& p) const
  {
    dpoint2d dp(row() - p.row(), col() - p.col());
    return dp;
  }

  inline point2d
  point2d::plus_dp(const dpoint2d& dp) const
  {
    point2d p = *this;
    p += dp;
    return p;
  }

  inline point2d
  point2d::minus_dp(const dpoint2d& dp) const
  {
    point2d p = *this;
    p -= dp;
    return p;
  }

  inline point2d
  point2d::minus() const
  {
    point2d p(row(), col());
    return p;
  }


} // end of oln


inline std::ostream&
operator<<(std::ostream& o, const oln::point2d& p)
{
  return o << '(' << p.row() << ',' << p.col() << ')';
}



#endif // ! OLENA_CORE_POINT2D_HXX
