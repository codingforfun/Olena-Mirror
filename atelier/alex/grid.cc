#include <iostream>
#include "grid2.hh"

int main ()
{
  glg::point2d_<int> p1(3, 2), p2(7,5), p3(5,4);
  glg::box_<glg::point2d_<int> > b(p1, p2);
  //  glg::box2d<int> bo(p1, p2);

  std::cout << b.includes(p3) << std::endl;

  return bidon (p1, p2);
}
