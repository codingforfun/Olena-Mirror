#ifndef MINIOLN_COLOR_HH
# define MINIOLN_COLOR_HH

# include <iostream>
# include "bijfun.hh"
# include "f.hh"
# include "misc.hh"


template <class T, class less>
struct less_mixin : public T
{
  typedef less_mixin<T, less> self_type;
  typedef T super;

  less_mixin()
  {}

  less_mixin(const self_type& rhs) :
    super(rhs)
  {
  }

  less_mixin(const T& rhs)
    : super(rhs)
  {}

  self_type& operator=(const self_type& rhs)
  {
    this->super::operator=(rhs);
    return *this;
  }

  self_type& operator=(const T& rhs)
  {
    this->super::operator=(rhs);
    return *this;
  }

  bool operator<(const T& rhs) const
  {
    return less_fun_(*this, rhs);
  }

  static const less& get_less() { return less_fun_; }
  static void set_less(const less& fun) { less_fun_ = fun; }

  static less less_fun_;

//   void operator=(const self_type&);
};

set_name_TCC(less_mixin);


template <class T, class less>
std::ostream& operator<<(std::ostream& ostr,
			 const less_mixin<T, less>& value)
{
  return ostr << T(value);
}


template <class T, class less>
less less_mixin<T, less>::less_fun_;



struct rgb
{
  unsigned short red, green, blue;

  typedef unsigned short red_type;

  rgb(unsigned short v = 0)
    : red(v), green(v), blue(v)
  {}

  rgb(unsigned short red, unsigned short green, unsigned short blue)
    : red(red), green(green), blue(blue)
  {}
};

set_name(rgb);

std::ostream& operator<<(std::ostream& ostr, const rgb& c)
{
  return ostr << "(" << unsigned(c.red)
	      << "," << unsigned(c.green)
	      << "," << unsigned(c.blue) << ")";
}


struct rgb_less
{
  bool operator()(const rgb& lhs, const rgb& rhs) const
  {
    return
      lhs.red < rhs.red or
      (lhs.red == rhs.red and (lhs.green < rhs.green or
			       (lhs.green == rhs.green and lhs.blue < rhs.blue)));
  }
};

set_name(rgb_less);


namespace f_
{

  template <class T, class less>
  struct add_less : public abstract::bijfun< add_less<T, less> >
  {
    typedef less_mixin<T, less> result_type;

    result_type apply(const T& val) const
    {
      return val;
    }

    T apply_1(const result_type& val) const
    {
      return val;
    }

  };

} // end of namespace f_

set_name_TCC(f_::add_less);


#endif
