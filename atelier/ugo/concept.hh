#ifndef   	CONCEPT_HH_
# define   	CONCEPT_HH_

//--includes---------------------

#include <mlc/assert.hh>
#include <mlc/cmp.hh>
#include <mlc/int.hh>
#include <cassert>
#include "../local/scoop.hh"

namespace ugo
{

  //--equipement-------------------

  stc_equip_namespace;

  stc_decl_associated_type( grid  );
  stc_decl_associated_type( coord );
  stc_decl_associated_type( dim   );
  stc_decl_associated_type( value );
  stc_decl_associated_type( point );
  stc_decl_associated_type( iter  );
  stc_decl_associated_type( box   );


  //--Point------------------------

  template <typename Exact>
  struct Point : public virtual any<Exact>,
		 public automatic::get_impl<Point, Exact>
  {
      stc_typename(grid);
      stc_typename(coord);
      stc_typename(dim);

      enum { n = mlc_value(dim) };

      bool operator== (Point<Exact> const& rhs) const	{ return this->exact().impl_equal ( rhs.exact () );  }
      bool operator< (Point<Exact> const& rhs) const	{ return this->exact().impl_inf ( rhs.exact () );    }
      bool operator!= (Point<Exact> const& rhs) const	{ return this->exact().impl_dif ( rhs.exact () );    }
      bool operator> (Point<Exact> const& rhs) const	{ return this->exact().impl_sup ( rhs.exact () );    }
      bool operator>= (Point<Exact> const& rhs) const	{ return this->exact().impl_supeq ( rhs.exact () );  }
      bool operator<= (Point<Exact> const& rhs) const	{ return this->exact().impl_infequ ( rhs.exact () ); }
      bool operator[] (Point<Exact> const& rhs) const	{ return this->exact().impl_croch ( rhs.exact () ); }
    protected:
      Point() {}
  };



  //--Iterator---------------------

  template <typename Exact>
  struct Iterator : public any<Exact>,
		    public automatic::get_impl<Iterator, Exact>
  {
      stc_typename(value);

      void start()		{ this->exact().impl_start();		}
      void next()		{ this->exact().impl_next();		}
      void invalidate()		{ this->exact().impl_invalidate();	}
      bool is_valid() const	{ return this->exact().impl_is_valid(); }
      void set(const value& v)	{ this->exact().impl_set(v);		}
    protected:
      Iterator() {}
  };



  //--Iterator_on_Points------------------

  template <typename Exact>
  struct Iterator_on_Points : public Iterator<Exact>,
			      public automatic::get_impl<Iterator_on_Points, Exact>
  {
      stc_typename(point);

      Iterator_on_Points();
      ~Iterator_on_Points();

      operator stc_type(Exact, point)() const         { return this->exact().impl_cast();      }
      stc_type(Exact, point) to_point() const         { return this->exact().impl_to_point();  }
      stc_type(Exact, point) const* point_adr() const { return this->exact().impl_point_adr(); }
    protected:
      Iterator_on_Points() {}
  };



  //--Point_set-------------------

  template <typename Exact>
  struct Point_set : public virtual any< Exact >,
		     public automatic::get_impl<Point_set, Exact>
  {
      stc_typename( point );
      stc_typename( grid  );
      stc_typename( iter  );

      unsigned	npoints() const		         { return this->exact().impl_npoints();   }
      bool	includes(const point& p) const   { return this->exact().impl_includes(p); }
    protected:
      Point_set() {}
  };

  //--Image----------------------

  template <typename Exact>
  struct Image : public virtual any< Exact >,
		 public automatic::get_impl<Image, Exact>
  {
      stc_typename( point );
      stc_typename( value );
      stc_typename( box   );
      stc_typename( iter  );
      stc_typename( grid  );

      value operator() (const point& p)
      {
	assert(owns(p));
	return this->exact().impl_value_acces(p);
      }
      bool owns(const point& p) const { return this->exact().impl_owns(p); }
      box bbox() const { return this->exact().impl_bbox(); }

    protected:
      Image() {}
  };

}

//--implementations

#include "implementation.hh"

#endif	    /* !CONCEPT_HH_ */
