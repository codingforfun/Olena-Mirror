#include "grid2.hh"

int main ()
{
  glg::point2d_<int> p1(3, 4), p2(7,2);
  glg::box_<glg::point2d_<int> > b(p1, p2);

  return bidon (p1, p2);
}
