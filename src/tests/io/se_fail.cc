#include "loadsave.hh"

bool
check(void)
{
  bool fail = false;

  // these tests should fail

  fail |= ! loadsave< neighborhood2d >(data("se9.ppbm"), "se9.ppbm");
  fail |= ! loadsave< neighborhood2d >(data("se9.pbm"), "se9.pbm");

  fail |= ! loadsave< neighborhood2d >(data("se.ppbm"), "se.ppbm");
  fail |= ! loadsave< neighborhood2d >(data("se.pbm"), "se.pbm");

#if HAVE_ZLIB
  fail |= ! loadsave< neighborhood2d >(data("neighbor9.pbm.gz"), "neighbor9.pbm.gz");
#endif

  return fail;
}
