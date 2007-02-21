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
      bool impl_croch(point2d_<T> const& i) const    { if (i == 1) return x; else return y;             	   }

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
      typedef top< current >		super_type;

      typedef stc::is<Point_set>	category;

      typedef stc::abstract point;

      typedef stc_deferred(point) point__;
      typedef stc::final<stc_type(point__, grid)> grid;

      typedef Iterator<grid>	 iter;
  };

  template <typename Exact>
  class point_set_base : public top< point_set_base<Exact> >
  {
      typedef point_set_base<Exact>	current;
      typedef top< current >		super;
    protected:
      point_set_base() {}
  };




  //--point_set_2d-------------------

  template < typename T > struct point_set_2d;

  template <typename T>
  struct vtypes<point_set_2d<T> >
  {
      typedef point_set_2d<T>		current;
      typedef point_set_base< current >	super_type;

      typedef stc::is<Point_set> category;
  };

  template <typename T>
  struct point_set_2d : public point_set_base< point_set_2d<T> >
  {
      typedef point_set_2d<T>		current;
      typedef point_set_base< current > super;

      stc_using(point);

      unsigned	impl_npoints()                 { return nb;     }
      bool impl_includes(const point& p) const { return false;  }

    private:
      int	nb;
      T		truc;
  };



  //--box_------------------------

  template <typename P> struct box_;

  template <typename T>
  struct vtypes< box_<T> >
  {
      typedef box_<T>		         current;
      typedef point_set_base< current >  super_type;

  };

  template <typename P>
  struct box_ : public point_set_base< box_<P> >
  {
      typedef box_<P>			current;
      typedef point_set_base< current >	super;

      stc_using(box);

      P pmin, pmax;

      enum { n = mlc_value(typename P::dim) };
  };

  //-----

  typedef point2d_<int>		 point2d;
  typedef box_<point2d>	         box2d;

  //-----

  //----------------------------------------------------------------------------
  //              ITERATORS
  //----------------------------------------------------------------------------


  //--Iterator_on_Points------------------

  template <typename T>  struct iterator_on_Points;

  template <typename T>
  struct vtypes< iterator_on_Points<T> >
  {
      typedef iterator_on_Points<T> current;
      typedef top< current >	    super_type;

      typedef stc::abstract	point;
      typedef stc::is<Iterator> category;

      typedef T value;
  };

  template <typename Exact>
  struct iterator_on_Points : public top < iterator_on_Points<Exact> >
  {
      typedef iterator_on_Points<Exact>	current;
      typedef top< current >		super;

      stc_typename(point);

      iterator_on_Points();
      ~iterator_on_Points();

      operator point () const		{ return this->exact().impl_cast();  }
      point to_point() const		{ return *this;                      }
      const point point_adr()		{ return this->exact().impl_adr();   }
  };



  //--box_iterator_----------------

  template <typename E> struct box_iterator_;

  template <typename T>
  struct vtypes< box_iterator_<T> >
  {
      typedef box_iterator_<T>	             current;
      typedef iterator_on_Points< current >  super_type;
  };

  template <typename P>
  struct box_iterator_ : public iterator_on_Points< box_iterator_<P> >
  {
      typedef  P point_t;

      box_iterator_(const box2d& bb);

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

      typedef stc::abstract	point;
      typedef stc::is<Iterator> category;

      typedef T value;
  };

  template <typename T>
  struct dummy_iterator_on_point2d : public iterator_on_Points< dummy_iterator_on_point2d<T> >
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

}

#endif
