#include <oln/basics2d.hh>
#include <oln/convol/convolution.hh>

#include <oln/level/compare.hh>

#include <iostream>
using std::cout;
using std::endl;

#include "check.hh"

using namespace oln;
using namespace meta;

#define OK_OR_FAIL				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	fail = true;				\
      }


bool
check(void)
{
  bool fail = false;

  image2d<int_u8> img = load(data("random.pgm"));
  cout << "=== Image ===" << endl << endl << img << endl;

  //
  // First way to do it, using w_windows.
  //

  w_window2d<int> w_win = (ints_2d =
			   1, 1, 1, lbrk,
			   1, 1, 1,
			   1, 1, 1, end);
  cout << "=== Convolution kernel ===" << endl << endl
       << w_win << endl << endl;

  image2d< int_s< 10 > > ret;
  ret = convol::slow::convolve(img, w_win);
  cout << "=== Result image (by convolve(w_window2d)) ===" << endl << endl
       << ret << endl;
  image2d< int_u8 > res_img = load(srcdir + "sum_on_random.pgm");
  if (level::is_equal(ret, res_img))
    OK_OR_FAIL;

  return fail;
}
