#include <oln/basics2d.hh>
#include <oln/morpho/watershed.hh>

using namespace oln;

void morpholena_watershed()
{
  image2d<int_u8> lena = load(IMGDIR2 "/lena.pgm");

  neighborhood2d n = neighb_c4 ();
  save(morpho::watershed_seg<int_u8>(lena, n), "lena-wss.pgm");
  save(morpho::watershed_con<int_u8>(lena, n), "lena-wsc.pgm");

}
