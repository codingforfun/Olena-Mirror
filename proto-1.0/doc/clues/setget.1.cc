#include <assert.h>
#include <iostream>


typedef int value_type;
typedef int point_type;


template <typename I>
struct value_box
{
  I& ima;
  point_type p;

  value_box(I& ima, point_type p) :
    ima(ima), p(p)
  {}

  value_box& operator=(value_type v)
  {
    ima.set(p, v);
    return *this;
  }
  operator value_type() const
  {
    return ima.get(p);
  }
};

template <typename I>
std::ostream& operator<<(std::ostream& ostr, const value_box<I>& v)
{
  return ostr << value_type(v);
}


struct dummy_image
{
  value_type data;

  dummy_image()
  {
    data = 3;
  }
  value_box<dummy_image> operator[](point_type p)
  {
    return value_box<dummy_image>(*this, p);
  }
  const value_type get(point_type) const
  {
    std::cout << "get" << std::endl;
    return data;
  }
  void set(point_type p, value_type v)
  {
    std::cout << "set" << std::endl;
    data = v;
  }
};

int main()
{
  dummy_image ima;
  point_type p = 1;
  ima[p] = 3 * ima[p] * ima[p];
  std::cout << ima[p] << std::endl;
}
