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
  
  image2d<int_u8> img = load("../data/random.pgm");
  cout << "=== Image ===" << endl << endl << img << endl;

  cout << "=== Convolution kernel ===" << endl << endl 
       << "1 1 1" << endl
       << "1 1 1" << endl
       << "1 1 1" << endl
       << endl;
  
  image2d< int_s< 10 > > ret;
   
  //
  // Second way to do it, using meta-arrays.
  //
  
  ret = convol::slow::convolve(img, (ints_2d =
				     1, 1, 1, lbrk,
				     1, x(1), 1,
				     1, 1, 1, end));

  cout << "=== Result image (by convolve(array2d)) ===" << endl << endl
       << ret << endl;

  image2d< int_u8 > res_img = load("./sum_on_random.pgm");
  if (level::is_equal(ret, res_img))
    OK_OR_FAIL;
  
   return fail;
}
