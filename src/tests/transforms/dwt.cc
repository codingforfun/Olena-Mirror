#include <oln/types/all.hh>
#include <oln/basics1d.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/level/compare.hh>
#include <oln/transforms/dwt.hh>
#include <oln/transforms/wavelet_coeffs.hh>

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
using namespace transforms;

Wavelet_coeffs_definition(daub4_test, dfloat, 4)
{
  const dfloat sqrt_3 = sqrt(3);
  const dfloat denom = 4 * sqrt(2);
  
  Wavelet_coeffs_begin
    (1 + sqrt_3)/denom,
    (3 + sqrt_3)/denom,
    (3 - sqrt_3)/denom,
    (1 - sqrt_3)/denom
  Wavelet_coeffs_end
};

bool
check()
{
  bool fail = false;

  // 1D tests

  image1d<int_u16> vec1(4096);
  image1d<int_u16>::iter it1(vec1);
  for_all(it1)
    vec1[it1] = std::rand() / 10000000;

  dwt<image1d<int_u16>, daub4_test> wave1d(vec1);

  image1d<dfloat> vec2 = wave1d.transform(dwt_std, true);

  image1d<int_u16> vec3 = wave1d.transform_inv<int_u16>();

  std::cout << "Test: Image1D == F-1(F(Image1D)) ... " << std::flush;
  if (level::is_equal(vec1, vec3))
    OK_OR_FAIL;

  // 2D tests

  image2d<int_u8> im1(data("lena.pgm"));

  dwt<image2d<int_u8>, coiflet6> wave2d(im1);

  image2d<dfloat> im2 = wave2d.transform(dwt_std, true, 4);

  image2d<int_u8> im3 =
    wave2d.transform_inv<int_u8>();

  io::save(im3, OUTPUT_DIR OUTPUT_NAME "_dwt_copy.pgm");

  std::cout << "Test: Image2D == F-1(F(Image2D)) ... " << std::flush;
  if (level::is_equal(im1, im3))
    OK_OR_FAIL;

  image2d<int_u<8, saturate> > out(im2.size());
  image2d<dfloat>::iter it2(im2);
  for_all(it2)
    out[it2] = im2[it2];
  
  io::save(out, OUTPUT_DIR OUTPUT_NAME "_dwt_trans_std.pgm");

  im2 = wave2d.transform(dwt_non_std, true, 4);

  for_all(it2)
    out[it2] = im2[it2];
  
  io::save(out, OUTPUT_DIR OUTPUT_NAME "_dwt_trans_non_std.pgm");

  im2 = wave2d.transform(dwt_non_std, false, 3);
  for_all(it2)
    if (fabs(im2[it2]) < 42)
      if (im2[it2] != 0)
	im2[it2] = 0;

  out = wave2d.transform_inv<int_u<8, saturate> >();
    
  io::save(out, OUTPUT_DIR OUTPUT_NAME "_dwt_compressed.pgm");

  // 3D tests

  image3d<int_u32> cube1(64, 64, 64);
  image3d<int_u32>::iter it3(cube1);
  for_all(it3)
    cube1[it3] = std::rand();

  dwt<image3d<int_u32>, haar> wave3d(cube1);

  image3d<dfloat> cube2 = wave3d.transform(dwt_non_std, true, 1);
  image3d<int_u32> cube3 = wave3d.transform_inv<int_u32>();

  std::cout << "Test: Image3D == F-1(F(Image3D)) ... " << std::flush;
  if (level::is_equal(cube1, cube3))
    OK_OR_FAIL;
 
  return fail;
}
