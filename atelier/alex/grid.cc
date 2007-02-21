#include <iostream>
#include "grid2.hh"

namespace glg
{
  typedef box_iterator_<point2d> box_iterator2d;
}

int main ()
{
  using namespace glg;
  point2d p1(0, 0), p2(3, 3), p3(5, 4);
  box2d   b(p1, p2);
  box_iterator2d bb(b);
  pset_std_<point2d> tst;
  image2d<int> aie(b);
  //  box2d<int> bo(p1, p2);

  std::cout << b.includes(p3) << std::endl;

  print(aie);

  return bidon (p1, p2);
}
