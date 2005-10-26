#ifndef MINIOLN_BIJFUN_HH
# define MINIOLN_BIJFUN_HH

# include <utility>
# include "misc.hh"
# include "abstract.hh"
# include "ch_value.hh"
# include "value_box.hh"



// fwd decl

template <class I, class F> struct through_bijfun_;



// . . . . . . . . . . . . . . . . . . . . . . . .  f_::abstract::bijfun


namespace f_ { namespace abstract {

  template <class F>
  struct bijfun : public mlc::any<F>
  {
    template <class I>
    through_bijfun_<I, F> operator()(const ::abstract::image<I>& ima) const;
  };

} } // end of namespaces

set_name_TC(f_::abstract::bijfun);



// . . . . . . . . . . . . . . . . . . . . . . . .  through bijfun


template <class I, class Fun>
struct through_bijfun_ : public abstract::morphed_image< I,
							 through_bijfun_<I, Fun> >
{
  typedef through_bijfun_<I, Fun> self_type;
  typedef abstract::morphed_image<I, self_type> super;

  typedef typename Fun::result_type value_type;
  typedef value_box<self_type> value_get_type;
  typedef value_box<self_type> value_set_type;

  typedef typename ch_value<oln_type_of(I, concrete), value_type>::ret concrete_type;
  typedef_super(point_type);

  // attrs
  Fun fun_;
  value_box<self_type> vb_;


  // ctors

  through_bijfun_()
    : super(),
      fun_()
  {
    vb_.this_ = this;
  }

  through_bijfun_(const self_type& rhs)
    : super(rhs),
      fun_(rhs.fun_)
  {
    vb_.this_ = this;
  }

  through_bijfun_(const abstract::image<I>& ima)
    : super(ima),
      fun_()
  {
    vb_.this_ = this;
  }

  template <class Info>
  through_bijfun_(const abstract_info<Info>& info)
    : super(info),
      fun_() // IDEA: get fun from info?
  {
    vb_.this_ = this;
  }

  // op[]

  const value_get_type operator[](const point_type& p) const
  {
    vb_.p_ = p;
    return vb_;
  }

  // TODO: the code below relies on an implicit assumption
  // this->image_ owns effective data (that can be modified)
  // is this *very* restrictive? => should we define two different
  // types that differ from the impl of "op[] mutable"?
  // why bijfun could not be only used as a direct function?

  // IDEA: f_::abstract::op()(abstract::image) can have two impl
  // one for image with data (the present one) and another to
  // instantiated a through_fun_ image when there is no eff data...

  value_set_type& operator[](const point_type& p)
  {
    vb_.p_ = p;
    return vb_;
  }

  template <class V>
  void set(const V& value)
  {
    this->image_[vb_.p_] = fun_.apply_1(value);
  }

  const value_type get() const
  {
    return fun_.apply(this->image_[vb_.p_]);
  }

};

set_name_TCC(through_bijfun_);



// op()

namespace f_ { namespace abstract {

  template <class F> template <class I>
  through_bijfun_<I, F> bijfun<F>::operator()(const ::abstract::image<I>& ima) const
  {
    // first check that image data is effective
    mlc::neq<oln_type_of(I,value_get), void>::ensure();

    through_bijfun_<I, F> tmp(const_cast<I&>(ima.exact()));
    return tmp;
  }

} } // end of namespaces



#endif
