#include <oln/basics2d.hh>
#include <oln/morpho/extrema.hh>


using namespace oln;
using namespace ntg;

void morpholena_minima()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");
  
  save(morpho::sure::regional_minima(lena, neighb_c4()),
       "regional_min_sure.pbm");
  save(morpho::sequential::regional_minima(lena, neighb_c4()),
       "regional_min_sequential.pbm");
  save(morpho::hybrid::regional_minima(lena, neighb_c4()),
       "regional_min_hybrid.pbm");
}

void morpholena_minima_imp()
{
  image2d<int_u8> lena = load(IMGDIR "/lena.pgm");

  image2d<bin> minima_map = load(IMGDIR "/map.pbm");
  save(morpho::sure::minima_imposition(lena, minima_map, neighb_c4()),
       "minima_imposition_sure.pgm");
  save(morpho::sequential::minima_imposition(lena, minima_map, neighb_c4()),
       "minima_imposition_sequential.pgm");
  save(morpho::hybrid::minima_imposition(lena, minima_map, neighb_c4()),
       "minima_imposition_hybrid.pgm");

}
