#include <oln/basics2d.hh>

#include <oln/morpho/opening.hh>
#include <oln/morpho/geodesic_dilation.hh>
#include <oln/morpho/geodesic_erosion.hh>
#include <oln/morpho/reconstruction.hh>
#include <oln/morpho/extrema.hh>
#include <oln/level/fill.hh>

#include "check.hh"

using namespace oln;
using namespace oln::level;

#define OK_OR_FAIL				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	fail = true;				\
      }

bool
check()
{
  bool fail = false;

  image2d<int_u8> lena = load(data("lena128.pgm"));
  image2d<int_u8> lena_open = morpho::opening(lena, win_c4p());
  image2d<bin> minima_map(lena.size());
  image2d<int_u8> max_map(lena.size());

  level::fill (minima_map, false);
  level::fill (max_map, 255);
  minima_map(10,10) = true;
  minima_map(100,100) = true;

  // test regional minima
  {
    std::cout << "regional minima sequential ... " << std::flush;
    if (level::is_equal(morpho::sure_regional_minima(lena, neighb_c4()),
			morpho::sequential_regional_minima(lena, neighb_c4())))
      OK_OR_FAIL;

    std::cout << "regional minima hybrid ... " << std::flush;
    if (level::is_equal(morpho::sure_regional_minima(lena, neighb_c4()),
			morpho::hybrid_regional_minima(lena, neighb_c4())))
      OK_OR_FAIL;
  }

  // test minima imposition
  {
    std::cout << "minima impos sequential ... " << std::flush;
    if (level::is_equal(morpho::sure_minima_imposition(lena,
						       minima_map, neighb_c4()),
			morpho::sequential_minima_imposition(lena,
							     minima_map,
							     neighb_c4())))
      OK_OR_FAIL;

    std::cout << "minima impos hybrid ... " << std::flush;
    if (level::is_equal(morpho::sure_minima_imposition(lena,
						       minima_map, neighb_c4()),
			morpho::hybrid_minima_imposition(lena,
							 minima_map,
							 neighb_c4())))
      OK_OR_FAIL;
  }

  // test geodesic erosion and geodesic dilation
  {
    std::cout << "geodesic erosion ... " << std::flush;
    if (level::is_equal(morpho::geodesic_erosion(lena, lena_open, neighb_c4()),
			morpho::simple_geodesic_erosion(lena, lena_open,
							neighb_c4())))
      OK_OR_FAIL;

    std::cout << "geodesic dilation ... " << std::flush;
    if (level::is_equal(morpho::geodesic_dilation(lena_open, lena, neighb_c4()),
			morpho::simple_geodesic_dilation(lena_open,
							 lena, neighb_c4())))
      OK_OR_FAIL;
  }



  {
    std::cout << "sequential G.R. erosion ... " << std::flush;
    if (level::is_equal(morpho::sequential_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4()),
			morpho::sure_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4())))
      OK_OR_FAIL;

    std::cout << "hybrid G.R. erosion ... " << std::flush;
    if (level::is_equal(morpho::hybrid_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4()),
			morpho::sure_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4())))
      OK_OR_FAIL;
  }

  {
    std::cout << "sequential G.R. dilation ... " << std::flush;
    if (level::is_equal(morpho::sequential_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4()),
			morpho::sure_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4())))
      OK_OR_FAIL;

    std::cout << "hybrid G.R. dilation ... " << std::flush;
    if (level::is_equal(morpho::hybrid_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4()),
			morpho::sure_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4())))
      OK_OR_FAIL;
  }

  return fail;
}
