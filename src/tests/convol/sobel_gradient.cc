#include <oln/basics2d.hh>
#include <oln/convol/convolution.hh>
#include <oln/convert/basics.hh>

#include "check.hh"

#include <iostream>
using std::cout;
using std::endl;

using namespace oln;
using namespace meta;


bool check()
{
  image2d<int_u8> img = load(data("lena.pgm"));

  image2d<int_s<10> > res_img = convol::slow::convolve(img, (ints_2d =
							     -1, 0, 1, lbrk,
							     -2, 0, 2,
							     -1, 0, 1, end));

  save(apply(convert::stretch<int_u8>(), res_img), "lena-sobel.pgm");

  // FIXME: This tests always succeed.
  return false;
}
