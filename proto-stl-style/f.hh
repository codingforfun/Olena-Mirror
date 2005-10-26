#ifndef MINIOLN_F_HH
# define MINIOLN_F_HH

# include <cmath>

# include "misc.hh"
# include "abstract.hh"
# include "fun.hh"
# include "bijfun.hh"



namespace f_
{

  template <template <class> class F>
  struct mufun
  {
    mufun() {}

    template <class I>
    thru_pw_fun_< I, F<oln_type_of(I, value)> >
    operator()(const ::abstract::image<I>& ima) const
    {
      typedef F<oln_type_of(I, value)> Fun;
      static Fun f = Fun();
      thru_pw_fun_<I, Fun> tmp(ima, f);
      return tmp;
    }
  };

  // - - - - - - - - - - - - - - - - - - - - - - - -  pure functions

  template <class T>
  struct cos : public abstract::fun< cos<T> >
  {
    typedef abstract::fun< cos<T> > super;
    using super::operator();

    typedef float result_type;
    
    float apply(const T& val) const
    {
      return std::cos(val);
    }
  };

  template <class T>
  struct uminus : public abstract::fun< uminus<T> >
  {
    typedef abstract::fun< uminus<T> > super;
    using super::operator();

    typedef T result_type;
    
    T apply(const T& val) const
    {
      return - val;
    }
  };

  template <class T>
  struct div : public abstract::fun< div<T> >
  {
    typedef abstract::fun< div<T> > super;
    using super::operator();

    typedef float result_type;
  
    float denom_;
    div(float denom = 1.f) : denom_(denom) {}
  
    float apply(const T& val) const
    {
      return float(val) / denom_;
    }
  };


  template <class T>
  struct leq : public abstract::fun< leq<T> >
  {
    typedef abstract::fun< leq<T> > super;
    using super::operator();

    typedef bool result_type;
    leq(T value = T(0)) : value_(value) {}
    bool apply(const T& val) const
    {
      return val <= value_;
    }
  protected:
    T value_;
  };


  template <class T>
  struct geq : public abstract::fun< geq<T> >
  {
    typedef abstract::fun< geq<T> > super;
    using super::operator();

    typedef bool result_type;
    geq(T value = T(0)) : value_(value) {}
    bool apply(const T& val) const
    {
      return val >= value_;
    }
  protected:
    T value_;
  };



  // - - - - - - - - - - - - - - - - - - - - - - - -  bijective functions

  template <class T>
  struct inc : public abstract::bijfun< inc<T> >
  {
    typedef T result_type;

    T apply(const T& val) const
    {
      return val + 1;
    }

    T apply_1(const T& val) const
    {
      return val - 1;
    }

  };


} // end of namespace f_

set_name_TC(f_::cos);
set_name_TC(f_::div);
set_name_TC(f_::leq);
set_name_TC(f_::geq);
set_name_TC(f_::inc);

f_::mufun<f_::cos> f_cos; 


#endif
