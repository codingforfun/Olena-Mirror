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

#ifndef OLENA_CORE_POINT1D_HH
# define OLENA_CORE_POINT1D_HH

# include <oln/config/system.hh>
# include <ntg/core/predecls.hh>
# include <oln/core/coord.hh>
# include <oln/core/abstract/point.hh>
# include <iostream>

namespace oln {


  // fwd decl
  class dpoint1d;
  class point1d;

  template<>
  struct point_traits<point1d> : public point_traits<abstract::point<point1d> >
  {
    enum { dim = 1 };
    typedef dpoint1d dpoint_type;
  };

  class point1d : public abstract::point<point1d>
  {
  public:

    typedef abstract::point<point1d>	super_type;
    typedef point_traits<point1d>::dpoint_type dpoint_type;

    friend class super_type;

    point1d();

    point1d(coord col);
    
    coord 
    col() const;
    
    coord& 
    col();
 
    static std::string 
    name() 
    { 
      return "point1d"; 
    }

  protected:

    point1d 
    plus_dp(const dpoint1d& dp) const;
  
    point1d 
    minus_dp(const dpoint1d& dp) const;

    point1d& 
    plus_assign_dp(const dpoint1d& dp);

    point1d& 
    minus_assign_dp(const dpoint1d& dp);

    dpoint1d 
    minus_p(const point1d& p) const;

    point1d 
    minus() const;

  };

  namespace internal
  {
    template<>
    struct default_less<point1d> : public default_less<point1d::super_type>
    {
    };
  } // internal


} // oln

inline std::ostream&
operator<<(std::ostream& o, const oln::point1d& p);

# include <oln/core/point1d.hxx>

#endif // ! OLENA_CORE_POINT1D_HH
