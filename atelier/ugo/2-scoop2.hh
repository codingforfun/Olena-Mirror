#ifndef  _2_SCOPP2_HH_
# define _2_SCOPP2_HH_

#include "concept.hh"

namespace ugo
{
  //--Grid----------------------

  template <typename Exact> struct Grid { typedef Exact type; };

  struct grid1d : public Grid< grid1d > { };

  struct grid2d : public Grid< grid2d > { };



  //--Point2d_------------------

  template <typename T> struct point2d_;

  template <typename T>
  struct vtypes< point2d_<T> >
  {
      typedef point2d_<T>	current;
      typedef top< current >	super_type;

      typedef stc::is<Point>	category;

      typedef grid2d		grid;
      typedef T			coord;
      typedef mlc::uint_<2>	dim;
  };

  template <typename T>
  struct point2d_ : public top< point2d_<T> >
  {
      typedef point2d_<T>	current;
      typedef top< current >	super;

      stc_using( grid  );
      stc_using( coord );
      stc_using( dim   );

      bool impl_equal(point2d_<T> const& rhs) const  { return row == rhs.row and col == rhs.col;		   }
      bool impl_inf(point2d_<T> const& rhs) const    { return row < rhs.row or (row == rhs.row and col < rhs.col); }
      bool impl_dif(point2d_<T> const& rhs) const    { return not impl_equal(rhs);				   }
      bool impl_sup(point2d_<T> const& rhs) const    { return not impl_inf(rhs) and not impl_equal(rhs);	   }
      bool impl_supeq(point2d_<T> const& rhs) const  { return not impl_inf(rhs);				   }
      bool impl_infeq(point2d_<T> const& rhs) const  { return not impl_sup(rhs);				   }

      point2d_() : row(0), col(0), x(0), y(0) {}

      int	row, col;
      coord	x, y;
  };

  //--Functions-------------------------

  template <typename P>
  bool	bidon(const Point<P>&	p1,
	      const Point<P>&	p2)
  {
    return (p1 == p2);
  }



  //--typedef--------------------------

  typedef point2d_<int> point2d;



}

#endif
