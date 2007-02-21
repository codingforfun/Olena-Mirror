#include <iostream>
#include "concept.hh"


int		main()
{
  ugo::point2d_<int>	p1;
  ugo::point2d_<int>	p2;
  ugo::point2d	p;
  ugo::point_set_2d<ugo::point2d> sp;
  ugo::box_<ugo::point2d> box;
  ugo::image2d<int>	  image(box);

  typedef ugo::point2d point_t;

  mlc::assert_<mlc_eq(point_t::grid, ugo::grid2d)>::check();

  assert (ugo::bidon(p1, p2));

  p1.row = 5;
  return (ugo::bidon(p1, p2));
}
