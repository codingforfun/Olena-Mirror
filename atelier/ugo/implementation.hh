#ifndef _5_SET_HH_
#define _5_SET_HH_

#include "concept.hh"

namespace ugo
{

  //----------------------------------------------------------------------------
  //              POINT
  //----------------------------------------------------------------------------

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

      typedef grid2d		grid;
      typedef T			coord;
      typedef mlc::uint_<2>	dim;

      typedef stc::is<Point>	category;
  };

  template <typename T>
  struct point2d_ : public top< point2d_<T> >
  {
      typedef point2d_<T>	current;
      typedef top< current >	super;

      stc_using( grid  );
      stc_using( coord );
      stc_using( dim   );

      point2d_(coord x, coord y) : row(x), col(y) {}

      bool impl_equal(point2d_<T> const& rhs) const  { return row == rhs.row and col == rhs.col;		   }
      bool impl_inf(point2d_<T> const& rhs) const    { return row < rhs.row or (row == rhs.row and col < rhs.col); }
      bool impl_dif(point2d_<T> const& rhs) const    { return not impl_equal(rhs);				   }
      bool impl_sup(point2d_<T> const& rhs) const    { return not impl_inf(rhs) and not impl_equal(rhs);	   }
      bool impl_supeq(point2d_<T> const& rhs) const  { return not impl_inf(rhs);				   }
      bool impl_infeq(point2d_<T> const& rhs) const  { return not impl_sup(rhs);				   }
      bool impl_croch(point2d_<T> const& i) const    { if (i == 1) return row; else return col;             	   }

      point2d_() : row(0), col(0) {}

      coord	row, col;
  };



  //--Point1d_------------------

  template <typename T> struct point1d_;

  template <typename T>
  struct vtypes< point1d_<T> >
  {
      typedef point1d_<T>	current;
      typedef top< current >	super_type;

      typedef grid1d		grid;
      typedef T			coord;
      typedef mlc::uint_<2>	dim;

      typedef stc::is<Point>	category;
  };

  template <typename T>
  struct point1d_ : public top< point1d_<T> >
  {
      typedef point1d_<T>	current;
      typedef top< current >	super;

      stc_using( grid  );
      stc_using( coord );
      stc_using( dim   );

      point1d_(coord x) : i(x) {}

      bool impl_equal(point1d_<T> const& rhs) const  { return i == rhs.i ;		                   }
      bool impl_inf(point1d_<T> const& rhs) const    { return i < rhs.i ;                                  }
      bool impl_dif(point1d_<T> const& rhs) const    { return not impl_equal(rhs);			   }
      bool impl_sup(point1d_<T> const& rhs) const    { return not impl_inf(rhs) and not impl_equal(rhs);   }
      bool impl_supeq(point1d_<T> const& rhs) const  { return not impl_inf(rhs);			   }
      bool impl_infeq(point1d_<T> const& rhs) const  { return not impl_sup(rhs);			   }
      bool impl_croch(point1d_<T> const& x) const    { if (x) return true; else return false;              }

      point1d_() : i(0) {}

      coord	i;
  };



  //--Functions-------------------------

  template <typename P>
  bool	bidon(const Point<P>&	p1,
	      const Point<P>&	p2)
  {
    return (p1 == p2);
  }



  //-----

  typedef point2d_<int> point2d;

  //-----

  //----------------------------------------------------------------------------
  //              POINT_SET
  //----------------------------------------------------------------------------

  //-point_set_base------------------

  template <typename T> struct point_set_base;

  template <typename Exact>
  struct vtypes< point_set_base<Exact> >
  {
      typedef point_set_base<Exact>	current;
      typedef top<Exact>		super_type;

      typedef stc::abstract		point;
      typedef stc_deferred(point) point__;
      typedef stc::final<stc_type(point__, grid)> grid;
      typedef Iterator<grid>		iter;

      typedef stc::is<Point_set>	category;
  };

  template <typename Exact>
  class point_set_base : public top<Exact>
  {
    protected:
      point_set_base() {}
  };



  //--point_set_2d-------------------

  template <typename P> struct point_set_2d;

  template <typename P>
  struct vtypes< point_set_2d<P> >
  {
      typedef point_set_2d<P>		current;
      typedef point_set_base<current>	super_type;

      typedef point2d			point;
  };

  template <typename P>
  struct point_set_2d : public point_set_base< point_set_2d<P> >
  {
      typedef point_set_2d<P>		current;
      typedef point_set_base<current>	super;

      stc_using(grid);
      stc_using(point);

      unsigned	impl_npoints()                 { return nb;				}
      bool impl_includes(const point& p) const { if (p) return true; else return false; }

    private:
      int	nb;
      P		plus;
  };



  //--box_------------------------

  template <typename P> struct box_;

  template <typename P>
  struct vtypes< box_<P> >
  {
      typedef box_<P>			current;
      typedef point_set_base<current>	super_type;

      typedef P point;
  };

  template <typename P>
  struct box_ : public point_set_base< box_<P> >
  {
      typedef box_<P>			current;
      typedef point_set_base< current >	super;

      stc_using( point );

      box_() : pmin(0, 0), pmax(0, 0) {}
      box_(point p1, point p2) : pmin(p1), pmax(p2) {}

      enum { n = mlc_value(typename P::dim) };
      const box_& impl_bbox() const { return *this; }
      bool impl_includes(const point& p) const
      {
	for (unsigned i = 0; i < n; ++i)
	  if (p[i] < pmin[i] or p[i] > pmax[i])
	    return false;
	return true;
      }

    private:
      point pmin, pmax;
  };

  //-----

  typedef point2d_<int>		 point2d;
  typedef box_<point2d>	         box2d;

  //-----

  //----------------------------------------------------------------------------
  //              ITERATORS
  //----------------------------------------------------------------------------

  //--box_iterator_----------------

  template <typename E> struct box_iterator_;

  template <typename T>
  struct vtypes< box_iterator_<T> >
  {
      typedef box_iterator_<T>		current;
      typedef top< current >		super_type;

      typedef stc::is<Point_set>	category;
  };

  template <typename P>
  struct box_iterator_ : public top< box_iterator_<P> >
  {
      typedef  P point_t;

      box_iterator_(box_<point_t> box) : bb_(box)
      {
	nop_ = box.pmax;
	++nop_[0];
      }

      void impl_start();

      void impl_next()
      {
	for (int i = point_t::n - 1; i >= 0; --i)
	  if (p_[i] == bb_.pmax(i))
	    p_[i] = bb_.pmin(i);
	  else
          {
            ++p_[i];
            break;
          }
	if (p_ == bb_.pmin())
	  p_ = nop_;
      }

      void impl_invalidate();

      bool impl_is_valid() const;

    protected:
      point_t		p_;
      box_<point_t>	bb_;
      point_t		nop_;

  };



  //--dummy_iterator_on_point2d-------------

  template <typename T> struct dummy_iterator_on_point2d;

  template <typename T>
  struct vtypes< dummy_iterator_on_point2d<T> >
  {
      typedef dummy_iterator_on_point2d<T> current;
      typedef top< current >	           super_type;

      typedef T		                   point;
      typedef T				   value;

      typedef stc::is<Iterator_on_Points> category;
  };

  template <typename T>
  struct dummy_iterator_on_point2d : public top< dummy_iterator_on_point2d<T> >
  {
    public:
      typedef dummy_iterator_on_point2d<T> current;
      typedef top< current >		   super;

      stc_using(value);

      dummy_iterator_on_point2d();
      ~dummy_iterator_on_point2d();

      void impl_start()		  { return; }
      void impl_next()		  { return; }
      bool impl_is_valid() const  { return; }
      void impl_cast()		  { return; }

      int**	grid;
      int	max_value;
  };

  //-----

  typedef box_iterator_<point2d> iter2d;

  //-----

  //----------------------------------------------------------------------------
  //              IMAGES
  //----------------------------------------------------------------------------

  //--image2d-------------------

# define current    image2d<T>
# define super      top<current>
# define templ      template <typename T>
# define classname  image2d

  stc_Header;
  typedef iter2d	iter;
  typedef point2d	point;
  typedef point::coord	coord;
  typedef T		value;
  typedef grid2d	grid;
  typedef box2d		box;

  typedef stc::is<Image2d> category;
  stc_End;


  template <typename T>
  class image2d : public super
  {
    public:
      stc_using( point );
      stc_using( value );
      stc_using( box   );
      stc_using( iter  );
      stc_using( grid  );

      image2d(box &box_init) : bb_(box_init) {}

      bool  imp_owns(const point& p) const   { return  bb_.includes(p); }
      value imp_value_acces(const point& p)  { return T_(p.row, p.col); }
      box   impl_bbox() const		     { return bb_;              }
    protected:
      box &bb_;
  };

# include "../local/undefs.hh"



  //--signal-------------------

# define current    signal<T>
# define super      top<current>
# define templ      template <typename T>
# define classname  signal

  stc_Header;
  typedef iter2d	iter;
  typedef point2d	point;
  typedef point::coord	coord;
  typedef T		value;
  typedef grid1d		grid;
  typedef box_<point1d_<T> >	box;

  typedef stc::is<Image> category;
  stc_End;


  template <typename T>
  class signal : public super
  {
    public:
      stc_using( point );
      stc_using( value );
      stc_using( box   );
      stc_using( iter  );
      stc_using( grid  );

      signal(box &box_init) : bb_(box_init) {}

      bool imp_owns(const point& p) const   { return  bb_.includes(p); }
      value imp_value_acces(const point& p) { return T_(p.row, p.col); }
      box impl_bbox() const		    { return bb_;              }
    protected:
      box &bb_;
  };
}

#endif
