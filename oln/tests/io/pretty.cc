#include "loadsave.hh"

bool
check(void)
{
  bool fail = false;

  window2d w = io::load(data("se"));
  neighborhood2d n = io::load(data("neighbor"));
  image2d<rgb_8> i = io::load(data("lena"));

  return fail;
}
