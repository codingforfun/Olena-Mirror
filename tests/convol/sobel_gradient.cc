#include <oln/basics2d.hh>
#include <oln/convol/convolution.hh>

#include <iostream>

using std::cout;
using std::endl;

using namespace oln;
using namespace meta;


int	main(void)
{
  image2d<int_u8> img = load("../../img/lena.pgm");

  cout 
    << convol::slow::convolve(img, (ints_2d =
  				    -1, 0, 1, lbrk,
  				    -2, 0, 2,
  				    -1, 0, 1, end)) << endl;

  // save(img, "lena-sobel.pgm"); // FIXME !! 
}


// As for now, please do it yourself as described below :
//
// 1. For zsh users :
// while read i; do echo $((($i+1024)/8)); done < =(./sobel_gradient | tr -s ' ' | tr ' ' ' 
// ') > lena-sobel.pgm
// (Be careful about the linebreak (^v ^j) !)
// 
// 2. Add the following header in the file lena-sobel.pgm :
// P2
// 514 510
// 255
//
// Why 514 510 instead of 512 512 ? -> because of the border.
//
// Last, but not least, you can admire the result.
//
