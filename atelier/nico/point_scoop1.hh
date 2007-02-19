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

namespace oln_point
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



#define stc_simple_typename(Type)  typedef typename vtypes<Exact>::Type Type


  template < typename Exact >
  class Point : public any<Exact>
  {
  public:
    stc_simple_typename(grid);
    stc_simple_typename(coord);
    stc_simple_typename(dim);

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


  template <typename C>
  class point2d_;


  template<typename C>
  struct vtypes<point2d_< C > >
  {
    typedef grid2d grid;
    typedef C coord;
    typedef mlc::uint_<2> dim;
  };

  template < typename C >
  class point2d_ : public Point< point2d_<C> >
  {
  public:

    C col;
    C row;

    typedef point2d_<C> self;
    bool impl_egal(self const& rhs) const      { return row == rhs.row and col == rhs.col; }
    bool impl_inf(self const& rhs)  const      { return row < rhs.row or (row == rhs.row and col < rhs.col); }
    bool impl_diff(self const& rhs) const      { return row != rhs.row or col != rhs.col; }
    bool impl_sup(self const& rhs)  const      { return row > rhs.row or (row == rhs.row and col < rhs.col); }
    bool impl_sup_egal(self const& rhs) const  { return row >= rhs.row or (row == rhs.row and col < rhs.col); }
    bool impl_inf_egal(self const& rhs) const  { return row <= rhs.row or (row == rhs.row and col <= rhs.col); }
  };

# include "../local/undefs.hh"


  //End point class
  /////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////
  //Point Test procedure
  template <typename Exact>
  bool bidon(const Point<Exact>& p1, const Point<Exact>& p2)
  {
    return p1 == p2;
  }

  ////////////////////////////////////////////////////////////
  //Iterator

  // abstract world


  //Properties
  stc_decl_associated_type(value);
  stc_decl_associated_type(point);


  template <typename Exact>
  struct Iterator : public any <Exact>
  {
    stc_typename(value);

    void start() { this->exact().impl_start(); }
    void next() { this->exact().impl_next(); }
    void invalidate() { this->exact().impl_invalidate(); }
    bool is_valid() const { return this->exact().impl_is_valid(); }
  };

# include "../local/undefs.hh"

  template <typename Exact>
  struct Iterator_on_Points;


# define current   Iterator_on_Points<Exact>
# define super     Iterator <Exact>
# define templ      template <typename Exact>
# define classname  Iterator_on_Points





  template <typename Exact>
  struct Iterator_on_Points : public super
  {
    stc_typename(point);

    typedef Iterator_on_Points<Exact> self;

    operator stc_type(Exact, point)() const { return this->exact().impl_cast(); }
    stc_type(Exact, point) to_point() const { return this->exact().impl_to_point(); }
    stc_type(Exact, point) const* point_adr() const { return this->exact().impl_point_adr(); }
  };

# include "../local/undefs.hh"


  // Concret world


# define current    simple_iter<C>
# define super      top <current>
# define templ      template <typename C>
# define classname  simple_iter


  stc_Header;
  typedef stc::is<Iterator> category;
  typedef C value;
  typedef point2d_<C> point;
  stc_End;


  template < typename C >
  class simple_iter : public top < simple_iter<C> >
  {
  public:
    //FIX IT
//     stc_using(point);
    typedef point2d_<C> point;

    simple_iter(const point& init) : i_(0)
    {
      for (int i = 0; i < 15; ++i)
	v_[i] = init;
    }

    void impl_start() { i_ = 0; }
    void impl_next() { ++i_; }
    void impl_invalidate() { i_ = 42; }
    bool impl_is_valid() const { return i_ < 15; }
    point impl_to_point() const { return (point) *this; }
    point const* point_adr() const { return v_; }

    operator point const()
    {
      if (i_ < 16)
	return v_[i_];
      return (v_[0]);
    }

  protected:
    int		  i_;
    point v_[15];
  };

# include "../local/undefs.hh"

  //End Iterator
  /////////////////////////////////////////////////////

  ////////////////////////////////////////////////////
  /// start box



  template <typename P>
  class box_: any< box_<P> >
  {
  public:
    P pmin;
    P pmax;

  };



  template <typename P>
  class box2d: public box_< P >
  {
  public:
    box2d(int i, int j) : row_max_(i), col_max_(j)
    {
      tab_ = new point2d_<P>* [i];
      for (int k = 0; k < j; ++k)
	tab_[k] = new point2d_<P> [j];
    }
    ~box2d()
    {
      for (int k = 0; k < row_max_; ++k)
	delete[] tab_[k];
      delete[] tab_;
    }

    void set(int i,  int j, point2d_<P> new_point)
    {
      if (i < row_max_ && j < col_max_)
	tab_[i][j] = new_point;
      if (i == 0 && j == 0)
	box_< point2d_<P> >::pmin = tab_[0][0];
      if (i == row_max_ - 1 && j == col_max_ - 1)
	box_< point2d_<P> >::pmax = tab_[i - 1][j - 1];
    }
    point2d_<P>& get(int i, int j) { return tab_[i][j]; }
  protected:
    point2d_<P> **tab_;
    int row_max_;
    int col_max_;
  };

  /// end box
  ///////////////////////////////////////////////////


}
#endif /* !POINT_HH_ */
