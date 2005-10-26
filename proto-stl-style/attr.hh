#ifndef MINIOLN_ATTR_HH
# define MINIOLN_ATTR_HH

# include "misc.hh"
# include "abstract.hh"
# include "ch_value.hh"


// fwd decl

template <class I, class A> struct attr_;



// . . . . . . . . . . . . . . . . . . . . . . . .  f_::abstract::attr


namespace f_ {

  struct red
  {
    template <class T>
    struct result
    {
      typedef typename T::red_type ret;
    };

    template <class T>
    typename T::red_type& operator()(T& value) const
    {
      return value.red;
    }

    template <class T>
    const typename T::red_type& operator()(const T& value) const
    {
      return value.red;
    }

    template <class I>
    attr_<I, red> operator()(const ::abstract::image<I>& ima) const;
  };
  
} // end of f_

set_name(f_::red);

f_::red f_red;


// . . . . . . . . . . . . . . . . . . . . . . . .  through attr


template <class I, class A>
struct attr_ : public abstract::morphed_image< I,
					       attr_<I, A> >
{
  typedef attr_<I, A> self_type;
  typedef abstract::morphed_image<I, self_type> super;

  typedef typename A::template result<oln_type_of(I, value)>::ret value_type;
  typedef value_type value_get_type;
  typedef value_type value_set_type;

  typedef typename ch_value<oln_type_of(I, concrete), value_type>::ret concrete_type;
  typedef_super(point_type);

  A fun_;

  attr_()
    : super()
  {}

  attr_(const self_type& rhs)
    : super(rhs)
  {}

  attr_(const abstract::image<I>& ima) // IDEA: relax?
    : super(ima)
  {}

  template <class Info>
  attr_(const abstract_info<Info>& info)
    : super(info)
  {}

  const value_get_type operator[](const point_type& p) const
  {
    return fun_(this->image_[p]);
  }

  // TODO: the code below relies on an implicit assumption
  // this->image_ owns effective data (that can be modified)
  // this is *very* restrictive => we should define two different
  // attr_-like types that differ from the impl of "op[] mutable"...
  value_set_type& operator[](const point_type& p)
  {
    return fun_(this->image_[p]);
  }

};



// op()

namespace f_ {

  template <class I>
  attr_<I, red> red::operator()(const ::abstract::image<I>& ima) const
  {
    attr_<I, red> tmp(ima);
    return tmp;
  }
  
} // end of f_


#endif
