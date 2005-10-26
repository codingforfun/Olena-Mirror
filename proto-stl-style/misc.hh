#ifndef MINIOLN_MISC_HH
# define MINIOLN_MISC_HH

# include <cassert>

# include <string>
# include <sstream>


template <class T>
struct name
{
  static std::string of()
  {
    return "UNKNOWN!";
  }
};

template <class T>
std::string name_of(const T&)
{
  return name<T>::of();
}

# define set_name(NAME) \
template <> struct name <NAME> { static std::string of() { return #NAME; } }

# define set_name_TC(CLASS) \
template <class C> struct name < CLASS<C> > \
{ static std::string of() { return std::string(#CLASS) + "<" + name<C>::of() + ">"; } }

# define set_name_TCC(CLASS) \
template <class C1, class C2> struct name < CLASS<C1,C2> > \
{ static std::string of() { return std::string(#CLASS) + "<" + name<C1>::of() + ", " +  name<C2>::of() + ">"; } }

# define set_name_TCc(CLASS) \
template <class C1, class C2> struct name < CLASS<C1,C2> > \
{ static std::string of() { return std::string(#CLASS) + "<" + name<C1>::of() + ">"; } }

# define set_name_TCcc(CLASS) \
template <class C1, class C2, class C3> struct name < CLASS<C1,C2,C3> > \
{ static std::string of() { return std::string(#CLASS) + "<" + name<C1>::of() + ">"; } }


set_name(std::string);

set_name(void);
set_name(char);
set_name(signed char);
set_name(unsigned char);
set_name(short);
set_name(unsigned short);
set_name(int);
set_name(unsigned);
set_name(long);
set_name(unsigned long);
set_name(float);
set_name(double);
set_name(bool);

namespace std {
  template <class _Tp, class _Alloc>                  class vector;
  template <class _Key, class _Compare, class _Alloc> class set;
  template <class _Tp, class _Alloc>                  class list;
}
set_name_TCc(std::vector);
set_name_TCcc(std::set);
set_name_TCc(std::list);



# define precondition  assert
# define postcondition assert
# define invariant     assert




// useful macro to explicitly say that a typedef is deduced from another
// (nota bene: end with a semicolumn and do *not* omit the _type termination)

# define typedef_deduced(From_Type, Deduced_Typedef) \
typedef  typename From_Type::Deduced_Typedef  Deduced_Typedef

# define typedef_deduced_(From_Type, Deduced_Typedef) \
typedef  From_Type::Deduced_Typedef  Deduced_Typedef


# define typedef_super(Deduced_Typedef) \
typedef  typename super::Deduced_Typedef  Deduced_Typedef




template <class E>
struct min_acc
{
  min_acc()
    : init_(false)
  {
  }
  min_acc(const E& value)
    : init_(true),
      value_(value)
  {
  }
  min_acc<E>& operator()(const E& value)
  {
    if (not init_)
      {
	init_ = true;
	value_ = value;
      }
    else
      {
	if (value < value_)
	  value_ = value;
      }
    return *this;
  }
  operator const E&() const
  {
    precondition(init_);
    return value_;
  }
protected:
  bool init_;
  E value_;
};


template <class E>
struct max_acc
{
  max_acc()
    : init_(false)
  {
  }
  max_acc(const E& value)
    : init_(true),
      value_(value)
  {
  }
  max_acc<E>& operator()(const E& value)
  {
    if (not init_)
      {
	init_ = true;
	value_ = value;
      }
    else
      {
	if (value > value_)
	  value_ = value;
      }
    return *this;
  }
  operator const E&() const
  {
    precondition(init_);
    return value_;
  }
protected:
  bool init_;
  E value_;
};


#endif
