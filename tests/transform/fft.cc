#include <oln/types/all.hh>
#include <oln/transform/fft.hh>
#include <oln/level/compare.hh>

#include "check.hh"

#define OUTPUT_DIR  "output/"
#define OUTPUT_NAME "output"

#define OK_OR_FAIL				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	fail = true;				\
      }

using namespace oln;

bool
check()
{
#if HAVE_FFTW

  bool fail = false;

  image2d<int_u8> im1(data("lena.pgm"));

  oln::transform::fft<int_u8> fourier(im1);

  image2d<cplx<polar, dfloat> > im2 = fourier.transform();

  image2d<int_u8> im3 = fourier.transform_inv();

  io::save(im3, OUTPUT_DIR OUTPUT_NAME "_copy.pgm");

  std::cout << "Test: Image = F-1(F(Image)) ... " << std::flush;
  if (level::is_equal(im1, im3))
    OK_OR_FAIL;

  image2d<int_u8> out = fourier.transformed_image_clipped_magn(0.01);

  io::save(out, OUTPUT_DIR OUTPUT_NAME "_fft_clipped.pgm");

  image2d<int_u8> outlog = fourier.transformed_image_log_magn<int_u8>(1, 80);

  io::save(outlog, OUTPUT_DIR OUTPUT_NAME "_fft_log.pgm");
  
  for (int row = 40; row < im2.nrows() - 40; ++row)
    for (int col = 0; col < im2.ncols(); ++col)
      im2(row, col) = 0;

  for (int row = 0; row < im2.nrows(); ++row)
    for (int col = 40; col < im2.ncols() - 40; ++col)
      im2(row, col) = 0;
 
  im1 = fourier.transform_inv();

  io::save(im1, OUTPUT_DIR OUTPUT_NAME "_low_pass.pgm");

  fourier.transform();

  return fail;

#else

  std::cout << "You have not enabled fftw support." << std::endl;
  return true;

#endif

}
