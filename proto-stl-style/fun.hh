#ifndef MINIOLN_FUN_HH
# define MINIOLN_FUN_HH

# include "misc.hh"
# include "abstract.hh"
# include "ch_value.hh"


// fwd decl

template <class I, class F> struct thru_pw_fun_;



// . . . . . . . . . . . . . . . . . . . . . . . .  f_::abstract::fun


namespace f_ { namespace abstract {

  template <class F>
  struct fun : public mlc::any<F>
  {
    template <class I>
    thru_pw_fun_<I, F> operator()(const ::abstract::image<I>& ima) const;
  };
  
} } // end of namespaces

set_name_TC(f_::abstract::fun);



// . . . . . . . . . . . . . . . . . . . . . . . .  through fun


template <class I, class Fun>
struct thru_pw_fun_ : public abstract::morphed_image< I,
						      thru_pw_fun_<I, Fun> >
{
  typedef thru_pw_fun_<I, Fun> self_type;
  typedef abstract::morphed_image<I, self_type> super;

  typedef typename Fun::result_type value_type;
  typedef value_type                value_get_type;
  typedef mlc::no_type              value_set_type;

  typedef typename ch_value<oln_type_of(I, concrete), value_type>::ret concrete_type;
  typedef_super(point_type);

  Fun fun_; // a pure function

  thru_pw_fun_()
    : super(),
      fun_()
  {}

  thru_pw_fun_(const self_type& rhs)
    : super(rhs),
      fun_(rhs.fun_)
  {}

  thru_pw_fun_(const abstract::image<I>& ima,
	       const Fun& fun)
    : super(ima),
      fun_(fun)
  {}

  template <class Info>
  thru_pw_fun_(const abstract_info<Info>& info)
    : super(info),
      fun_() // IDEA: get fun from info?
  {}

  const value_get_type operator[](const point_type& p) const
  {
    return fun_.apply(this->image_[p]);
  }

  // disable writing access:
  void operator[](const point_type&);

};



// op()

namespace f_ { namespace abstract {

  // FIXME: should be thru_pw_fun_<const I, F>?
  template <class F> template <class I>
  thru_pw_fun_<I, F> fun<F>::operator()(const ::abstract::image<I>& ima) const
  {
    thru_pw_fun_<I, F> tmp(ima, this->exact());
    return tmp;
  }
  
} } // end of namespace


#endif
