#include <iostream>
#include "concept3.hh"

template <typename I>
void   println(I& img)
{
  typename I::iter it(img);
  for (it.start(); it.is_valid(); it.next())
  {
    std::cout << img(it) << ' ';
    if (((ugo::point2d)it)[0] == img.bbox.pmax[0])
      std::cout << std::endl;
  }
}


int main ()
{
  ::std::cout << "HAAAAAAAAAAAaaaaaaaaaaaaaa....." << ::std::endl;


  ugo::point2d			p1(0, 0);
  ugo::point2d			p2(5, 5);
  ugo::box_<ugo::point2d>	box(p1, p2);
  ugo::image2d			img(box);


  typedef ugo::image2d_<int> I;

  ugo::point2d p(0, 0), q(1, 1);
  ugo::box2d b(p, q);

  I ima_0(b), ima_1(b);
  ima_0(p) = 5;
  ima_1(p) = 1;

  println(ima_0);
  println(ima_1);

  ugo::image_stack<2,I> s;
//  s.image(0) = ima_0;
//  s.image(1) = ima_1;
  //  ou directement :
  //image_stack<2,I> s = make_stack(ima_0, ima_1);

  //  println(s);

//  s(p) = make_vec(6, 9);
//  std::cout << s(p)     << ' '
//	    << ima_0(p) << ' '
//	    << ima_1(p) << ' ' << std::endl;

  return (0);
}
