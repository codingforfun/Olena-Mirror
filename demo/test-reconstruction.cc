/* FIXME: This is not really a demo.  */

#include "basics2d.hh"

#include "morpho/erosion.hh"
#include "morpho/dilation.hh"
#include "morpho/opening.hh"
#include "morpho/geodesic_dilation.hh"
#include "morpho/geodesic_erosion.hh"
#include "morpho/reconstruction.hh"

#include "level/extrema_killer.hh"

#include "utils/timer.hh"

using namespace oln;
using namespace oln::level;

int main()
{
  image2d<int_u8> lena = load("../img/lena128.pgm");
  image2d<int_u8> lena_open = morpho::opening(lena, win_c4p());

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
