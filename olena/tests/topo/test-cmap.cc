#include <oln/basics2d.hh>
#include <ntg/all.hh>

// Always include system headers after Olena.
#include <iostream>

#include <oln/topo/cmap.hh>

#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace topo::cmap;

bool
check()
{
  bool fail = false;

  image2d<ntg::int_u8> src = load(rdata("lena128.pgm"));

  std::cout << "building cmap ... " << std::flush;
  cmap< image2d<ntg::int_u8> > cm(src);

  std::cout << "OK" << std::endl;

  return fail;
}
