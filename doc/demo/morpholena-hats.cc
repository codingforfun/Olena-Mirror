#include <oln/basics2d.hh>
#include <oln/morpho/top_hat.hh>

using namespace oln;

void morpholena_hats()
{
  image2d<int_u8> lena = load(IMGDIR2 "/lena.pgm");

  save(morpho::white_top_hat(lena, win_c8p()), "lena-wth.pgm");
  save(morpho::black_top_hat(lena, win_c8p()), "lena-bth.pgm");
  save(morpho::self_complementary_top_hat(lena, win_c8p()),
       "lena-sth.pgm");
  save(morpho::top_hat_contrast_op(convert::bound<int_u8>(),
				   lena, win_c8p()), "lena-thc.pgm");
}
