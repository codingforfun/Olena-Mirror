#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/topo/dmap.hh>

#include <iostream>

#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace ntg;
using namespace topo;

#define OK_OR_FAIL				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	fail = true;				\
      }

bool
check()
{
  bool fail = false;

  chamfer<int> ch = cityblock();

  image2d<int_u8> src = load(rdata("lena256.pgm"));

  dmap<int_u8, int> d(src.size(), ch);

  d.compute(src);

  std::cout << "OK" << std::endl;

  return fail;
}
