#include <oln/basics2d.hh>
#include <ntg/all.hh>

#include "check.hh"
#include "data.hh"

#include <iostream>

using namespace oln;
using namespace ntg;

bool
check(void)
{
  bool fail = false;

  image2d<bin> im = io::load(rdata("black.pbm"));
  image2d<bin>::iter_type it(im);
  for_all (it)
    assert(im[it] == bin(0));

  im = io::load(rdata("white.pbm"));  
  for_all (it)
    assert(im[it] == bin(1));

  return fail;
}
