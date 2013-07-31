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

  // General parameters
  std::string source("images/keith.pbm");
  const unsigned blur_level = 5;
  const unsigned octave_level = 4;

  ScaleSpace<I> *ss = new ScaleSpace<I>(octave_level, blur_level);
  DoGSpace<I, S>* dogs = new DoGSpace<I, S>(ss);
  std::list<Keypoint> keypoints;

  I original;
  C extrema, extrema_b,
    extrema2, extrema2_b;
    //improved, improved_b;

  io::pgm::load(original, source.c_str());

  initialize(extrema_b, original);
  initialize(extrema2_b, original);
  //initialize(improved_b, original);

  extrema = data::convert(value::rgb8(), original);
  extrema2 = data::convert(value::rgb8(), original);
  //improved = data::convert(value::rgb8(), original);

  // Localization
  ss->build(original);
  //ss->save();
  dogs->build();
  //dogs->save();
  dogs->findKeypoints(keypoints);

  writeKeypoints(keypoints, extrema, extrema_b);

  std::cout << "BEFORE: Low contrast discarding: " << keypoints.size() << std::endl;
  dogs->discardLowContrastKeypoints(keypoints);
  std::cout << "AFTER: Low contrast discarding: " << keypoints.size() << std::endl;

  std::cout << "BEFORE: Elimination of edge responses: " << keypoints.size() << std::endl;
  //dogs->eliminateEdgeResponses(keypoints);
  std::cout << "AFTER: Elimination of edge responses: " << keypoints.size() << std::endl;

  writeKeypoints(keypoints, extrema2, extrema2_b);

  //writeKeypoints(keypoints, extrema2, extrema2_b);

  // Save
  io::ppm::save(extrema, "output/extrema.ppm");
  io::ppm::save(extrema_b, "output/extrema_b.ppm");
  io::ppm::save(extrema2, "output/extrema2.ppm");
  io::ppm::save(extrema2_b, "output/extrema2_b.ppm");
  //io::ppm::save(improved, "output/extrema_improved.ppm");
  //io::ppm::save(improved_b, "output/extrema_b_improved.ppm");

  return 0;
}
