// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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


# include "point_nd.hh"

namespace oln {

  template <unsigned Dim>
  struct point;//fwd decl

  /*! \class Concrete point 3.
  **
  ** Provides syntactic sugar.
  */
  template <>
  struct point<3>: public abstract::point_nd<point<3> >
  {
    point()
    {}
    point(coord slice, coord row, coord col)
    {
      coord_[0] = slice;
      coord_[1] = row;
      coord_[2] = col;
    }

    coord slice() const		{ return coord_[0]; }
    coord& slice()		{ return coord_[0]; }

    coord row() const		{ return coord_[1]; }
    coord& row() 		{ return coord_[1]; }

    coord col() const		{ return coord_[2]; }
    coord& col() 		{ return coord_[2]; }

    static std::string
    name()
    {
      return "point<3>";
    }
  };

  /// Depreciated. Use point<3> instead.
  typedef point<3> point3d;
} // end of oln

#endif // ! OLENA_CORE_POINT3D_HH
