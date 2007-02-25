#include <iostream>
#include "concept3.hh"

int main ()
{
  ::std::cout << "HAAAAAAAAAAAaaaaaaaaaaaaaa....." << ::std::endl;


  ugo::point2d			p1(0, 0);
  ugo::point2d			p2(5, 5);
  ugo::box_<ugo::point2d>	box(p1, p2);
  ugo::image2d			img(box);

  return (0);
}
