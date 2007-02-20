#ifndef  _3_ITERATOR_HH_
# define _3_ITERATOR_HH_

#include "concept.hh"

namespace ugo
{

  //--Iterator_on_Points------------------

  template <typename T>  struct Iterator_on_Points;

  template <typename T>
  struct vtypes< Iterator_on_Points<T> >
  {
      typedef Iterator_on_Points<T> current;
      typedef top< current >	    super_type;

      typedef stc::abstract	point;
      typedef stc::is<Iterator> category;

      typedef T value;
  };

  template <typename Exact>
  struct Iterator_on_Points : public top < Iterator_on_Points<Exact> >
  {
      typedef Iterator_on_Points<Exact>	current;
      typedef top< current >		super;

      stc_typename(point);

      Iterator_on_Points();
      ~Iterator_on_Points();

      operator point () const		{ return this->exact().impl_cast();  }
      point to_point() const		{ return *this;                      }
      const point point_adr()		{ return this->exact().impl_adr();   }
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
  struct dummy_iterator_on_point2d : public Iterator_on_Points< dummy_iterator_on_point2d<T> >
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




}

#endif  /* _3_ITERATOR_HH_ */

