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

#ifndef OLENA_CORE_POINT3D_HH
# define OLENA_CORE_POINT3D_HH

# include <ntg/predecls.hh>
# include <oln/core/coord.hh>
# include <oln/core/pointnd.hh>
# include <iostream>

namespace oln {


  // fwd decl
  class dpoint3d;

  class point3d : public pointnd< 3, point3d >
  {
  public:

    typedef pointnd< 3, point3d >	super;
    typedef dpoint3d			dpoint;

    enum { dim = 3 };

    point3d();
    point3d(coord slice, coord row, coord col);

    const point3d& point_ref() const;

    coord slice() const;
    coord& slice();
    coord row() const;
    coord& row();
    coord col() const;
    coord& col();

    point3d operator+(const dpoint3d& dp) const;
    point3d operator-(const dpoint3d& dp) const;
    point3d& operator+=(const dpoint3d& dp);
    point3d& operator-=(const dpoint3d& dp);

    dpoint3d operator-(const point3d& p) const;
    point3d operator-() const;

    static std::string name() { return "point3d"; }
  };

  _PointForDim(3, point3d);

  namespace internal
  {
    template<>
    struct default_less<point3d> : public default_less<point3d::super>
    {
    };

  } // end of internal

} // end of oln

inline std::ostream&
operator<<(std::ostream& o, const oln::point3d& p);


# include <oln/core/point3d.hxx>


#endif // ! OLENA_CORE_POINT3D_HH
