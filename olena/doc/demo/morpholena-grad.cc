#include <oln/basics2d.hh>
#include <oln/morpho/gradient.hh>

using namespace oln;

void morpholena_grad()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");

  save(morpho::beucher_gradient(lena, win_c8p()), "lena-beg.pgm");
  save(morpho::internal_gradient(lena, win_c8p()), "lena-ing.pgm");
  save(morpho::external_gradient(lena, win_c8p()), "lena-exb.pgm");
  save(morpho::fast::beucher_gradient(lena, win_c8p()), "lena-beg-f.pgm");
  save(morpho::fast::internal_gradient(lena, win_c8p()), "lena-ing-f.pgm");
  save(morpho::fast::external_gradient(lena, win_c8p()), "lena-exb-f.pgm");
}
