// Copyright (C) 2007 EPITA Research and Development Laboratory.
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef POINT_SET_HH_
# define POINT_SET_HH_


# include "forward.hh"
# include <mlc/int.hh>
# include <set>
# include <algorithm>


namespace oln
{



  //abstract world
# define current   Point_Set<Exact>
# define super      any<Exact>
# define templ      template <typename Exact>
# define classname  Point_Set


  /*!
  ** \class Point_set
  ** \brief top abstract class for Point Set hierarchy
  **
  ** Point_Set class define base method and properties of a point set
  ** Methodes:
  ** unsigned int npoint() const: return the number of point that compose the set
  ** bool includes(const point& p) const: Is a point includes in a set?
  ** box bbox() const: return the box of the set
  **
  */
  template <typename Exact>
  struct Point_Set : public super
  {
    stc_typename(point);	/*!< Point type*/
    stc_typename(grid);		/*!< Grid type*/
    stc_typename(box);		/*!< type of box which contains the points */
    stc_typename(iter);		/*!< Point_set iterator type*/


    // based methodes declaration
    unsigned int npoint() const { return this->exact().impl_npoint();  };
    bool includes(const point& p) const { return this->exact().impl_includes(p);  };
    box bbox() const { return this->exact().impl_includes(); }
  };

# include "../local/undefs.hh"

  // concrete world
# define current   point_set_base<Exact>
# define super      top <Exact>
# define templ      template <typename Exact>
# define classname  point_set_base

  stc_Header;
  typedef stc::is<Point_Set> category;
  typedef stc::abstract point;
  typedef stc::abstract grid;
  typedef stc::abstract box;
  typedef stc::abstract iter;
  stc_End;


  /*!
  ** \class point_set_base
  ** \brief top concrete class for Point Set
  ** usefull for factorize code
  */
  template <typename Exact>
  class point_set_base : public super
  {
  public:
    stc_using(point);
    stc_using(box);

  protected:
    point_set_base() {}
  };

# include "../local/undefs.hh"


# define current    pset_std<P>
# define super      point_set_base< current >
# define templ      template <typename P>
# define classname  pset_std

  stc_Header;
  typedef P point;
  typedef stc::final<stc_type(point, grid)> grid;
  typedef current box;
  typedef pset_std_iterator_<P> iter;
  stc_End;

  /*!
  ** \class pset_std
  ** \brief use std set for the representation of the set
  **
  ** Point_Set that use a std set for the representation of the set
  ** P is the type of Points that compose the set
  */
  template <typename P>
  class pset_std : public super
  {
  public:
    stc_using(box);
    stc_using(point);

    point pmin;			/*!< box down left point */
    point pmax;			/*!< box up right point */

    enum { n = mlc_value(typename point::dim) }; /*!< dimension of the box*/

    //ctor
    pset_std(const P& p1, const P& p2) : pmin(p1), pmax(p2) { set_.insert(pmin); set_.insert(pmax); }

    //base methods implementation
    const pset_std& impl_bbox() const { return *this; }
    bool impl_includes(const point& p) const
    {
      for (unsigned i = 0; i < n; ++i)
	if (p[i] < pmin[i] or p[i] > pmax[i])
	  return false;
      return true;
    }
  protected:
    std::set<point> set_;
  };


# include "../local/undefs.hh"


# define current    box_<P>
# define super      point_set_base< current >
# define templ      template <typename P>
# define classname  box_

  stc_Header;
  typedef P point;
  typedef stc::final<stc_type(point, grid)> grid;
  typedef current box;
  typedef box_iterator_<P> iter;
  stc_End;

  /*!
  ** \class box_
  ** \brief box Pset
  **
  ** look like a square in two dimension, cube in three dimension...
  */
  template <typename P>
  class box_: public super
  {
  public:
    stc_using(box);
    stc_using(point);

    enum { n = mlc_value(typename point::dim) }; /*!< dimension of the box*/
    point pmin;			/*!< box down left point */
    point pmax;			/*!< box up right point */

    //ctor
    box_()  {}
    box_(const P& p1, const P& p2) : pmin(p1), pmax(p2) {}

    //implementation of base methodes
    const box_& impl_bbox() const { return *this; }
    bool impl_includes(const point& p) const
    {
      for (unsigned i = 0; i < n; ++i)
	if (p[i] < pmin[i] or p[i] > pmax[i])
	  return false;
      return true;
    }
  };

# include "../local/undefs.hh"

  typedef box_< point2d_<int> > box2d;
  typedef box_< point1d_<int> > box1d;

}

#endif /* !POINT_SET_HH_ */
