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

  image2d<ntg::int_u8> src = load(rdata("test-cmap.pgm"));

  std::cerr << "building cmap ... " << std::endl;
  cmap< image2d<ntg::int_u8> > cm(src);

  std::cerr << cm << std::endl;

  std::cerr << "OK" << std::endl;

  return fail;
}
