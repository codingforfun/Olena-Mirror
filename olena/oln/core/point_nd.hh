// Copyright (C) 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_POINT_HH
# define OLENA_CORE_POINT_HH

# include "abstract/point.hh"

namespace oln {

  /// \class Concrete point for dimension grater than 3.
  template <unsigned Dim>
  struct point : public abstract::point_nd<point<Dim> >
  {

    //FIXME: to be complete
    static std::string
    name()
    {
      std::ostringstream s;
      s << Dim;
      return std::string("point<") + s.str() + ">";
    }

  };

  template <unsigned Dim>
  struct dpoint; //fwd decl


  template <unsigned Dim>
  struct prop<point<Dim> >
  {
    enum {dim = Dim};
    typedef dpoint<Dim> dpoint_type;
  };

} // oln

#endif // ! OLENA_CORE_POINT_HH
