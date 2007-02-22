#include <iostream>
#include "concept.hh"

typedef ugo::point2d_<int> point2d;
typedef ugo::signal<int> signal;
typedef ugo::point1d_<int> point1d;

int					main()
{
  point2d			p1;
  point2d			p2;
  point2d			p;

  ugo::point_set_2d<point2d>	sp;
  ugo::box_<point2d>		box;
  ugo::image2d<point2d>		image(box);

  point1d	p3(5);
  point1d	p4(0);

  ugo::box_<point1d>		box1(p3, p4);
  ugo::box_<point1d>		box2(p4, p3);

  signal	s1(box1);
  signal	s2(box2);

  mlc::assert_<mlc_eq(point2d::grid, ugo::grid2d)>::check();

  assert (ugo::bidon(p1, p2));

  return (ugo::bidon(p1, p2));
}
