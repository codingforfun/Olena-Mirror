#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/erosion.hh>
#include <oln/morpho/dilation.hh>
#include <oln/morpho/opening.hh>
#include <oln/morpho/closing.hh>
#include <oln/morpho/laplacian.hh>
#include <oln/morpho/gradient.hh>
#include <oln/morpho/top_hat.hh>
#include <oln/morpho/hit_or_miss.hh>
#include <oln/morpho/watershed.hh>
#include <oln/morpho/thinning.hh>
#include <oln/morpho/thickening.hh>
#include <oln/convol/fast_gaussian.hh>
#include <oln/level/fill.hh>
#include <iostream>

using namespace oln;

#ifndef DATA_TYPE
# error
#endif

#ifndef IMG_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " filename_in filename_out"
            << std::endl;
  std::cerr << "\tex: " << progname
            << " lena.pgm lena_ero.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
    usage(argv[0]);
  IMG_TYPE<DATA_TYPE> img_in = load(argv[1]);
  save(morpho::FUNC(img_in, win_c8p()), argv[2]);
  return 0;
}

