#include <oln/basics2d.hh>
#include <oln/morpho/closing.hh>
#include <oln/morpho/opening.hh>


using namespace oln;

void morpholena_opening_closing()
{
  image2d<int_u8> lena = load(IMGDIR2 "/lena.pgm");

  window2d mywin;
  mywin
    .add(-3,-2).add(-3,-1).add(-3,0).add(-3,1).add(-3,2)
    .add(-2,-1).add(-2,0).add(-2,1)
    .add(-1,0);
  save(morpho::closing(lena, mywin), "lena-myw.pgm");

  window2d face_se = load(IMGDIR "/face_se.pbm");
  save(morpho::fast::opening(lena,  face_se), "lena-ope-f-face.pgm");

}
