#include "loadsave.hh"

bool
check(void)
{
  bool fail = false;

  window2d w = io::load(rdata("se"));
  neighborhood2d n = io::load(rdata("neighbor"));
  image2d<rgb_8> i = io::load(rdata("lena"));

  return fail;
}
