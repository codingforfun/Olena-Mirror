#include <iostream>

#include "2-scoop2.hh"
#include "3-iterator.hh"
#include "5-set.hh"


int		main()
{
  ugo::point2d_<int>	p1;
  ugo::point2d_<int>	p2;

  typedef ugo::point2d_<int> point_t;

  mlc::assert_<mlc_eq(point_t::grid, ugo::grid2d)>::check();

  assert (ugo::bidon(p1, p2));

  p1.row = 5;
  return (ugo::bidon(p1, p2));
}
