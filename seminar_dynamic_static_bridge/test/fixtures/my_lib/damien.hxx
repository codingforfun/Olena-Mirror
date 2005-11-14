#ifndef DAMIEN_HXX
#define DAMIEN_HXX

#include <iostream>
#include "mlc.hh"
#include "damien.hh"

template <typename T>
const T* down<T>::get_t() const { return &t_; }

template <typename T>
const down<T>* down<T>::clone()
{
  return new down<T>(t_);
}

template <typename T>
down<T>
mk_down(const T& t)
{
  return t;
}

void foo(const up& a)
{
  std::cout << "foo(" << a << ")" << std::endl;
}

void foo(const int& i)
{
  std::cout << "foo(" << i << ")" << std::endl;
}

void bar(const up& a, const up& b, int c=0)
{
  std::cout << "bar(" << a << ", " << b << ", " << c << ")" << std::endl;
}

void change(up& a)
{
  a.i_++;
}


template <typename T>
const T& deref(const T* ptr)
{
  return *ptr;
}

#endif

