#include "loadsave.hh"

bool
check(void)
{
  bool fail = false;

  fail |= loadsave< image2d<bin>     >(data("lena"), "lena");
  fail |= loadsave< image2d<int_u8>  >(data("lena"), "lena");
  fail |= loadsave< image2d<nrgb_8>  >(data("lena"), "lena");

  fail |= loadsave< image2d<bin>     >(data("lena.pbm"), "lena.pbm");
  fail |= loadsave< image2d<int_u8>  >(data("lena.pgm"), "lena.pgm");
  fail |= loadsave< image2d<nrgb_8>  >(data("lena.ppm"), "lena.ppm");

  fail |= loadsave< image2d<bin>     >(data("lena.ppbm"), "lena.ppbm");
  fail |= loadsave< image2d<int_u8>  >(data("lena.ppgm"), "lena.ppgm");
  fail |= loadsave< image2d<nrgb_8>  >(data("lena.pppm"), "lena.pppm");

#if HAVE_ZLIB
  fail |= loadsave< image2d<bin>     >(data("lena.pbm.gz"), "lena.pbm.gz");
  fail |= loadsave< image2d<int_u8>  >(data("lena.pgm.gz"), "lena.pgm.gz");
  fail |= loadsave< image2d<nrgb_8>  >(data("lena.ppm.gz"), "lena.ppm.gz");
#endif

  //  fail |= loadsave< image2d<int_u16> >(*argv);
  return fail;
}
