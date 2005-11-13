#include <iostream>
#include "mlc.hh"

struct up
{
  virtual void print(std::ostream&) const = 0;
  virtual void print_noarg() const = 0;
  virtual ~up() {};
};


template <typename T>
struct down : public up
{
  down(const T& t) : t_(t) {}

  virtual void print(std::ostream& ostr) const
  {
    ostr <<  t_;
  }

  virtual void print_noarg() const
  {
    std::cout << t_ << std::endl;
  }

  const T& t_;

  virtual ~down() {}
};

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
