#include <iostream>
#include "grid2.hh"

namespace glg
{
  typedef box_iterator_<point2d> box_iterator2d;
  typedef box_iterator_<point1d> box_iterator1d;
}

int main ()
{
  using namespace glg;
  point2d p1(0, 0), p2(3, 3), p3(5, 4);
  point1d pp1(2), pp2(7);
  box2d   b(p1, p2);
  box1d   c(pp1, pp2);
  box_iterator2d bb(b);
  box_iterator1d bc(c);
  pset_std_<point2d> tst;
  image2d<int> aie(b);
  signal<int> ouille(c);
//  box2d<int> bo(p1, p2);

  std::cout << b.includes(p3) << std::endl;

  print(aie);

  std::cout << b.includes(p3) << std::endl;

  print(ouille);

  return bidon (p1, p2);
}
