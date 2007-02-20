#include <iostream>
#include "grid2.hh"

namespace glg
{
  typedef point2d_<int> point2d;
  typedef box_<point2d> box2d;
  typedef box_iterator_<point2d> box_iterator2d;
}

int main ()
{
  using namespace glg;
  point2d p1(3, 2), p2(7,5), p3(5,4);
  box2d   b(p1, p2);
  box_iterator2d bb(b);
  //  box2d<int> bo(p1, p2);

  std::cout << b.includes(p3) << std::endl;

  return bidon (p1, p2);
}
