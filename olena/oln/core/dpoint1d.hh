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

#ifndef OLENA_CORE_DPOINT1D_HH
# define OLENA_CORE_DPOINT1D_HH

# include <oln/config/system.hh>
# include <ntg/predecls.hh>
# include <oln/core/coord.hh>
# include <oln/core/dpointnd.hh>
# include <iostream>

namespace oln {

  // fwd decl
  class point1d;

  class dpoint1d : public dpointnd< 1, dpoint1d>
  {
  public:
    typedef dpointnd< 1, dpoint1d> super;

    dpoint1d();
    dpoint1d(coord col);
    explicit dpoint1d(const point1d& p);

    coord col() const;
    coord& col();

    dpoint1d operator+(const dpoint1d& dp) const;
    dpoint1d operator-() const;
    dpoint1d operator-(const dpoint1d& dp) const;
    dpoint1d& operator+=(const dpoint1d& dp);
    dpoint1d& operator-=(const dpoint1d& dp);

    static std::string name() { return "dpoint1d"; }
  };

  _DPointForDim(1, dpoint1d);

  namespace internal
  {

    template<>
    struct default_less<dpoint1d> :
      public default_less<dpoint1d::super>
    {
    };

  } // end of internal
} // end of oln

inline std::ostream&
operator<<(std::ostream& o, const oln::dpoint1d& p);

# include <oln/core/dpoint1d.hxx>

#endif // ! OLENA_CORE_DPOINT1D_HH
