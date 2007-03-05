#include <ostream>
#include "point_scoop1.hh"


int main()
{
  oln::point2d_<int> p1 (0, 0), p2 (5, 8), p5(1, 1), p6(4, 4);

  oln::point1d p3;
  oln::point1d p4;

  p3.col = 0;
  p4.col = 5;


  oln::simple_iter<int> iter (p1);

  oln::box2d  box(p1, p2);
  oln::box2d  box2(p5, p6);
  oln::box1d  box1(p3, p4);

  oln::iter2d toto (box);
  oln::pset_std< oln::point2d_<int> > titi (p1, p2);

  oln::image2d<int> ima2d (box);
  oln::polite_image< oln::image2d<int> > polite (ima2d);
  oln::signal<int> ima1d (box1);
//   oln::pset_std_iterator_< oln::point2d_<int> > tutu (titi);

  oln::value_cast_image<float, oln::image2d<int> > casted (ima2d);
  //  print(casted);
  print(ima2d);

  oln::singleton<int> single;
  oln::vec<4, int> vectorr;
  vectorr[0] = 1;
  vectorr[1] = 1;
  vectorr[2] = 1;
  vectorr[3] = 1;
  std::cout << vectorr << std::endl;


  polite.talk();
  print(polite);

  oln::image_stack< 2, oln::image2d<int> > s;

  s.image(0) = ima2d;
  s.image(1) = ima2d;
  print(s);

  oln::chessboard_t chess2d;
  oln::subset_image_1<oln::image2d<int> , oln::chessboard_t> masked (ima2d, chess2d);
  oln::point2d x (0, 1);
  if (!((ima2d | chess2d).has(x)))
    std::cout << "ok" << std::endl;
  else
    std::cout << "ko" << std::endl;
  print(masked);

  oln::subset_image_2<oln::image2d<int>, oln::box2d> masked2(ima2d, box2);

  if (!(masked2).has(x))
    std::cout << "ok" << std::endl;
  else
    std::cout << "ko" << std::endl;

  print(masked2);
  return bidon(p3, p4);
}
