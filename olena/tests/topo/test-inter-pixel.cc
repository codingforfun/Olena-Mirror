#include <oln/basics2d.hh>
#include <ntg/all.hh>
#include <oln/topo/inter-pixel/inter-pixel.hh>

#include <iostream>

#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace ntg;
using namespace topo::inter_pixel;

bool
check()
{
  bool fail = false;

  image2d<int_u8> src = load(rdata("test-cmap.pgm"));

  std::cerr << "building inter-pixel representation ... " << std::endl;
  interpixel< image2d<int_u8> > ip(src);

  std::cerr << ip << std::endl;

  std::cerr << "OK" << std::endl;

  return fail;
}
