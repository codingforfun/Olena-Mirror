#include <mln/core/image/image2d.hh>
#include <mln/data/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/literal/all.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_s16.hh>

//#include <cmath>
#include <string>
#include <list>

#include "scale-space.hh"
#include "dog-space.hh"
#include "keypoint.hh"
//#include "matrix.hh"

int main(void)
{
  typedef image2d<value::int_u8> I;
  typedef image2d<value::int_s16> S;
  typedef image2d<value::rgb8> C;

  std::string source("images/tshirt.pgm");
  const unsigned blur_level = 3;
  const unsigned octave_level = 6;

  ScaleSpace<I> *ss = new ScaleSpace<I>(octave_level, blur_level);
  DoGSpace<I, S>* dogs = new DoGSpace<I, S>(ss);
  std::list<Keypoint> keypoints;

  I original;
  C extrema1, extrema1_b, extrema2, extrema2_b, extrema3, extrema3_b;

  io::pgm::load(original, source.c_str());

  initialize(extrema1_b, original);
  initialize(extrema2_b, original);
  initialize(extrema3_b, original);

  extrema1 = data::convert(value::rgb8(), original);
  extrema2 = data::convert(value::rgb8(), original);
  extrema3 = data::convert(value::rgb8(), original);

  // Localization
  ss->build(original);
  ss->save();
  dogs->build();
  dogs->save();

  dogs->findKeypoints(keypoints);
  writeKeypoints(keypoints, extrema1, extrema1_b);
  std::cout << "Keypoints.size " << keypoints.size() << std::endl;

  dogs->discardLowContrastKeypoints(keypoints);
  writeKeypoints(keypoints, extrema2, extrema2_b);
  std::cout << "Keypoints.size " << keypoints.size() << std::endl;

  dogs->eliminateEdgeResponses(keypoints);
  writeKeypoints(keypoints, extrema3, extrema3_b);
  std::cout << "Keypoints.size " << keypoints.size() << std::endl;

  // Save
  io::ppm::save(extrema1, "output/extrema1.ppm");
  io::ppm::save(extrema1_b, "output/extrema1_b.ppm");
  io::ppm::save(extrema2, "output/extrema2.ppm");
  io::ppm::save(extrema2_b, "output/extrema2_b.ppm");
  io::ppm::save(extrema3, "output/extrema3.ppm");
  io::ppm::save(extrema3_b, "output/extrema3_b.ppm");

  return 0;
}
