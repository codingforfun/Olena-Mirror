#include <oln/basics2d.hh>
#include <oln/morpho/closing.hh>
#include <oln/level/fill.hh>
#include <oln/morpho/watershed.hh>
#include <oln/morpho/gradient.hh>

using namespace oln;

void morpholena_watershed_beucher()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");

  image2d<int_u8> markers(lena.size());
  level::fill(markers, 0);

  for (int i = 0; i < 10; ++i)
    for (int j = 0; j < 10; ++j)
      {
	markers(477 + i, 67 + j) = 72;
	markers(100 + i, 200 + j) = 20;
	markers(50 + i, 50 + j) = 60;
	markers(100 + i, 500 + j) = 200;
	markers(255 + i, 355 + j) = 120;
      }

  save
    (morpho::watershed_seg_or
     (morpho::closing
      (morpho::beucher_gradient(lena, win_c8p()), win_c8p()), markers,
      neighb_c8()),
     "lena-wor.pgm");
}
