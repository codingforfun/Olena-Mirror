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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef	OLENA_CORE_DPOINT3D_HXX
# define OLENA_CORE_DPOINT3D_HXX

# include <oln/core/point3d.hh>
# include <iostream>

namespace oln {

  inline coord
  dpoint3d::slice() const
  {
    return nth(0);
  }

  inline coord&
  dpoint3d::slice()
  {
    return nth(0);
  }

  inline coord
  dpoint3d::row() const
  {
    return nth(1);
  }

  inline coord&
  dpoint3d::row()
  {
    return nth(1);
  }

  inline coord
  dpoint3d::col() const
  {
    return nth(2);
  }

  inline coord&
  dpoint3d::col()
  {
    return nth(2);
  }

  inline
  dpoint3d::dpoint3d()
  {
    slice() = 0;
    row() = 0;
    col() = 0;
  }

  inline
  dpoint3d::dpoint3d(coord s, coord r, coord c)
  {
    slice() = s;
    row() = r;
    col() = c;
  }

  inline
  dpoint3d::dpoint3d(const point3d& p) : super_type(p)
  {
  }

  inline dpoint3d
  dpoint3d::minus() const
  {
    dpoint3d dp(-slice(), -row(), -col());
    return dp;
  }

  inline dpoint3d&
  dpoint3d::plus_assign_dp(const dpoint3d& dp)
  {
    slice() += dp.slice();
    row() += dp.row();
    col() += dp.col();
    return *this;
  }

  inline dpoint3d&
  dpoint3d::minus_assign_dp(const dpoint3d& dp)
  {
    slice() -= dp.slice();
    row() -= dp.row();
    col() -= dp.col();
    return *this;
  }

  inline dpoint3d
  dpoint3d::plus_dp(const dpoint3d& dp) const
  {
    dpoint3d tmp = *this;
    tmp += dp;
    return tmp;
  }

  inline dpoint3d
  dpoint3d::minus_dp(const dpoint3d& dp) const
  {
    dpoint3d tmp = *this;
    tmp -= dp;
    return tmp;
  }

} // end of oln

inline std::ostream&
operator<<(std::ostream& o, const oln::dpoint3d& dp)
{
  return o << '(' << dp.slice() << ',' << dp.row() << ',' << dp.col() << ')';
}

#endif // ! OLENA_CORE_DPOINT3D_HXX
