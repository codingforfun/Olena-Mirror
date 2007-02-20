#ifndef GRID_HH_
# define GRID_HH_

# include <mlc/int.hh>
# include <mlc/uint.hh>
# include "../local/scoop.hh"
# include "../local/undefs.hh "

namespace glg
{
  stc_equip_namespace;

  // Abstract types
  stc_decl_associated_type(grid);
  stc_decl_associated_type(coord);
  stc_decl_associated_type(dim);

  template <typename Exact> class Grid;

  class grid1d;
  class grid2d;

  // Grids

  template <typename Exact>
  class Grid : public stc::any <Exact>
  {};

  class grid1d : public Grid <grid1d>
  {};

  class grid2d : public Grid <grid2d>
  {};


  template <typename Exact> class Point;
  template <typename Exact> class point2d_;

  template <typename Exact>
  struct vtypes <Point <Exact> >
  {
    typedef any <Exact> super_type;
    typedef stc::abstract grid;
    typedef stc::abstract coord;
    typedef stc::abstract dim;
  };


  // Points

  template <typename Exact>
  class Point : public any <Exact>
  {
  public :

    typedef stc_type(Exact, grid) grid;
    typedef stc_type(Exact, coord) coord;
    typedef stc_type(Exact, dim) dim;

    typedef Point<Exact> self;

    bool operator==(self const& rhs) const { return this->exact().impl_eq(rhs.exact()); }
    bool operator<(self const& rhs) const { return this->exact().impl_lt(rhs.exact()); }
    bool operator!=(self const& rhs) const { return this->exact().impl_ne(rhs.exact()); }
    bool operator>(self const& rhs) const { return this->exact().impl_gt(rhs.exact()); }
    bool operator>=(self const& rhs) const { return this->exact().impl_ge(rhs.exact()); }
    bool operator<=(self const& rhs) const { return this->exact().impl_le(rhs.exact()); }
  };


  template <typename C>
  struct vtypes <point2d_ <C> >
  {
    typedef Point< point2d_<C> > super_type;
    typedef grid2d grid;
    typedef C coord;
    typedef mlc::uint_<2> dim;
  };


  template <typename C>
  class point2d_ : public Point < point2d_ <C> >
  {
  public:

    typedef typename Point< point2d_<C> >::grid grid;
    typedef typename Point< point2d_<C> >::coord coord;
    typedef typename Point< point2d_<C> >::dim dim;

    point2d_ (int x, int y) : row(x), col(y) {}

    C row, col;

    bool impl_eq(point2d_<C> const& rhs) const { return col == rhs.col and row == rhs.row; }
    bool impl_lt(point2d_<C> const& rhs) const { return col < rhs.col and row < rhs.row; }
    bool impl_ne(point2d_<C> const& rhs) const { return col != rhs.col and row != rhs.row; }
    bool impl_gt(point2d_<C> const& rhs) const { return col > rhs.col and row > rhs.row; }
    bool impl_ge(point2d_<C> const& rhs) const { return col >= rhs.col and row >= rhs.row; }
    bool impl_le(point2d_<C> const& rhs) const { return col <= rhs.col and row <= rhs.row; }
  };
}

template <typename Exact>
bool bidon (const glg::Point<Exact>& p1, const glg::Point<Exact>& p2)
{
  return p1 == p2;
}


#endif // !GRID_HH_
