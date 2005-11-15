// part of generic lib

#include <cmath>
#include <iostream>
#include "name_of.hh"

template <class T>
struct u
{
  u(const T& x) : x_(x) {}
  const T& x_;
};

mlc_set_name_TC(u);

template <class T, class V>
struct t
{
  t<T, V>(const T& x, const V& y) : x_(x), y_(y) {}
  const T& x_, y_;
};

mlc_set_name_TCC(t);

template <class T>
std::ostream& operator<< (std::ostream& ostr, const u<T>& x)
{
  return ostr << "u< " << x.x_ << " >";
}

template <class T, class V>
std::ostream& operator<< (std::ostream& ostr, const t<T, V>& x)
{
  return ostr << "t< " << x.x_ << ", " << x.y_ << " >";
}

void foo1()
{
  std::cout << "foo1()" << std::endl;
}

int* foo2()
{
  std::cout << "foo2() => 42" << std::endl;
  return new int(42);
}

template <class T>
T* foo3(T t1, const T t2, const T& t3)
{
  std::cout << "foo3(" << t1 << ", " << t2 << ", " << t3 << ")" << std::endl;
}

namespace my_lib
{
  namespace x
  {
    template <template<class, class> class T, template<class> class U >
    U<char>*** foo4(U<float> a, T< T<char, char>, U<const int> > b)
    {
      std::cout << "foo4(" << a << ", " << b << ")" << std::endl;
    }
  } // end of x

  template <class T>
  T sqr(const T& t)
  {
    std::cout << "sqr(" << t << ") => " << t * t << std::endl;
    return t * t;
  }

  template <class T>
  float power(float f, const T& t)
  {
    std::cout << "power(" << f << "," << t << ")" << std::endl;
    return std::exp(t * std::log(f));
  }

} // end of my_lib
