#include "loadsave.hh"
#include "check.hh"

bool
check(void)
{
  bool fail = false;

  fail |= loadsave< image2d<bin>     >(data("lena"));
  fail |= loadsave< image2d<int_u8>  >(data("lena"));
  fail |= loadsave< image2d<nrgb_8>  >(data("lena"));

  fail |= loadsave< image2d<bin>     >(data("lena.pbm"));
  fail |= loadsave< image2d<int_u8>  >(data("lena.pgm"));
  fail |= loadsave< image2d<nrgb_8>  >(data("lena.ppm"));

  fail |= loadsave< image2d<bin>     >(data("lena.ppbm"));
  fail |= loadsave< image2d<int_u8>  >(data("lena.ppgm"));
  fail |= loadsave< image2d<nrgb_8>  >(data("lena.pppm"));

  //  fail |= loadsave< image2d<int_u16> >(*argv);
  return fail;
}
