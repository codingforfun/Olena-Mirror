#include <oln/basics2d.hh>
#include <oln/morpho/extrema.hh>


using namespace oln;

void morpholena_minima()
{
  image2d<int_u8> lena = load(IMGDIR2 "/lena.pgm");
  
  save(morpho::sure::regional_minima(lena, neighb_c4()),
       "regional_min_sure.pbm");
  save(morpho::sequential::regional_minima(lena, neighb_c4()),
       "regional_min_sequential.pbm");
  save(morpho::hybrid::regional_minima(lena, neighb_c4()),
       "regional_min_hybrid.pbm");
}
