#include <oln/basics2d.hh>
#include <oln/morpho/thinning.hh>
#include <oln/morpho/thickening.hh>

using namespace oln;
using namespace ntg;

void morpholena_thin_thick()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");

  window2d mywin;
  mywin
    .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
    .add(-2,-1).add(-2,0).add(-2,1)
    .add(-1,0);
  window2d mywin2 = - mywin;

  save(morpho::fast::thinning(lena, mywin, mywin2), "lena-thin.pgm");
  save(morpho::fast::thickening(lena, mywin, mywin2), "lena-thick-f.pgm");

}
