#ifndef   	CONCEPT2_HH_
# define   	CONCEPT2_HH_

//--includes---------------------

#include <mlc/assert.hh>
#include <mlc/cmp.hh>
#include <mlc/int.hh>
#include <cassert>
#include <mlc/contract.hh>
#include "../local/scoop.hh"
#include "tools.hh"

namespace ugo
{

  stc_equip_namespace;

  stc_decl_associated_type( grid  );
  stc_decl_associated_type( value );
  stc_decl_associated_type( point );
  stc_decl_associated_type( iter  );
  stc_decl_associated_type( box   );
  stc_decl_associated_type( coord );
  stc_decl_associated_type( data  );
  stc_decl_associated_type( dim  );

  //--Point------------------------

  template <typename Exact>
  struct Point : public virtual any<Exact>,
		 public automatic::get_impl<Point, Exact>
  {
      stc_typename( grid  );
      stc_typename( coord );
      stc_typename( dim   );

      enum { n = mlc_value(dim) };

      bool operator== (Point<Exact> const& rhs) const	{ return this->exact().impl_equal ( rhs.exact() );  }
      bool operator< (Point<Exact> const& rhs) const	{ return this->exact().impl_inf ( rhs.exact() );    }
      bool operator!= (Point<Exact> const& rhs) const	{ return this->exact().impl_dif ( rhs.exact() );    }
      bool operator> (Point<Exact> const& rhs) const	{ return this->exact().impl_sup ( rhs.exact() );    }
      bool operator>= (Point<Exact> const& rhs) const	{ return this->exact().impl_supeq ( rhs.exact() );  }
      bool operator<= (Point<Exact> const& rhs) const	{ return this->exact().impl_infequ ( rhs.exact() ); }
      coord operator[] (int rhs) const			{ return this->exact().impl_croch ( rhs	);	    }
      coord& operator[] (int rhs)			{ return this->exact().impl_croch ( rhs	);    	    }
    protected:
      Point() {}
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



  //--Iterator----------------------

  template <typename Exact>
  struct Iterator : public virtual any<Exact>,
		    public automatic::get_impl<Iterator, Exact>
  {
      stc_typename(value);

      void start()		{ this->exact().impl_start();		}
      void next()		{ this->exact().impl_next();		}
      void invalidate()		{ this->exact().impl_invalidate();	}
      void set(const value& v)	{ this->exact().impl_set(v);		}
      bool is_valid() const	{ return this->exact().impl_is_valid(); }
    protected:
      Iterator() {}
  };

  //--Iterator_on_Points------------------

  template <typename Exact>
  struct Iterator_on_Points : public Iterator<Exact>,
			      public automatic::get_impl<Iterator_on_Points, Exact>
  {
      stc_typename(point);

      operator point() const			      { return this->exact().impl_cast();      }
      operator point()				      { return this->exact().impl_cast();      }
      stc_type(Exact, point) to_point() const         { return this->exact().impl_to_point();  }
      stc_type(Exact, point) const* point_adr() const { return this->exact().impl_point_adr(); }
    protected:
      Iterator_on_Points() {}
  };



  //--Image----------------------

  template <typename Exact>
  struct Image : public virtual any< Exact >,
		 public automatic::get_impl<Image, Exact>
  {
      stc_typename( point );
      stc_typename( value );
      stc_typename( grid  );
      stc_typename( box   );
      stc_typename( iter  );
      stc_typename( data  );
      stc_typename( coord );

      value operator() (const point& p) const {
	assert(owns(p));
	return this->exact().impl_point_value(p);
      }
      value& operator() (const point& p) {
	assert(owns(p));
	return this->exact().impl_point_value(p);
      }
      bool owns(const point& p) const { return this->exact().impl_owns(p); }
      box  bbox() const               { return this->exact().impl_bbox();  }

    protected:
      Image() {}
  };



  //--Image2d-------------------Macro-Mode

# define current    Image2d<Exact>
# define super      Image<Exact>
# define templ      template <typename Exact>
# define classname  Image2d

  template <typename Exact>
  struct Image2d : super
  {
      stc_typename(coord);

      stc_using( point );
      stc_using( value );
      stc_using( box   );
      stc_using( iter  );
      stc_using( grid  );
      stc_using( data  );

      value operator() (const point& p)   { return this->exact().impl_point_value(p);  }
      bool  owns(const point& p) const	  { return this->exact().impl_owns(p);         }
      box   bbox() const		  { return this->exact().impl_bbox();          }
    protected:
      Image2d() {}
  };

# include "../local/undefs.hh"

}

#include "implementation2.hh"

#endif	    /* !CONCEPT2_HH_ */
