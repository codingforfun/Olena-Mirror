// EDOC == Erosion Dilation Opening Closing.
// Compare the morpho::fast:: and morpho:: versions.

#include <oln/basics2d.hh>
#include <ntg/all.hh>

#include <oln/morpho/erosion.hh>
#include <oln/morpho/dilation.hh>
#include <oln/morpho/opening.hh>
#include <oln/morpho/closing.hh>

#include <oln/level/compare.hh>

#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace ntg;

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

  image2d<int_u8> lena(rdata("lena128.pgm"));

  std::cout << "erosion ... " << std::flush;
  if (level::is_equal(morpho::erosion(lena, win_c4p()),
		      morpho::fast::erosion(lena, win_c4p())))
    OK_OR_FAIL;
    std::cout << "dilation ... " << std::flush;
  if (level::is_equal(morpho::dilation(lena, win_c4p()),
		      morpho::fast::dilation(lena, win_c4p())))
    OK_OR_FAIL;
  std::cout << "opening ... " << std::flush;
  if (level::is_equal(morpho::opening(lena, win_c4p()),
		      morpho::fast::opening(lena, win_c4p())))
    OK_OR_FAIL;
  std::cout << "closing ... " << std::flush;
  if (level::is_equal(morpho::closing(lena, win_c4p()),
		      morpho::fast::closing(lena, win_c4p())))
    OK_OR_FAIL;

  return fail;
}
