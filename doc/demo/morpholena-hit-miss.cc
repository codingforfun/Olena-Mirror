#include <oln/basics2d.hh>
#include <oln/morpho/hit_or_miss.hh>

using namespace oln;

void morpholena_hit_or_miss()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");

  window2d mywin;
  mywin
    .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
    .add(-2,-1).add(-2,0).add(-2,1)
    .add(-1,0);
  window2d mywin2 = - mywin;

  save(morpho::hit_or_miss(lena, mywin, mywin2), "lena-hom.pgm");
  save(morpho::fast::hit_or_miss(lena, mywin, mywin2), "lena-hom-f.pgm");
}
