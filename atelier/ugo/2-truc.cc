#include "../local/scoop.hh"
#include <mlc/int.hh>

/* MELANGE SCOOP 1 - 2 qui ne marche surrement pas */

namespace pts
{
  stc_equip_namespace;
  stc_decl_associated_type(grid);
  stc_decl_associated_type(coord);
  stc_decl_associated_type(dim);

  //GRID ///////////////////////

  template <typename Exact> struct Grid { typedef Exact type;};

  struct grid1d : public Grid<grid1d> { };

  struct grid2d : public Grid<grid2d> { };

  //////////////////////////////

  template <typename T> struct point2d_;

  template <typename Exact>
  struct Point : public virtual any<Exact>,
  {
      stc_typename(grid);
      stc_typename(coord);
      stc_typename(dim);

      enum { n = mlc_value(dim) };
      bool operator== (Point<Exact> const& rhs) const	{ return this->exact().impl_equal(rhs.exact());  }
      bool operator< (Point<Exact> const& rhs) const	{ return this->exact().impl_inf(rhs.exact());    }
      bool operator!= (Point<Exact> const& rhs) const	{ return this->exact().impl_dif(rhs.exact());    }
      bool operator> (Point<Exact> const& rhs) const	{ return this->exact().impl_sup(rhs.exact());    }
      bool operator>= (Point<Exact> const& rhs) const	{ return this->exact().impl_supeq(rhs.exact());  }
      bool operator<= (Point<Exact> const& rhs) const	{ return this->exact().impl_infequ(rhs.exact()); }
    protected:
      Point() {};
  };

  template <typename T>
  struct vtypes< point2d_<T> >
  {
      typedef point2d_<T>	current;
      typedef Point< current >	super_type;
      typedef grid2d		grid;
      typedef T			coord;
      typedef mlc::uint_<2>	dim;
  };

  template <typename T>
  struct point2d_ : public Point< point2d_<T> >
  {
      typedef point2d_<T> current;
      typedef Point< current > super;

      stc_using(grid);
      stc_using(coord);
      stc_using(dim);

      int	row;
      int	col;
      coord	x;
      coord	y;

      bool impl_equal(point2d_<T> const& rhs) {	return row = rhs.row and col = rhs.col;			   }
      bool impl_inf(point2d_<T> const& rhs)   { return row < rhs.row or (row = rhs.row and col < rhs.col); }
      bool impl_dif(point2d_<T> const& rhs)   { return not impl_equal(rhs);				   }
      bool impl_sup(point2d_<T> const& rhs)   { return not impl_inf(rhs) and not impl_equal(rhs);	   }
      bool impl_supeq(point2d_<T> const& rhs) {	return not impl_inf(rhs);				   }
      bool impl_infeq(point2d_<T> const& rhs) {	return not impl_sup(rhs);				   }
  };

  typedef point2d_<int> point2d;

  template <typename P>
  bool	bidon(const P&	p1,
	      const P&	p2)
  {
    return p1 == p2;
  }
}

int	main()
{
  return (0);
}
