#include <oln/basics2d.hh>
#include <oln/utils/stat.hh>
#include "check.hh"
#include "data.hh"
#include <iostream>

using namespace oln;
using namespace ntg;

template< class Input >
struct f_l2_vec3 : public std::binary_function<Input, Input, float_s>
{
  float_s operator()(Input l, Input r) const
  {
    float v0 = (float) l[0] - r[0];
    float v1 = (float) l[1] - r[1];
    float v2 = (float) l[2] - r[2];
    
    return sqrt(v0 * v0 + v1 * v1 + v2 * v2);
  }
};

template< class F, class Dist, class I1, class I2 >
F&
compare_images(F& functor, Dist distance,
	       const abstract::image<I1>& im1, const abstract::image<I2>& im2)
{
  precondition(im1.size() == im2.size());
  Iter(I1) p(im1);
  for_all(p)
    functor(distance(im1[p], im2[p]));
  return functor;
}
