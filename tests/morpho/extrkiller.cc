#include <oln/basics2d.hh>
#include <oln/level/extrema_killer.hh>

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

  {
    for (unsigned int size=1; size < 20; ++size) {
      std::cout << "minima killer (size " << size << ") ... " << std::flush;
      if (level::is_equal(sure_minima_killer(lena, size, neighb_c4()),
			  fast_minima_killer(lena, size, neighb_c4())))
	OK_OR_FAIL;
    }
  }
  {
    for (unsigned int size=1; size < 20; ++size) {
      std::cout << "maxima killer (size " << size << ") ... " << std::flush;
      if (level::is_equal(sure_maxima_killer(lena, size, neighb_c4()),
			  fast_maxima_killer(lena, size, neighb_c4())))
	OK_OR_FAIL;
    }
  }

  return fail;
}
