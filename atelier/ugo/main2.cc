#include <string>
#include <iostream>
#include <mlc/contract.hh>
#include "tracked_ptr.hh"
#include "concept2.hh"

template <typename I>
void   println(const ugo::Image<I>& input)      {
  typename I::iter it(input.bbox);
  for (it.start(); it.is_valid(); it.next())
    std::cout << input(it) << ' ' << std::endl;
}

int main()
{
  ugo::point2d			p1(0, 0);
  ugo::point2d			p2(5, 5);
  ugo::box_<ugo::point2d>	box(p1, p2);
  ugo::image2d			img(box);

  ugo::image2d::iter		it(box);

  for (it.start(); it.is_valid(); it.next())
    std::cout << img(it) << ' ' << std::endl;

  return(0);
}
