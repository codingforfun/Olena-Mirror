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

#ifndef POINT_SCOOP1_HH_
# define POINT_SCOOP1_HH_

# include <mlc/int.hh>
# include <cassert>
# include <set>
# include <algorithm>
# include <iostream>

# include "../local/scoop.hh"



# include "init_oln_point.hh"
# include "forward.hh"
# include "plug.hh"
# include "tools.hh"
# include "impl_deleg.hh"
# include "array.hh"
# include "tracked_ptr.hh"


namespace oln
{

  //macro for scoop1 paradigm
#define stc_simple_typename(Type)     typedef typename vtypes<Exact>::Type Type
#define stc_simple_using(Type, Where) typedef typename vtypes<Where>::Type Type
#define stc_simple_find(Type, Where)  typename vtypes<Where>::Type




  /////////////////////////////////////////////////////////////
  //all Grid class
  template < class Exact >
  struct Grid
  {
  };

  struct grid1d : public Grid< grid1d >
  {
  };

  struct grid2d : public Grid< grid2d >
  {
  };


  //End Grid
  ////////////////////////////////////////////////////////////
  //all Poin class




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


  template <typename C>
  class point2d_;


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

    point2d_() {}
    point2d_(C row, C col) : row_(row), col_(col) {}

    C row_;
    C col_;

    typedef point2d_<C> self;


    //    point2d_(const point2d_<C> &rhs) { row_ = rhs.row_; col_ =  rhs.col_; }

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

  template <typename C>
  std::ostream& operator<<(std::ostream& ostr, const point2d_<C> p)
  {
    return ostr << "p[" << p.row_ << "][" << p.col_ << "]";
  }

  typedef point2d_<int> point2d;

# include "../local/undefs.hh"

  template <typename C>
  class point1d_;


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

    C col;

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

  template <typename C>
  std::ostream& operator<<(std::ostream& ostr, const point1d_<C> p)
  {
    return ostr << "p[" << p.col << "]";
  }


  typedef point1d_<int> point1d;

# include "../local/undefs.hh"


  /////////////////////////////////////////////////////////////
  //Point Test procedure
  template <typename Exact>
  bool bidon(const Point<Exact>& p1, const Point<Exact>& p2)
  {
    return p1 == p2;
  }
  //
  ///////////////////////////////////////////////////////////

  //End point class
  /////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////
  // Point set class

  //abstract world

# define current   Point_Set<Exact>
# define super      any<Exact>
# define templ      template <typename Exact>
# define classname  Point_Set

  template <typename Exact>
  struct Point_Set : public super
  {
    stc_typename(point);
    stc_typename(grid);
    stc_typename(box);


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
  stc_End;


  template <typename Exact>
  class point_set_base : public super
  {
  public:
    stc_using(point);
    stc_using(box);


    // unsigned int npoint() const { return this->exact().impl_npoint();  };
//     bool includes() const { return this->exact().impl_includes();  };
//     box bbox () const  { return this->exact().impl_includes(); }
  protected:
    point_set_base() {}
  };


# include "../local/undefs.hh"
  // End Point class
  //////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////
  //Iterator

  // abstract world
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
  struct Iterator_on_Pointss;


# define current   Iterator_on_Points<Exact>
# define super     Iterator <Exact>
# define templ      template <typename Exact>
# define classname  Iterator_on_Points





  template <typename Exact>
  struct Iterator_on_Points : public super
  {
    stc_typename(point);

    typedef Iterator_on_Points<Exact> self;

    operator point() const { return this->exact().impl_cast(); }
    point to_point() const { return this->exact().impl_to_point(); }
    point const* point_adr() const { return this->exact().impl_point_adr(); }
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
  typedef oln::point2d_<C> point;
  stc_End;


  template < typename C >
  class simple_iter : public super
  {
  public:
    typedef stc_type(simple_iter<C>, point) point;

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

  template <typename P>
  class box_;



# define current    box_iterator_<P>
# define super      top  <current>
# define templ      template <typename P>
# define classname  box_iterator_



  stc_Header;
  typedef stc::is<Iterator_on_Points> category;
  typedef P point;
  typedef P value;
  stc_End;


  template <typename P>
  class box_;


  template < typename P >
  class box_iterator_ : public super
  {
  public:
    typedef P point;

    box_iterator_(const box_<point>& box) : bb_(box)
    {
      nop_ = box.pmax;
      ++nop_[0];
    }

    void impl_start() { std::cout << "start: box min:" << bb_.pmin << std::endl; p_ = bb_.pmin; }
    void impl_next();
    void impl_invalidate() { p_ = nop_; }
    bool impl_is_valid() const { return p_ != nop_; }
    point impl_to_point() const { return p_; }
    point const* point_adr() const { return &p_; }
    operator point const() { return p_; }

  protected:
    point p_;
    point nop_;
    const box_<point> &bb_;
  };



  //// methode implementation
  template < typename P >
  void box_iterator_< P >::impl_next()
  {
    typedef typename box_iterator_<P>::point point;
    std::cout << "next: box min:" << bb_.pmin << " cur: " << p_ << std::endl;
    for (int i = point::n - 1; i >= 0; --i)
      if (p_[i] == bb_.pmax[i])
      	p_[i] = bb_.pmin[i];
      else
      {
	++p_[i];
	break;
      }

    if (p_ == bb_.pmin)
      p_ = nop_;
  }

  typedef box_iterator_< point2d_<int> > iter2d;
  typedef box_iterator_< point1d_<int> > iter1d;

# include "../local/undefs.hh"


  template <typename P>
  class pset_std;

# define current    pset_std_iterator_<P>
# define super      top  <current>
# define templ      template <typename P>
# define classname  pset_std_iterator_

  stc_Header;
  typedef stc::is<Iterator> category;
  typedef P point;
  typedef P value;
  stc_End;

  template < typename P >
  class pset_std_iterator_ : public super
  {
  public:
    typedef stc_type(pset_std_iterator_, point) point;

    pset_std_iterator_(std::set<P> &box) : bb_(box) {}

    void impl_start() { iter = bb_.begin(); }
    void impl_next() { ++iter;  }
    void impl_invalidate() { iter = bb_.end(); }
    bool impl_is_valid() const { return *iter != bb_.end(); }
    point impl_to_point() const { return *iter; }
    point const* point_adr() const { return iter; }

  protected:
    typename std::set<P> &bb_;
    typename std::set< P >::iterator iter;
  };


# include "../local/undefs.hh"

//End Iterator
//////////////////////////////////////////////////



//////////////////////////////////////////////////
/// set

# define current    pset_std<P>
# define super      point_set_base< current >
# define templ      template <typename P>
# define classname  pset_std

  stc_Header;
  typedef P point;
  typedef stc::final<stc_type(point, grid)> grid;
  typedef current box;
  stc_End;

  template <typename P>
  class pset_std : public super
  {
  public:
    stc_using(box);
    stc_using(point);
    point pmin;
    point pmax;

    enum { n = mlc_value(typename point::dim) };


    pset_std(const P& p1, const P& p2) : pmin(p1), pmax(p2) { set_.insert(pmin); set_.insert(pmax); }
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


//// start box


# define current    box_<P>
# define super      point_set_base< current >
# define templ      template <typename P>
# define classname  box_

  stc_Header;
  typedef P point;
  typedef stc::final<stc_type(point, grid)> grid;
  typedef current box;
  stc_End;

  template <typename P>
  class box_: public super
  {
  public:
    stc_using(box);
    stc_using(point);

    enum { n = mlc_value(typename point::dim) };
    const point pmin;
    const point pmax;

    box_(const P& p1, const P& p2) : pmin(p1), pmax(p2) {  }
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


  /// end set
  ////////////////////////////////////////////////

  ///////////////////////////////////////////////
  // Image

  // Properties

  // Abstract world

# define current    Image<Exact>
# define super      any<Exact>
# define templ      template <typename Exact>
# define classname  Image

  template <typename Exact>
  struct Image : public virtual super, automatic::get_impl<Image, Exact>
  {
    stc_typename(point);
    stc_typename(value);
    stc_typename(box);
    stc_typename(iter);
    stc_typename(grid);
    stc_typename(data);
    stc_typename(rvalue);
    stc_typename(psite);

    rvalue operator() (const psite& p) const
    {
      assert(owns_(p));
      return this->exact().impl_rvalue_access(p);
    }
    bool owns_(const psite& p) const { return this->exact().impl_owns(p); }
    bool has_data() const { return this->exact().impl_has_data(); }
    box bbox() const { return this->exact().impl_bbox(); }
  };

# include "../local/undefs.hh"


# define current    Signal<Exact>1
# define super      Image<Exact>
# define templ      template <typename Exact>
# define classname  Signal

  template <typename Exact>
  struct Signal : public virtual super, automatic::get_impl<Signal, Exact> {};

# include "../local/undefs.hh"

# define current    Image2d<Exact>
# define super      Image<Exact>
# define templ      template <typename Exact>
# define classname  Image2d

  template <typename Exact>
  struct Image2d : public virtual super, automatic::get_impl<Image2d, Exact>
  {
    stc_typename(coord);
  };

# include "../local/undefs.hh"


# define current    Mutable_Image<Exact>
# define super      Image<Exact>
# define templ      template <typename Exact>
# define classname  Mutable_Image

  template <typename Exact>
  struct Mutable_Image : public virtual super, automatic::get_impl<Mutable_Image, Exact>
  {
    stc_using(psite);
    stc_typename(lvalue);

    lvalue operator() (const psite& p)
    {
      assert(owns_(p));
      return this->exact().impl_lvalue_access(p);
    }
  };

# include "../local/undefs.hh"

# define current    Point_Wise_Accessible_Image<Exact>
# define super      Image<Exact>
# define templ      template <typename Exact>
# define classname  Point_Wise_Accessible_Image

  template <typename Exact>
  struct Point_Wise_Accessible_Image : public virtual super, automatic::get_impl<Point_Wise_Accessible_Image, Exact>
  {
    stc_using(point);

    bool has(const point& pt) { return this->exact().impl_has(pt); }
  };

# include "../local/undefs.hh"
  // Concret World

# define current    image_base<Exact>
# define super      top <Exact>
# define templ      template <typename Exact>
# define classname  image_base

  stc_Header;
  typedef stc::is<Image> category;
  typedef stc::abstract point;
  typedef stc::abstract grid;
  typedef stc::abstract box;
  typedef stc::abstract data;
  stc_End;


  template <typename Exact>
  class image_base : public super
  {
  public:
    stc_using(point);
    stc_using(value);
    stc_using(box);
    stc_using(iter);
    stc_using(grid);

    stc_lookup(data);
    typedef data data_t;
    bool impl_has_data() const { return data_ != 0; }

  protected:
    image_base() { data_ = 0; }
    internal::tracked_ptr<data_t> data_;
  };

# include "../local/undefs.hh"


# define current    primitive_image<Exact>
# define super      image_base <Exact>
# define templ      template <typename Exact>
# define classname  primitive_image

  stc_Header;
  stc_End;


  template <typename Exact>
  class primitive_image : public super
  {
  public:
//     stc_using(data);
//     stc_using(point);
//     stc_using(value);
//     stc_using(box);
//     stc_using(iter);
//     stc_using(grid);
//    stc_using(coord);

  protected:
    primitive_image() {}
  };

# include "../local/undefs.hh"

# define classname  image_morpher
# define current    image_morpher<Exact>
# define super      image_base <Exact>
# define templ      template <typename Exact>

  stc_Header;
  typedef stc::abstract delegatee;
  typedef stc::not_delegated data;
  stc_End;

  template <typename Exact>
  class classname : public super
  {
  public:
    stc_typename(delegatee);
  protected:
    image_morpher() {}
  };

# include "../local/undefs.hh"



# define classname  single_image_morpher
# define current    single_image_morpher<Exact>
# define super      image_morpher<Exact>
# define templ      template <typename Exact>

  stc_Header;
  stc_End;

  template <typename Exact>
  class single_image_morpher : public super
  {
  public:
    stc_using(delegatee);

    delegatee& image() { assert(this->has_data());
      return this->exact().impl_image(); }
    delegatee image() const { assert(this->has_data());
    return this->exact().impl_image(); }
  };

# include "../local/undefs.hh"


# define classname  image_extansion
# define current    image_extansion<Exact>
# define super      single_image_morpher<Exact>
# define templ      template <typename Exact>

  stc_Header;
  typedef behavior::identity behavior;
  stc_End;

  template <typename Exact>
  struct image_extansion : public super
  {
    stc_typename(behavior);
  };

# include "../local/undefs.hh"




# define classname  multiple_image_morpher
# define current    multiple_image_morpher<Exact>
# define super      image_morpher<Exact>
# define templ      template <typename Exact>

  stc_Header;
  stc_End;

  template <typename Exact>
  class classname : public super
  {
  public:
    stc_using(delegatee);

    delegatee& image(unsigned i) { assert(this->has_data() && i < n);
      return this->exact().impl_image(); }
    delegatee image(unsigned i) const {assert(this->has_data() && i < n);
      return this->exact().impl_image(); }
  protected:
    unsigned n;
  };

# include "../local/undefs.hh"

# define classname  polite_image
# define current    polite_image<I>
# define super      image_extansion<current>
# define templ      template <typename I>

  stc_Header;
  typedef I delegatee;
  typedef singleton<I> data;
  stc_End;

  template <typename I>
  class polite_image : public super
  {
  public:
    stc_using(delegatee);
    stc_using(data);

    polite_image(I& ima) : delegatee_(ima) { this->data_ = new data (ima); }

    void talk() const { std::cout << "Nice" << std::endl; }

    delegatee& impl_image() { return this->data_->value_; }
    delegatee impl_image() const { return this->data_->value_; }
  protected:
    delegatee& delegatee_;
  };

# include "../local/undefs.hh"

# define current    image2d<T>
# define super      primitive_image<current>
# define templ      template <typename T>
# define classname  image2d

  stc_Header;
  // typedef stc::is<Image2d> category;
  typedef iter2d iter;
  typedef point2d point;
  typedef point::coord coord;
  typedef point psite;
  typedef array2d<T, coord>  data;
  typedef T value;
  typedef const value& rvalue;
  typedef value& lvalue;
  typedef grid2d grid;
  typedef box2d box;
  stc_End;


  template <typename T>
  class image2d : public super
  {
  public:
    stc_using(point);
    stc_using(value);
    stc_using(box);
    stc_using(iter);
    stc_using(grid);
    stc_using(coord);
    stc_using(data);
    stc_using(rvalue);
    stc_using(lvalue);
    stc_using(psite);

    image2d(box &box_ref) : bb_(box_ref) {
      this->data_ = new data (box_ref.pmin.row_, box_ref.pmin.col_, box_ref.pmax.row_, box_ref.pmax.col_); }

    bool impl_owns(const psite& p) const { return  bb_.includes(p); }
    bool impl_has(const point& p) { return  bb_.includes(p); }
    lvalue impl_lvalue_access(const psite& p) { return (this->data_.ptr_)->operator()(p.row_, p.col_); }
    rvalue impl_rvalue_access(const psite& p) const  { return (this->data_.ptr_)->operator()(p.row_, p.col_); }
    box impl_bbox() const {return bb_;}
  protected:
    box &bb_;
  };

# include "../local/undefs.hh"


  //break, don't use this class
# define current    signal<T>
# define super      primitive_image<current>
# define templ      template <typename T>
# define classname  signal

  stc_Header;
  typedef T value;
  typedef const value& rvalue;
  typedef value& lvalue;
  typedef iter1d iter;
  typedef T*  data;
  typedef point1d point;
  typedef point psite;
  typedef point::coord coord;
  typedef grid1d grid;
  typedef box1d box;
  stc_End;

  template <typename T>
  class signal : public super
  {
  public:
    stc_using(point);
    stc_using(value);
    stc_using(box);
    stc_using(iter);
    stc_using(grid);
    stc_using(psite);

    signal(box &box_ref) : bb_(box_ref) { }

    bool imp_owns(const psite& p) const  { return  bb_.includes(p); }
    //value imp_value_acces(const point& p) { return data_[p.col]; }
    box impl_bbox() const {return bb_;}
  protected:
    box &bb_;
  };

  template <typename I>
  void print(const Image<I>& ima)
  {
    typename I::iter it (ima.bbox());

    for (it.start(); it.is_valid(); it.next())
    {
      std::cout << (typename I::point) it << std::endl;
      std::cout << ima.operator()(it.to_point()) << std::endl;
    }
  }

# include "../local/undefs.hh"

  // End Image
  //////////////////////////////////////////////
}

#endif /* !POINT_SCOOP1_HH_ */
