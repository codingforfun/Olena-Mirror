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

#include "level/extrema_killer.hh"

#include "utils/timer.hh"

using namespace oln;
using namespace oln::level;

int main()
{
  image2d<int_u8> lena = load("../img/lena128.pgm");
  image2d<int_u8> lena_open = morpho::opening(lena, win_c4p());
  image2d<bin> minima_map(lena.size());
  image2d<int_u8> max_map(lena.size());

  level::fill (minima_map, false);
  level::fill (max_map, 255);
  minima_map(10,10) = true;
  minima_map(100,100) = true;


  // test regional minima
  {
    if (level::is_equal(morpho::sure_regional_minima(lena, neighb_c4()),
			morpho::sequential_regional_minima(lena, neighb_c4())))
      cout << "regional minima sequential ok " << endl;
    else
      cout << "regional minima sequential bad " << endl;

    if (level::is_equal(morpho::sure_regional_minima(lena, neighb_c4()),
			morpho::hybrid_regional_minima(lena, neighb_c4())))
      cout << "regional minima hybrid ok " << endl;
    else
      cout << "regional minima hybrid bad " << endl;
  }

  save(morpho::hybrid_regional_minima(lena, neighb_c4()), "mini.pgm");

  // test minima imposition
  {
    if (level::is_equal(morpho::sure_minima_imposition(lena,
						       minima_map, neighb_c4()),
			morpho::sequential_minima_imposition(lena,
							     minima_map,
							     neighb_c4())))
      cout << "minima impos sequential ok " << endl;
    else
      cout << "minima impos sequential bad " << endl;

    if (level::is_equal(morpho::sure_minima_imposition(lena,
						       minima_map, neighb_c4()),
			morpho::hybrid_minima_imposition(lena,
							 minima_map,
							 neighb_c4())))
      cout << "minima impos hybrid ok " << endl;
    else
      cout << "minima impos hybrid bad " << endl;
  }

  // test geodesic erosion and geodesic dilation
  {
    if (level::is_equal(morpho::geodesic_erosion(lena, lena_open, neighb_c4()),
			morpho::simple_geodesic_erosion(lena, lena_open,
							neighb_c4())))
      cout << "geo ero ok " << endl;
    else
      cout << "geo ero bad " << endl;

    if (level::is_equal(morpho::geodesic_dilation(lena_open, lena, neighb_c4()),
			morpho::simple_geodesic_dilation(lena_open,
							 lena, neighb_c4())))
      cout << "geo dil ok " << endl;
    else
      cout << "geo dil bad " << endl;
  }



  {
    cout << "EROSION " << endl;
    if (level::is_equal(morpho::sequential_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4()),
			morpho::sure_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4())))
      cout << "seq ok " << endl;
    else
      cout << "seq bad " << endl;

    if (level::is_equal(morpho::hybrid_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4()),
			morpho::sure_geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4())))
      cout << "hybrid ok " << endl;
    else
      cout << "hybrid bad " << endl;
  }

  {
    cout << "DILATION " << endl;
    if (level::is_equal(morpho::sequential_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4()),
			morpho::sure_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4())))
      cout << "seq ok " << endl;
    else
      cout << "seq bad " << endl;

    if (level::is_equal(morpho::hybrid_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4()),
			morpho::sure_geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4())))
      cout << "hybrid ok " << endl;
    else
      cout << "hybrid bad " << endl;

  }

   {
     cout << "MINIMA KILLER" << endl;

    for (unsigned int size=1; size < 20; ++size)
      if (level::is_equal(sure_minima_killer(lena, size, neighb_c4()),
			  fast_minima_killer(lena, size, neighb_c4()))
	  )
	cout << "minima killer " << size << " ok " << endl;
      else
	cout << "minima killer " << size << " bad " << endl;
  }

  {
    cout << "MAXIMA KILLER" << endl;

    for (unsigned int size=1; size < 20; ++size)
      if (level::is_equal(sure_maxima_killer(lena, size, neighb_c4()),
			  fast_maxima_killer(lena, size, neighb_c4()))
	  )
	cout << "maxima killer " << size << " ok " << endl;
      else
	cout << "maxima killer " << size << " bad " << endl;
  }


    misc::timer t;

    t.start();
    morpho::sure_geodesic_reconstruction_erosion(lena, lena_open, neighb_c4());
    cout << "rec ero sure " << t.stop() << endl;

    t.start();
    morpho::sequential_geodesic_reconstruction_erosion(lena, lena_open,
						       neighb_c4());
    cout << "rec ero sequential " << t.stop() << endl;

    t.start();
    morpho::hybrid_geodesic_reconstruction_erosion(lena, lena_open, neighb_c4());
    cout << "rec ero hybrid " << t.stop() << endl;

    t.start();
    morpho::sure_geodesic_reconstruction_dilation(lena_open, lena, neighb_c4());
    cout << "rec dil sure " << t.stop() << endl;

    t.start();
    morpho::sequential_geodesic_reconstruction_dilation(lena_open, lena,
							neighb_c4());
    cout << "rec dil sequential " << t.stop() << endl;

    t.start();
    morpho::hybrid_geodesic_reconstruction_dilation(lena_open, lena,
						    neighb_c4());
    cout << "rec dil hybrid " << t.stop() << endl;


    t.start();
    sure_minima_killer(lena, 5, neighb_c4());
    cout << "sure minima killer  5 " << t.stop() << endl;

    t.start();
    sure_minima_killer(lena, 60, neighb_c4());
    cout << "sure minima killer  30 " << t.stop() << endl;
    t.start();

    fast_minima_killer(lena, 5, neighb_c4());
    cout << "minima killer  5 " << t.stop() << endl;

    t.start();
    fast_minima_killer(lena, 60, neighb_c4());
    cout << "minima killer  30 " << t.stop() << endl;
}
