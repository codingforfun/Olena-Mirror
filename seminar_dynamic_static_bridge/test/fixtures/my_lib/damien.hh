#include <iostream>
#include "mlc.hh"

struct up
{
  up(int i) : i_(i) {}
  virtual void print(std::ostream&) const = 0;
  virtual void print_noarg() const = 0;
  virtual ~up() {};
  int i_;
};


template <typename T>
struct down : public up
{
  down(const T& t) : up(0), t_(t) {}

  virtual void print(std::ostream& ostr) const
  {
    ostr << "down< " << t_ << " >";
  }

  virtual void print_noarg() const
  {
    std::cout << t_ << std::endl;
  }

  const T& t_;

  virtual ~down() {}
};

template <typename T>
down<T>
mk_down(const T& t)
{
  return t;
}

std::ostream& operator<<(std::ostream& ostr, const up& obj)
{
  obj.print(ostr);
  return ostr;
}

mlc_set_name_TC(down);

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