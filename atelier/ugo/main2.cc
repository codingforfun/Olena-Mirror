#include <string>
#include <iostream>
#include <mlc/contract.hh>
#include "tracked_ptr.hh"
#include "concept2.hh"

template <typename I>
void   println(I& img)
{
  typename I::iter it(img.bbox);
  for (it.start(); it.is_valid(); it.next())
  {
    std::cout << img(it) << ' ';
    if (((ugo::point2d)it)[0] == img.bbox.pmax[0])
      std::cout << std::endl;
  }
}

int main()
{
  ugo::point2d			p1(0, 0);
  ugo::point2d			p2(5, 5);
  ugo::box_<ugo::point2d>	box(p1, p2);
  ugo::image2d			img(box);

  ugo::image2d::iter		it(box);


  img(ugo::point2d(3, 3)) = 5;
  img(ugo::point2d(2, 3)) = 4;
  img(ugo::point2d(4, 4)) = 7;
  img(ugo::point2d(1, 2)) = 4;

  println<ugo::image2d>(img);

  return(0);
}
