#include "loadsave.hh"
#include "check.hh"

bool
check(void)
{
  bool fail = false;

  fail |= loadsave< image2d<bin>     >(srcdir + "/../data/lena");
  fail |= loadsave< image2d<int_u8>  >(srcdir + "/../data/lena");
  fail |= loadsave< image2d<nrgb_8>  >(srcdir + "/../data/lena");

  fail |= loadsave< image2d<bin>     >(srcdir + "/../data/lena.pbm");
  fail |= loadsave< image2d<int_u8>  >(srcdir + "/../data/lena.pgm");
  fail |= loadsave< image2d<nrgb_8>  >(srcdir + "/../data/lena.ppm");

  fail |= loadsave< image2d<bin>     >(srcdir + "/../data/lena.ppbm");
  fail |= loadsave< image2d<int_u8>  >(srcdir + "/../data/lena.ppgm");
  fail |= loadsave< image2d<nrgb_8>  >(srcdir + "/../data/lena.pppm");

  //  fail |= loadsave< image2d<int_u16> >(*argv);
  return fail;
}
