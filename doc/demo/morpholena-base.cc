#include <oln/basics2d.hh>
#include <oln/morpho/erosion.hh>
#include <oln/morpho/dilation.hh>
#include <oln/morpho/laplacian.hh>

using namespace oln;

void morpholena_base()
{
  image2d<int_u8> lena = load(IMGDIR2 "/lena.pgm");

  save(morpho::erosion(lena,  win_c8p()), "lena-ero.pgm");
  save(morpho::dilation(lena, win_c8p()), "lena-dil.pgm");
  save(morpho::laplacian(convert::force<int_u8>(), lena, win_c8p()),
       "lena-lap.pgm");
  save(morpho::fast::erosion(lena,  win_c8p()), "lena-ero-f.pgm");
  save(morpho::fast::dilation(lena, win_c8p()), "lena-dil-f.pgm");
  save(morpho::fast::laplacian(convert::force<int_u8>(), lena, win_c8p()),
       "lena-lap-f.pgm");
}
