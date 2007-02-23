#ifndef _5_SET_HH_
#define _5_SET_HH_

#include "concept.hh"

namespace ugo
{

//----------------------------------------------------------------------------
//              ARRAY
//----------------------------------------------------------------------------

  template <typename value_t, typename coord_t = int>
  struct array2d
  {
      array2d(coord_t imin, coord_t jmin, coord_t imax, coord_t jmax);
      array2d(coord_t ilen, coord_t jlen);

      ~array2d();

      value_t  operator()(coord_t i, coord_t j) const;
      value_t& operator()(coord_t i, coord_t j);

      bool has(coord_t i, coord_t j) const;

      std::size_t memsize() const;

    protected:

      coord_t   imin_, jmin_, imax_, jmax_;
      coord_t   ilen_, jlen_;
      value_t*  buffer_;
      value_t** array_;

    private:

      void allocate_();
      void deallocate_();
  };

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::array2d(coord_t imin, coord_t jmin,
				     coord_t imax, coord_t jmax) :
    imin_(imin),
    jmin_(jmin),
    imax_(imax),
    jmax_(jmax)
  {
    precondition(imax >= imin and jmax >= jmin);
    ilen_ = imax - imin + 1;
    jlen_ = jmax - jmin + 1;
    allocate_();
  }

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::array2d(coord_t ilen, coord_t jlen) :
    imin_(0),
    jmin_(0),
    ilen_(ilen),
    jlen_(jlen)
  {
    precondition(ilen > 0 and jlen > 0);
    imax_ = imin_ + ilen_;
    jmax_ = jmin_ + ilen_;
    allocate_();
  }

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::~array2d()
  {
    deallocate_();
  }

  template <typename value_t, typename coord_t>
  value_t array2d<value_t, coord_t>::operator()(coord_t i, coord_t j) const
  {
    precondition(has(i, j));
    return array_[i][j];
  }

  template <typename value_t, typename coord_t>
  value_t& array2d<value_t, coord_t>::operator()(coord_t i, coord_t j)
  {
    precondition(has(i, j));
    return array_[i][j];
  }

  template <typename value_t, typename coord_t>
  bool array2d<value_t, coord_t>::has(coord_t i, coord_t j) const
  {
    return
      i >= imin_ and i <= imax_ and
      j >= jmin_ and j <= jmax_;
  }

  template <typename value_t, typename coord_t>
  size_t array2d<value_t, coord_t>::memsize() const
  {
    return
      // buffer_
      size_t(ilen_) * size_t(jlen_) * sizeof(value_t)
      +
      // array_
      size_t(ilen_) * sizeof(value_t*);
  }

  template <typename value_t, typename coord_t>
  void array2d<value_t, coord_t>::allocate_()
  {
    buffer_ = new value_t[size_t(ilen_) * size_t(jlen_)];
    array_ = new value_t*[size_t(ilen_)];
    value_t* buf = buffer_ - jmin_;
    for (coord_t i = 0; i < ilen_; ++i)
    {
      array_[i] = buf;
      buf += jlen_;
    }
    array_ -= imin_;
  }

  template <typename value_t, typename coord_t>
  void array2d<value_t, coord_t>::deallocate_()
  {
    precondition(buffer_ != 0 and array_ != 0);
    delete[] buffer_;
    buffer_ = 0; // safety
    array_ += imin_;
    delete[] array_;
    array_ = 0;  // safety
  }



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

      enum { n = mlc_value(dim) };
      point2d_(coord x, coord y) : row(x), col(y) {}

      bool impl_equal(point2d_<T> const& rhs) const  { return row == rhs.row and col == rhs.col;		   }
      bool impl_inf(point2d_<T> const& rhs) const    { return row < rhs.row or (row == rhs.row and col < rhs.col); }
      bool impl_dif(point2d_<T> const& rhs) const    { return not impl_equal(rhs);				   }
      bool impl_sup(point2d_<T> const& rhs) const    { return not impl_inf(rhs) and not impl_equal(rhs);	   }
      bool impl_supeq(point2d_<T> const& rhs) const  { return not impl_inf(rhs);				   }
      bool impl_infeq(point2d_<T> const& rhs) const  { return not impl_sup(rhs);				   }
      T impl_croch(T const& i) const {
	switch (i)
	{
	  case 0: return row;
	    break;
	  case 1: return col;
	    break;
	  default: abort(); return T();
	}
      }
      point2d_() : row(0), col(0) {}

      coord	row, col;
  };



  //--Point1d_------------------

  template <typename T> struct point1d_;

  template <typename T>
  struct vtypes< point1d_<T> >
  {
      typedef point1d_<T>	current;
      typedef top<current>	super_type;

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
      T impl_croch(T const& x) const { assert(x == 0); return i;              }

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

      unsigned	impl_npoints()                      { return nb;		       	     }
      bool      impl_includes(const point& p) const { if (p) return true; else return false; }

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

  template <typename P> struct box_iterator_;

  template <typename P>
  struct vtypes< box_iterator_<P> >
  {
      typedef box_iterator_<P>		current;
      typedef top<current>		super_type;

      typedef P	point;
      typedef P value;

      typedef stc::is<Iterator_on_Points>	category;
  };

  template <typename P>
  // P = type de point
  struct box_iterator_ : public top< box_iterator_<P> >
  {
      typedef box_iterator_<P>	current;
      typedef top<current>	super;

      stc_using(value);
      stc_using(point);

      box_iterator_(const box_<point>& box) : bbox(box)  {
	nop = box.pmax;
	++nop[0];
      }

      void impl_start() { p =  bbox.pmin; }

      void impl_next()
      {
	for (int i = point::n - 1; i >= 0; --i)
	  if (p[i] == bbox.pmax(i))
	    p[i] = bbox.pmin(i);
	  else
          {
            ++p[i];
            break;
          }
	if (p == bbox.pmin())
	  p = nop;
      }

      void  impl_invalidate()             { p = nop;         }
      bool  impl_is_valid() const         { return p != nop; }
      point impl_to_point() const  	  { return p;        }
      point const* impl_point_adr() const { return &p;	     }
      point impl_cast() const             { return p;        }
      point impl_cast()		          { return p;        }

    protected:
      point		p;
      box_<point>	bbox;
      point		nop;
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
      stc_using(point);

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
  typedef array2d<T, coord>  data;

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
      stc_using( data  );

      image2d(box &box_init) : bb_(box_init) {}

      bool  imp_owns(const point& p) const   { return  bb_.includes(p); }
      value imp_value_acces(const point& p)  { return T_(p.row, p.col); }
      box   impl_bbox() const		     { return bb_;              }
    protected:
      box	&bb_;
      T*	buffer_;
      T**	array_;
  };

# include "../local/undefs.hh"



  //--signal-------------------

# define current    signal<T>
# define super      top<current>
# define templ      template <typename T>
# define classname  signal

  stc_Header;
  typedef point1d_<T>		point;
  typedef T			coord;
  typedef box_iterator_<point>	iter;
  typedef T			value;
  typedef grid1d		grid;
  typedef box_<point1d_<T> >	box;

  typedef stc::is<Image> category;
  stc_End;


  template <typename T>
  struct signal : public super
  {
      stc_using( point );
      stc_using( value );
      stc_using( box   );
      stc_using( iter  );
      stc_using( grid  );

      signal(box &box_init) : bb_(box_init) {}

      bool  impl_owns(const point& p) const  { return  bb_.includes(p); }
      value impl_value_acces(const point& p) { return T_(p.row, p.col); }
      box   impl_bbox() const	 	     { return bb_;              }
      value impl_includes(const point& p)    { return p.value;		}
    protected:
      box &bb_;
  };
}

#endif
