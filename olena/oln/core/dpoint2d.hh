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

#ifndef OLENA_CORE_DPOINT2D_HH
# define OLENA_CORE_DPOINT2D_HH

# include <oln/core/coord.hh>
# include <oln/core/dpointnd.hh>
# include <iostream>

namespace oln {

  // fwd decl
  class point2d;

  class dpoint2d : public dpointnd< 2, dpoint2d >
  {
  public:
    typedef dpointnd< 2, dpoint2d > super;

    dpoint2d();
    dpoint2d(coord row, coord col);
    explicit dpoint2d(const point2d& p);

    coord row() const;
    coord& row();
    coord col() const;
    coord& col();

    dpoint2d operator+(const dpoint2d& dp) const;
    dpoint2d operator-() const;
    dpoint2d operator-(const dpoint2d& dp) const;
    dpoint2d& operator+=(const dpoint2d& dp);
    dpoint2d& operator-=(const dpoint2d& dp);

    static std::string name() { return "dpoint2d"; }
  };

  _DPointForDim(2, dpoint2d);

  namespace internal
  {
    template<>
    struct default_less<dpoint2d> :
      public default_less<dpoint2d::super>
    {};
  } // end of internal

} // end of oln

inline std::ostream&
operator<<(std::ostream& o, const oln::dpoint2d& p);

# include <oln/core/dpoint2d.hxx>

#endif // OLENA_CORE_DPOINT2D_HH
