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

 
  /*! \class point_traits<point1d>
  **
  ** The specialized version for point1d.
  */ 
  
  template<>
  struct point_traits<point1d> : public point_traits<abstract::point<point1d> >
  {
    enum { dim = 1 };
    typedef dpoint1d dpoint_type;
  };

  /*! \class point1d
  **
  ** Subclass of abstract::point, declaration of point
  ** for image1d. To instantiate a point1d on an
  ** oln::image1d<ntg::rgb_8> for example, use the
  ** macro oln_point_type.\n
  ** oln_point_type(oln::image1d<ntg::rgb_8>) p();\n
  ** or\n
  ** oln_point_type(oln::image1d<ntg::rgb_8>) p(1);
  */
  
  class point1d : public abstract::point<point1d>
  {
  public:

    typedef abstract::point<point1d>	super_type;
    typedef point_traits<point1d>::dpoint_type dpoint_type;

    friend class abstract::point<point1d>;

    point1d();
    
    /// The coordinate of the point1d is set to \a col.
    
    point1d(coord col);
    
    /// Return the value of the point1d coordinate.
    
    coord 
    col() const;
    
    /// Return a reference to the point1d coordinate.

    coord& 
    col();
 
    static std::string 
    name() 
    { 
      return "point1d"; 
    }

  protected:

    
    /*! \brief Return a point1d whose coordinate is equal to 
    ** \a dp coordinate plus the current point1d coordinate.
    */
    
    point1d 
    plus_dp(const dpoint1d& dp) const;
  
    /*! \brief Return a point1d whose coordinate is equal to
    ** the current point1d coordinate minus \a dp coordinate.
    */
    
    point1d 
    minus_dp(const dpoint1d& dp) const;
    
    /*! \brief Return a reference to the current point1d 
    ** plus \a dp.
    */
    
    point1d& 
    plus_assign_dp(const dpoint1d& dp);

    /*! \brief Return a reference to the current point1d
    ** minus \a dp.
    */
    
    point1d& 
    minus_assign_dp(const dpoint1d& dp);
    
    /*! \brief Return a dpoint1d whose coordinate is equal
    ** to the current point1d coordinate minus \a p coordinate.
    */
    
    dpoint1d 
    minus_p(const point1d& p) const;

    /*! \brief Return a point1d whose coordinate is equal to 
    ** the opposite of the current point1d coordinate.
    */ 
    
    point1d 
    minus() const;

  };
    
  namespace internal
  {
    
    /*! \class default_less<point1d>
    ** 
    ** The specialized version for point1d.
    */
    
    template<>
    struct default_less<point1d> : public default_less<point1d::super_type>
    {
    };
  } // internal


} // oln

/// Write on an output stream \a o the coordinate of the point1d \a p.

inline std::ostream&
operator<<(std::ostream& o, const oln::point1d& p);

# include <oln/core/point1d.hxx>

#endif // ! OLENA_CORE_POINT1D_HH
