#include <oln/basics2d.hh>
#include <oln/morpho/top_hat.hh>

using namespace oln;

void morpholena_fast_hats()
{
  image2d<int_u8> lena = load(IMGDIR2 "/lena.pgm");

  save(morpho::fast::white_top_hat(lena, win_c8p()), "lena-wth-f.pgm");
  save(morpho::fast::black_top_hat(lena, win_c8p()), "lena-bth-f.pgm");
  save(morpho::fast::self_complementary_top_hat(lena, win_c8p()),
       "lena-sth-f.pgm");
  save(morpho::fast::top_hat_contrast_op(convert::bound<int_u8>(),
					 lena, win_c8p()), "lena-thc-f.pgm");
}
