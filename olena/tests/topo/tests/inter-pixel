#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/topo/inter-pixel/inter-pixel.hh>

#include <iostream>

#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace ntg;
using namespace topo;
using namespace inter_pixel;

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

  image2d<int_u8> src = load(rdata("test-cmap"));

  interpixel< image2d<int_u8> > ip(src);

  std::cout << "OK" << std::endl;

  return fail;
}
