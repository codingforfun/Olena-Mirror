#include <oln/basics2d.hh>
#include <ntg/all.hh>

// Always include system headers after Olena.
#include <iostream>
#include <cmath>

#include <oln/topo/inter-pixel.hh>

#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace ntg;
using namespace topo::boundary;

bool
check()
{
  bool fail = false;

  image2d<int_u8> src = load(rdata("lena128.pgm"));

  std::cout << "building inter-pixel representation ... " << std::flush;
  interpixel< image2d<int_u8> > ip(src);

  std::cout << "OK" << std::endl;

  return fail;
}
