#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/level/compare.hh>
#if HAVE_FFTW
# include <oln/transforms/fft.hh>
#endif
#include <cstdlib>

#include "check.hh"

#define CHECK(Condition)			\
    if (Condition)				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	status = 1;				\
      }

using namespace oln;
#if HAVE_FFTW
using namespace transforms;
#endif

bool
check()
{
  int status = 0;

#if HAVE_FFTW

  image2d<int_u8> im1(rdata("lena.pgm"));

  fft<int_u8> fourier(im1);

  image2d<cplx<polar, dfloat> > im2 = fourier.transform();

  image2d<int_u8> im3 = fourier.transform_inv();

  io::save(im3, "fft_copy.pgm");

  std::cout << "Test: Image == F-1(F(Image)) ... " << std::flush;
  CHECK (level::is_equal(im1, im3));

  image2d<int_u8> out = fourier.transformed_image_clipped_magn(0.01);

  io::save(out, "fft_trans_clipped.pgm");

  out = fourier.transformed_image_log_magn<int_u8>(1, 100);

  io::save(out, "fft_trans_log.pgm");

  for (int row = 40; row < im2.nrows() - 40; ++row)
    for (int col = 0; col < im2.ncols(); ++col)
      im2(row, col) = 0;

  for (int row = 0; row < im2.nrows(); ++row)
    for (int col = 40; col < im2.ncols() - 40; ++col)
      im2(row, col) = 0;

  out = fourier.transform_inv();

  io::save(out, "fft_low_pass.pgm");

#else

  std::cout << "You have not enabled fftw support." << std::endl;

#endif

  return status;
}
