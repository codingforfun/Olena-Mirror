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

#ifndef OLENA_CORE_POINT1D_HH
# define OLENA_CORE_POINT1D_HH

# include "point_nd.hh"

namespace oln {

  template <unsigned Dim>
  struct point;//fwd decl

 /*! \class Concrete point 1.
  **
  ** Provides syntactic sugar.
  */
  template <>
  struct point<1>: public abstract::point_nd<point<1> >
  {
    point()
    {}
    point(coord col)
    {
      coord_[0] = col;
    }

    coord col() const		{ return coord_[0]; }
    coord& col()		{ return coord_[0]; }
    static std::string
    name()
    {
      return "point<1>";
    }
  };

/// Depreciated. Use point<1> instead.
  typedef point<1> point1d;
} // oln

#endif // ! OLENA_CORE_POINT1D_HH
