#include "basics2d.hh"

#include "morpho/erosion.hh"
#include "morpho/dilation.hh"
#include "morpho/opening.hh"
#include "morpho/closing.hh"
#include "morpho/laplacian.hh"
#include "morpho/gradient.hh"
#include "morpho/top_hat.hh"
#include "morpho/geodesic_dilation.hh"
#include "morpho/geodesic_erosion.hh"
#include "morpho/watershed.hh"
#include "morpho/reconstruction.hh"
#include "morpho/extrema.hh"
#include "level/fill.hh"

#include "check.hh"

using namespace oln;
using namespace oln::level;

#define OK_OR_FAIL				\
      cout << "OK" << endl;			\
    else 					\
      {						\
	cout << "FAIL" << endl;			\
	fail = true;				\
      }

bool
check()
{
  bool fail = false;

  image2d<int_u8> lena = load(srcdir + "/../data/lena128.pgm");
  image2d<int_u8> lena_open = morpho::opening(lena, win_c4p());
  image2d<bin> minima_map(lena.size());
  image2d<int_u8> max_map(lena.size());

  level::fill (minima_map, false);
  level::fill (max_map, 255);
  minima_map(10,10) = true;
  minima_map(100,100) = true;

  // test regional minima
  {
    cout << "regional minima sequential ... " << flush;
    if (level::is_equal(morpho::sure_regional_minima(lena, neighb_c4()),
			morpho::sequential_regional_minima(lena, neighb_c4())))
      OK_OR_FAIL;

    cout << "regional minima hybrid ... " << flush;
    if (level::is_equal(morpho::sure_regional_minima(lena, neighb_c4()),
			morpho::hybrid_regional_minima(lena, neighb_c4())))
      OK_OR_FAIL;
  }

  // test minima imposition
  {
    cout << "minima impos sequential ... " << flush;
    if (level::is_equal(morpho::sure_minima_imposition(lena,
						       minima_map, neighb_c4()),
			morpho::sequential_minima_imposition(lena,
							     minima_map,
							     neighb_c4())))
      OK_OR_FAIL;

    cout << "minima impos hybrid ... " << flush;
    if (level::is_equal(morpho::sure_minima_imposition(lena,
						       minima_map, neighb_c4()),
			morpho::hybrid_minima_imposition(lena,
							 minima_map,
							 neighb_c4())))
      OK_OR_FAIL;
  }

  // test geodesic erosion and geodesic dilation
  {
    cout << "geodesic erosion ... " << flush;
    if (level::is_equal(morpho::geodesic_erosion(lena, lena_open, neighb_c4()),
			morpho::simple_geodesic_erosion(lena, lena_open,
							neighb_c4())))
      OK_OR_FAIL;

    cout << "geodesic dilation ... " << flush;
    if (level::is_equal(morpho::geodesic_dilation(lena_open, lena, neighb_c4()),
			morpho::simple_geodesic_dilation(lena_open,
							 lena, neighb_c4())))
      OK_OR_FAIL;
  }



  {
    cout << "sequential G.R. erosion ... " << flush;
    if (level::is_equal(morpho::sequential_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4()),
			morpho::sure_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4())))
      OK_OR_FAIL;

    cout << "hybrid G.R. erosion ... " << flush;
    if (level::is_equal(morpho::hybrid_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4()),
			morpho::sure_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4())))
      OK_OR_FAIL;
  }

  {
    cout << "sequential G.R. dilation ... " << flush;
    if (level::is_equal(morpho::sequential_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4()),
			morpho::sure_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4())))
      OK_OR_FAIL;

    cout << "hybrid G.R. dilation ... " << flush;
    if (level::is_equal(morpho::hybrid_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4()),
			morpho::sure_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4())))
      OK_OR_FAIL;
  }

  return fail;
}
