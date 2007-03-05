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

#ifndef POINT_HH_
# define POINT_HH_


# include "forward.hh"
# include <mlc/int.hh>

namespace oln
{
  //my macro for scoop1 paradigm (Point hierachy)
#define stc_simple_typename(Type)     typedef typename vtypes<Exact>::Type Type
#define stc_simple_using(Type, Where) typedef typename vtypes<Where>::Type Type
#define stc_simple_find(Type, Where)  typename vtypes<Where>::Type


  /*! \class Point
  **  \brief abstract point class (at the top of abstract class hierachy)
  **
  ** Abstract Point class
  ** define all base operations of a point\
  */
  template < typename Exact >
  class Point : public any<Exact>
  {
  public:
    stc_simple_typename(grid);	/*!< grid type of a point*/
    stc_simple_typename(coord);	/*!< coord type*/
    stc_simple_typename(dim);	/*!< Point dimension*/

    enum { n = mlc_value(dim) }; /*!< same as dimension*/

    // Point operators
    typedef Point<Exact> self;
    bool operator==(self const& rhs) const  { return this->exact().impl_egal(rhs.exact()); }
    bool operator<(self const& rhs) const   { return this->exact().impl_inf(rhs.exact()); }
    bool operator!=(self const& rhs) const  { return this->exact().impl_diff(rhs.exact()); }
    bool operator>(self const& rhs) const   { return this->exact().impl_sup(rhs.exact()); }
    bool operator>=(self const& rhs) const  { return this->exact().impl_sup_egal(rhs.exact()); }
    bool operator<=(self const& rhs) const  { return this->exact().impl_inf_egal(rhs.exact()); }
    coord operator[](unsigned i) const      { return this->exact().impl_acces(i); }
    coord& operator[](unsigned i)           { return this->exact().impl_acces(i); }
  protected:
    Point() {}
  };

# include "../local/undefs.hh"


  /*! \class point2d_
  **  \brief implementation of Point class for points in two dimension
  **
  ** point2d_ class
  ** implementation of all base operation of a point in two dimension
  */
  template <typename C>
  class point2d_;


  //virtual type of a point 2d
  template<typename C>
  struct vtypes<point2d_< C > >
  {
    typedef stc::is<Point> category;
    typedef mlc::none super_type;
    typedef grid2d grid;
    typedef C coord;
    typedef mlc::uint_<2> dim;
  };


  template < typename C >
  class point2d_ : public Point< point2d_<C> >
  {
  public:
    stc_simple_using(coord, point2d_);

    //ctor
    point2d_() {}
    point2d_(C row, C col) : row_(row), col_(col) {}

    C row_;			/*!< row coordinate */
    C col_;			/*!< col coordinate */

    // implementation of base operations
    typedef point2d_<C> self;

    bool impl_egal(self const& rhs) const      { return row_ == rhs.row_ and col_ == rhs.col_; }
    bool impl_inf(self const& rhs)  const      { return row_ < rhs.row_ or (row_ == rhs.row_ and col_ < rhs.col_); }
    bool impl_diff(self const& rhs) const      { return row_ != rhs.row_ or col_ != rhs.col_; }
    bool impl_sup(self const& rhs)  const      { return row_ > rhs.row_ or (row_ == rhs.row_ and col_ < rhs.col_); }
    bool impl_sup_egal(self const& rhs) const  { return row_ >= rhs.row_ or (row_ == rhs.row_ and col_ < rhs.col_); }
    bool impl_inf_egal(self const& rhs) const  { return row_ <= rhs.row_ or (row_ == rhs.row_ and col_ <= rhs.col_); }

    coord operator[](unsigned i) const         {
      if (i == 0) return  row_;
      return col_;
    }

    coord& operator[](unsigned i)               {
      if (i == 0) return  row_;
      return col_;
    }
  };

  /*!
  ** function which prints a 2d point
  **
  ** @param ostr an ostream
  ** @param p point to print
  **
  ** @return an ostream
  */
  template <typename C>
  std::ostream& operator<<(std::ostream& ostr, const point2d_<C> p)
  {
    return ostr << "p[" << p.row_ << "][" << p.col_ << "]";
  }

  typedef point2d_<int> point2d;

# include "../local/undefs.hh"


  /*! \class point2d_
  **  \brief implementation of Point class for points in one dimension
  **
  ** point1d_ represent point in 1 dimension
  ** implementation of all base operations for point in one dimension
  */
  template <typename C>
  class point1d_;

  // virtual type for point in one dimension
  template<typename C>
  struct vtypes<point1d_< C > >
  {
    typedef stc::is<Point> category;
    typedef mlc::none super_type;
    typedef grid1d grid;
    typedef C coord;
    typedef mlc::uint_<1> dim;
  };

  template < typename C >
  class point1d_ : public Point< point1d_<C> >
  {
  public:
    stc_simple_using(coord, point1d_);

    C col;			/*!< coordinate of point*/

    //implementation of base operations
    typedef point1d_<C> self;
    bool impl_egal(self const& rhs) const      { return col == rhs.col; }
    bool impl_inf(self const& rhs)  const      { return col < rhs.col; }
    bool impl_diff(self const& rhs) const      { return col != rhs.col; }
    bool impl_sup(self const& rhs)  const      { return col < rhs.col; }
    bool impl_sup_egal(self const& rhs) const  { return col < rhs.col; }
    bool impl_inf_egal(self const& rhs) const  { return col <= rhs.col; }
    coord operator[](unsigned i) const         { i = i; return col; }
    coord& operator[](unsigned i)              { i = i; return col; }
  };

  /*!
  ** function which print a 1d point.
  **
  ** @param ostr an ostream
  ** @param p point to print
  **
  ** @return an ostream
  */
  template <typename C>
  std::ostream& operator<<(std::ostream& ostr, const point1d_<C> p)
  {
    return ostr << "p[" << p.col << "]";
  }


  typedef point1d_<int> point1d;

# include "../local/undefs.hh"

  /*!
  ** make a comparaison between two point
  **
  ** @param p1 one point
  ** @param p2 another point
  **
  ** @return 1 if p1 = p2, 0 else
  */
  template <typename Exact>
  bool bidon(const Point<Exact>& p1, const Point<Exact>& p2)
  {
    return p1 == p2;
  }
}

#endif /* !POINT_HH_ */
