#include <mln/core/image/image2d.hh>
#include <mln/data/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/literal/all.hh>
#include <mln/value/int_s8.hh>

//#include <cmath>
#include <string>

#include "scale-space.hh"
//#include "dog-space.hh"
//#include "keypoint.hh"
//#include "matrix.hh"

int main(void)
{
  typedef image2d<value::int_u8> I;
  typedef image2d<value::rgb8> C;

  // General parameters
  std::string source("images/keith.pbm");
  const unsigned blur_level = 5;
  const unsigned octave_level = 3;
  const bool black = false;

  ScaleSpace<I> *ss = new ScaleSpace<I>(octave_level, blur_level);
  //DoGSpace<C>* dog = new DogSpace();
  //std::vector<Keypoint>* keypoints = new std::vector<Keypoint>();

  I original;
  C extrema, improved;

  io::pgm::load(original, source.c_str());

  if (black)
  {
    initialize(extrema, original);
    initialize(improved, original);
  }
  else
  {
    extrema = data::convert(value::rgb8(), original);
    improved = data::convert(value::rgb8(), original);
  }

  // Localization
  ss->build(original);
  ss->save();
  //buildDifferenceOfGaussianSpace(scaleSpace, dogSpace);
  //buildExtrema(extrema, dogSpace, keypoints);
  //discardLowContrastKeypoints(dogSpace, keypoints, improved);

  // Processing
  // TODO

  // Save
  io::ppm::save(extrema, "output/extrema.ppm");
  io::ppm::save(improved, "output/extrema_improved.ppm");

  return 0;
}
