// Copyright 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_POINT3D_HXX
# define OLENA_CORE_POINT3D_HXX

# include <iostream>
# include "core/dpoint3d.hh"


namespace oln {


  inline
  point3d::point3d()
  {
    nth(0) = 0;
    nth(1) = 0;
    nth(2) = 0;
  }

  inline
  point3d::point3d(coord slice, coord row, coord col)
  {
    nth(0) = slice;
    nth(1) = row;
    nth(2) = col;
  }

  inline const point3d&
  point3d::point_ref() const
  {
    return *this;
  }

  inline coord
  point3d::slice() const
  {
    return nth(0);
  }

  inline coord&
  point3d::slice()
  {
    return nth(0);
  }

  inline coord
  point3d::row() const
  {
    return nth(1);
  }

  inline coord&
  point3d::row()
  {
    return nth(1);
  }

  inline coord
  point3d::col() const
  {
    return nth(2);
  }

  inline coord&
  point3d::col()
  {
    return nth(2);
  }

  inline point3d&
  point3d::operator+=(const dpoint3d& dp)
  {
    slice() += dp.slice();
    row() += dp.row();
    col() += dp.col();
    return *this;
  }

  inline point3d&
  point3d::operator-=(const dpoint3d& dp)
  {
    slice() -= dp.slice();
    row() -= dp.row();
    col() -= dp.col();
    return *this;
  }

  inline dpoint3d
  point3d::operator-(const point3d& p) const
  {
    dpoint3d dp(slice() - p.slice(),
		row() - p.row(),
		col() - p.col());
    return dp;
  }

  inline point3d
  point3d::operator+(const dpoint3d& dp) const
  {
    point3d p = *this;
    p += dp;
    return p;
  }

  inline point3d
  point3d::operator-(const dpoint3d& dp) const
  {
    point3d p = *this;
    p -= dp;
    return p;
  }

  inline point3d
  point3d::operator-() const
  {
    point3d p(-slice(), -row(), -col());
    return p;
  }

} // end of oln


inline std::ostream&
operator<<(std::ostream& o, const oln::point3d& p)
{
  return o << '(' << p.slice() << ',' << p.row() << ',' << p.col() << ')';
}



#endif // ! OLENA_CORE_POINT3D_HXX
