#ifndef GRID_HH_
# define GRID_HH_

# include <set>
# include <mlc/int.hh>
# include <mlc/uint.hh>
# include "../local/scoop.hh"

namespace glg
{
  stc_equip_namespace;

  // ==============
  // ABSTRACT TYPES
  // ==============

  stc_decl_associated_type(grid);
  stc_decl_associated_type(coord);
  stc_decl_associated_type(dim);
  stc_decl_associated_type(point);
  stc_decl_associated_type(value);
  stc_decl_associated_type(box_);

  // ====================
  // FORWARD DECLARATIONS
  // ====================

  template <typename Exact> class Grid;
  class grid1d;
  class grid2d;
  template < typename P > class box_;
  template <typename Exact> class Point;
  template <typename C> class point2d_;
  template < typename Exact > class Iterator_on_Points;
  template < typename P > class pset_std_iterator_;

  // ========
  // CONCEPTS
  // =======-

  // Grids
  // -----

  template <typename Exact>
  class Grid : public stc::any <Exact>
  {};

  // Points
  // ------

  template <typename Exact>
  class Point : public any < Exact >
  {
  public :
    stc_typename(grid);
    stc_typename(coord);
    stc_typename(dim);

    enum { n = mlc_value(dim) };

    typedef Point<Exact> self;

    bool operator==(self const& rhs) const { return this->exact().impl_eq(rhs.exact()); }
    bool operator<(self const& rhs) const { return this->exact().impl_lt(rhs.exact()); }
    bool operator!=(self const& rhs) const { return this->exact().impl_ne(rhs.exact()); }
    bool operator>(self const& rhs) const { return this->exact().impl_gt(rhs.exact()); }
    bool operator>=(self const& rhs) const { return this->exact().impl_ge(rhs.exact()); }
    bool operator<=(self const& rhs) const { return this->exact().impl_le(rhs.exact()); }
    coord operator[](unsigned i) const { return this->exact().impl_acc(i); }
    coord operator[](unsigned i) { return this->exact().impl_acc(i); }

  protected:
    Point() {}
  };


  // Iterators
  // ---------

  template < typename Exact >
  class Iterator : public any < Exact >
  {
  public:
    void start ();
    void next ();
    void invalidate ();
    bool is_valid () const;
  protected:
    Iterator();

  };


  // Iterator on points
  // ------------------

  template < typename Exact >
  class Iterator_on_Points : public any < Exact >
  {
  public:
    stc_typename(point);
    operator point() const { return this->exact().impl_cast(); }
    point& to_point() const { return (point)(this); }
    point const* point_adr () const { return this->exact().impl_point_adr(); }
  };


  // Point set
  // ---------

   template < typename Exact >
   class Point_Set : public any < Exact >
   {
   public:
     stc_typename(point);
     stc_typename(grid);
     typedef Iterator_on_Points<point> iter;
     typedef box_<point> box;

     unsigned npoints () const { return this->exact().impl_npoints; }
     bool includes (point const& p) const { return this->exact().impl_includes(p); }
   };

  // Image
  // -----

  template < typename Exact >
  class Image : public any < Exact >
  {
    stc_typename(point);
    stc_typename(value);
    typedef box_<point> box;
    typedef Iterator_on_Points<point> iter;

    value operator() (point const& p) const;
    bool owns (point const& p) const;
    box bbox() const;
  };

  // Image_2D
  // --------

  // Ici on ne definit que les types et les proto.
  // On va faire une class concrete de base pour les preconditions etc...
  // C'est une 1ere idee, je vais aller voir comment c'est fait dans Olena
  // * quelques minutes plus tard *
  // Bon, j'ai pas trop compris comment fait Olena, c'est plein de macros oln_*
  // On va intuiter...

  // Bon la classe concrete image2d doit deriver de top...
  // Adieu le image_base_ donc...

  template < typename Exact >
  class Image_2D : public Image < Exact >
  {
    stc_typename(point);
    stc_typename(value);
    stc_typename(coord);
    value operator() (coord row, coord col);
  };

# include "../local/undefs.hh"

  // ===============
  // IMPLEMENTATIONS
  // ===============

  // Grids
  // -----

  class grid1d : public Grid <grid1d>
  {};

  class grid2d : public Grid <grid2d>
  {};



  // Points
  // ------

# define templ template <typename C>
# define classname point2d_
# define current point2d_<C>
# define super top< current >

  stc_Header;
  typedef stc::is<Point> category;
  typedef grid2d grid;
  typedef C coord;
  typedef mlc::uint_<2> dim;
  stc_End;

  template <typename C>
  class point2d_ : public super
  {
  public:
    stc_using(grid);
    stc_using(coord);
    stc_using(dim);

    point2d_ () {}
    point2d_ (int x, int y) : row(x), col(y) {}

    C row, col;

    bool impl_eq(point2d_<C> const& rhs) const { return col == rhs.col and row == rhs.row; }
    bool impl_lt(point2d_<C> const& rhs) const { return col < rhs.col and row < rhs.row; }
    bool impl_ne(point2d_<C> const& rhs) const { return !(col == rhs.col) and !(row == rhs.row); }
    bool impl_gt(point2d_<C> const& rhs) const { return col > rhs.col and row > rhs.row; }
    bool impl_ge(point2d_<C> const& rhs) const { return col >= rhs.col and row >= rhs.row; }
    bool impl_le(point2d_<C> const& rhs) const { return col <= rhs.col and row <= rhs.row; }

    coord operator[](unsigned i) const { return i?col:row; }
    coord& operator[](unsigned i) { return i?col:row; }
  };

# include "../local/undefs.hh"



  // Dummy iterator
  // --------------

# define templ template < typename C >
# define classname dummy_iterator_on_point2d
# define current dummy_iterator_on_point2d< C >
# define super Iterator_on_Points< current >

  stc_Header;
  typedef point2d_<C> point;
  stc_End;

  template < typename C >
  class dummy_iterator_on_point2d : public super
  {
    stc_using(point);
    point& impl_cast() { return point_; }
    point const* point_adr () const { return &point_; }

    void start () { };
    void next () { };
    void invalidate () { };
    bool is_valid () const { return false; };


  private:
    point2d_< C > point_;
  };

# include "../local/undefs.hh"


  // Iterator on box
  // ---------------

# define templ template < typename P >
# define classname box_iterator_
# define current box_iterator_ <P>
# define super top< current >

  stc_Header;
  typedef stc::is<Iterator_on_Points> category;
  typedef P point;
  stc_End;

  template < typename P >
  class box_iterator_ : public super
  {
  public:
    stc_using(point);
    point impl_cast() { return point_; }
    point const* point_adr () const { return &point_; }

    void start () { point_ = bb_.pmin(); }
    void next () {
      for (int i = point::n - 1; i >= 0; --i)
	if (point_[i] == bb_.pmax(i))
	  point_[i] = bb_.pmin(i);
	else
	  {
	    ++point_[i];
	    break;
	  }
      if (point_ == bb_.pmin())
	point_ = nop_;
    }
    void invalidate () { point_ = nop_; }
    bool is_valid () const { return point_ != nop_; }

    box_iterator_(const box_<point>& bb) : point_(), bb_(bb) { nop_ = bb_.pmax; ++nop_[0];}

  private:
    point point_;
    box_<point>  bb_;
    point nop_;
  };

# include "../local/undefs.hh"

  // Point set base
  // --------------

# define templ template < typename Exact >
# define classname point_set_base_
# define current point_set_base_< Exact >
# define super top< Exact >

  stc_Header;
  typedef stc::is<Point_Set> category;
  typedef stc::abstract point;
  typedef stc_deferred(point) point__;
  typedef stc_type(point__, grid) grid;
  stc_End;


  template < typename Exact >
  class point_set_base_ : public super
  {
  };


# include "../local/undefs.hh"


  // Point set std
  // -------------

# define templ template < typename P >
# define classname pset_std_
# define current pset_std_< P >
# define super point_set_base_< current >

  stc_Header;
  typedef P point;
  stc_End;

  template < typename P >
  class pset_std_ : public super
  {
  public:
    stc_using(point);
    stc_using(grid);

    typedef pset_std_iterator_< P > iter;
    unsigned npoints () const { return set_.size(); }
    bool includes (point const& p) const { return 1; }

    pset_std_() : set_() {}
  private:
    typename std::set<P> set_;
  };

# include "../local/undefs.hh"

  // pset_std_iterator_
  // ------------------

# define templ template < typename P >
# define classname pset_std_iterator_
# define current pset_std_iterator_<P>
# define super top< current >

  stc_Header;
  typedef stc::is<Iterator_on_Points> category;
  typedef P point;
  stc_End;

  template < typename P >
  class pset_std_iterator_ : public super
  {
  public:
    stc_using(point);

    point impl_cast() { return *iter_; }
    point const* point_adr () const { return &(*iter_); }

    void start () { iter_ = ref_.begin(); }
    void next () { assert(is_valid()); iter_++; }
    void invalidate () { iter_ = nop_; }
    bool is_valid () const { return iter_ != nop_; }

    pset_std_iterator_(std::set<P>& init) : ref_(init) { nop_ = ref_.end(); ++nop_;}

  private:
    typename std::set<P>& ref_;
    typename std::set<P>::iterator iter_;
    typename std::set<P>::iterator nop_;
  };

# include "../local/undefs.hh"

  // Box
  // ---

# define templ template < typename P >
# define classname box_
# define current box_< P >
# define super point_set_base_< current >

  stc_Header;
  typedef P point;
  stc_End;

  template < typename P >
  class box_ : public super
  {
  public:
    stc_using(point);
    stc_using(grid);
    P pmin, pmax;
    typedef typename point::dim dim;

    enum { n = mlc_value(dim) };

    box_(P p1, P p2) : pmin(p1), pmax(p2) {}
    bool impl_includes(const point& p) const {
      for (unsigned i = 0; i < n; ++i)
      {
        if (p[i] < pmin[i] or p[i] > pmax[i])
          return false;
      }
      return true;
    }
  };

# include "../local/undefs.hh"


  // ==========================
  // EXPERIMENTAL (NON WORKING)
  // ==========================

  // Empty

}

// =============================
// Very Advanced Test Algortihms
// =============================

template <typename Exact>
bool bidon (const glg::Point<Exact>& p1, const glg::Point<Exact>& p2)
{
  return p1 == p2;
}


#endif // !GRID_HH_
