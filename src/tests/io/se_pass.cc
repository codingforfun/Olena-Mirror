#include "loadsave.hh"

bool
check(void)
{
  bool fail = false;

  // these tests should pass

  fail |= loadsave< neighborhood2d >(data("neighbor.ppbm"), "neighbor.ppbm");
  fail |= loadsave< neighborhood2d >(data("neighbor.pbm"), "neighbor.pbm");

  fail |= loadsave< neighborhood2d >(data("neighbor9.ppbm"), "neighbor9.ppbm");
  fail |= loadsave< neighborhood2d >(data("neighbor9.pbm"), "neighbor9.pbm");

  fail |= loadsave< window2d >(data("se.ppbm"), "se.ppbm");
  fail |= loadsave< window2d >(data("se.pbm"), "se.pbm");

  fail |= loadsave< window2d >(data("se9.ppbm"), "se9.ppbm");
  fail |= loadsave< window2d >(data("se9.pbm"), "se9.pbm");

#if HAVE_ZLIB
  fail |= loadsave< window2d >(data("se9.pbm.gz"), "se9.pbm.gz");
  fail |= loadsave< neighborhood2d >(data("neighbor9.pbm.gz"), "neighbor9.pbm.gz");
#endif

  return fail;
}
