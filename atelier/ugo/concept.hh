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


  //--Point_set-------------------

  template <typename Exact>
  class Point_set : public virtual any< Exact >,
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



  //--
}

//--implementations

#include "implementation.hh"

#endif	    /* !CONCEPT_HH_ */
