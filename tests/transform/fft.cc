#include <oln/types/all.hh>
#include <oln/transform/fft.hh>

#include "check.hh"

using namespace oln;

int main()
{
  image2d<int_u8> im1(data("lena128.pgm"));
  
  fft<int_u8> fft1(im1);

  image2d<cplx<rect, dfloat> > im2 = fft1.transform();

  image2d<int_u8> out = fft1.transformed_image_clipped_magn(0.1);
  io::save(out, "lena_fft_clipped.pgm");

  image2d<int_u8> outlog = fft1.transformed_image_log_magn(1, 80);
  io::save(outlog, "lena_fft_log.pgm");
  
  for (int row = 40; row < im2.nrows() - 40; ++row)
    for (int col = 0; col < im2.ncols(); ++col)
      im2(row, col) = 0;

  for (int row = 0; row < im2.nrows(); ++row)
    for (int col = 40; col < im2.ncols() - 40; ++col)
      im2(row, col) = 0;
 
  im1 = fft1.transform_inv();

  io::save(im1, "lena_low_pass.pgm");

  fft1.transform();

  return 0; 
}
