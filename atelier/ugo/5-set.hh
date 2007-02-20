#ifndef _5_SET_HH_
#define _5_SET_HH_

#include "concept.hh"

namespace ugo
{

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

      unsigned	impl_npoints()                { return nb;   }
      bool	impl_includes(const point& p) { return true; }
    private:
      int nb;
      T truc;
  };



}

#endif
