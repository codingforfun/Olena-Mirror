#include <iostream>

struct up
{
  virtual void print(std::ostream&) const = 0;
};


template <typename T>
struct down : public up
{
  down(const T& t) : t_(t) {}

  virtual void print(std::ostream& ostr) const
  {
    ostr << t_ << std::endl;
  }
  const T& t_;

};

std::ostream& operator<<(std::ostream& ostr, const up& obj)
{
  obj.print(ostr);
  return ostr;
}

template <class T> struct mlc_name;

template <class T>
struct mlc_name<down<T> >
{
  static std::string of()
  {
    return "down< " + mlc_name<T>::of() + " >";
  }
};


void foo(const up& a)
{
  std::cout << a << std::endl;
}

void foo(const int& i)
{
  std::cout << i << std::endl;
}
