#ifndef MLC
# define MLC

# include <string>


# define mlc_set_name(NAME) \
template <> struct mlc_name <NAME> { static std::string of() { return #NAME; } }

# define mlc_set_name_TC(CLASS) \
template <class C> struct mlc_name < CLASS<C> > \
{ static std::string of() { return std::string(#CLASS) + "<" + mlc_name<C>::of() + ">"; } }

# define mlc_set_name_TCC(CLASS) \
template <class C1, class C2> struct mlc_name < CLASS<C1,C2> > \
{ static std::string of() { return std::string(#CLASS) + "<" + mlc_name<C1>::of() + ", " +  mlc_name<C2>::of() + ">"; } }

# define mlc_set_name_TCc(CLASS) \
template <class C1, class C2> struct mlc_name < CLASS<C1,C2> > \
{ static std::string of() { return std::string(#CLASS) + "<" + mlc_name<C1>::of() + ">"; } }

# define mlc_set_name_TCcc(CLASS) \
template <class C1, class C2, class C3> struct mlc_name < CLASS<C1,C2,C3> > \
{ static std::string of() { return std::string(#CLASS) + "<" + mlc_name<C1>::of() + ">"; } }


template <class T>
struct mlc_name
{
  static std::string of()
  {
    return "UNKNOWN!";
  }
};

template <class T>
std::string mlc_name_of(const T&)
{
  return mlc_name<T>::of();
}

// ptr
template <typename T>
struct mlc_name <T*>
{
  static std::string of() { return mlc_name<T>::of() + "*"; }
};


// const
template <typename T>
struct mlc_name <const T>
{
  static std::string of() { return std::string("const ") + mlc_name<T>::of(); }
};

// ref
template <typename T>
struct mlc_name <T&>
{
  static std::string of() { return mlc_name<T>::of() + "&"; }
};

// built-in

mlc_set_name(void);
mlc_set_name(char);
mlc_set_name(signed char);
mlc_set_name(unsigned char);
mlc_set_name(short);
mlc_set_name(unsigned short);
mlc_set_name(int);
mlc_set_name(unsigned);
mlc_set_name(long);
mlc_set_name(unsigned long);
mlc_set_name(float);
mlc_set_name(double);
mlc_set_name(bool);

// some std

namespace std {
  template <class _Tp, class _Alloc>                  class vector;
  template <class _Key, class _Compare, class _Alloc> class set;
  template <class _Tp, class _Alloc>                  class list;
}
mlc_set_name_TCc(std::vector);
mlc_set_name_TCcc(std::set);
mlc_set_name_TCc(std::list);
mlc_set_name(std::string);

#endif

