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


# include "../local/scoop.hh"
# include <mlc/int.hh>

namespace oln
{

  //mandatory for using scoop 2 mecanism
  stc_equip_namespace;

  /////////////////////////////////////////////////////////////
  //all Grid class
  template < class Exact >
  struct Grid
  {
  };

  struct grid1d : public Grid< grid1d >
  {
  };

  struct grid2d : public Grid< grid2d>
  {
  };


  //End Grid

  //////////////////////////////////////////////////////////////
  //all Point class


  //Properties list
  stc_decl_associated_type(grid);
  stc_decl_associated_type(dim);
  stc_decl_associated_type(coord);


// # define super      top<Exact>
// # define current    Point<Exact>
// # define templ      template <typename Exact>
// # define classname  Point


  template < typename Exact >
  class Point : public any<Exact>
  {
  public:
    stc_typename(grid);
    stc_typename(coord);
    stc_typename(dim);

    enum { n = mlc_value(dim) };

    typedef Point<Exact> self;


    bool operator==(self const& rhs) const  { return this->exact().impl_egal(rhs.exact()); }
    bool operator<(self const& rhs) const  { return this->exact().impl_inf(rhs.exact()); }
    bool operator!=(self const& rhs) const { return this->exact().impl_diff(rhs.exact()); }
    bool operator>(self const& rhs) const  { return this->exact().impl_sup(rhs.exact()); }
    bool operator>=(self const& rhs) const { return this->exact().impl_sup_egal(rhs.exact()); }
    bool operator<=(self const& rhs) const { return this->exact().impl_inf_egal(rhs.exact()); }
  protected:
    Point() {}
  };

# include "../local/undefs.hh"

  ////

  // concret world


  template <typename C>
  class point2d_;


# define current    point2d_<C>
# define super      top <current>
# define templ      template <typename C>
# define classname  point2d_


  stc_Header;
  typedef stc::is<Point> category;
  typedef grid2d grid;
  typedef C coord;
  typedef mlc::uint_<2> dim;
  stc_End;


  template < typename C >
  class point2d_ : public Point< point2d_<C> >
  {
  public:
    stc_using(grid);
    stc_using(coord);
    stc_using(dim);

    C col;
    C row;

    typedef point2d_<C> self;
    bool impl_egal(self const& rhs) const      { return row == rhs.row and col == rhs.col; }
    bool impl_inf(self const& rhs)  const      { return row < rhs.row or (row == rhs.row and col < rhs.col); }
    bool impl_diff(self const& rhs) const      { return row != rhs.row or col != rhs.col; }
    bool impl_sup(self const& rhs)  const      { return row > rhs.row or (row == rhs.row and col < rhs.col); }
    bool impl_sup_egal(self const& rhs)  const { return row >= rhs.row or (row == rhs.row and col < rhs.col); }
    bool impl_inf_egal(self const& rhs)  const { return row <= rhs.row or (row == rhs.row and col <= rhs.col); }
  };

# include "../local/undefs.hh"


  //End point class
  /////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////
  //Test procedure
  template <typename Exact>
  bool bidon(const Point<Exact>& p1, const Point<Exact>& p2)
  {
    return p1 == p2;
  }

}
#endif /* !POINT_HH_ */
