#include <oln/basics2d.hh>
#include <oln/utils/stat.hh>
#include "check.hh"
#include "data.hh"
#include <iostream>

using namespace oln;

template< class Input >
struct f_l2_vec3 : public std::binary_function<Input, Input, sfloat>
{
  sfloat operator()(Input l, Input r) const
  {
    float v0 = (float) l[0] - r[0];
    float v1 = (float) l[1] - r[1];
    float v2 = (float) l[2] - r[2];
    
    return sqrt(v0 * v0 + v1 * v1 + v2 * v2);
  }
};

template< class F, class Dist, class I1_, class I2_ >
F&
compare_images(F& functor, Dist distance,
	       const image<I1_>& _im1, const image<I2_>& _im2)
{
  Exact_cref(I1, im1);
  Exact_cref(I2, im2);
  precondition(im1.size() == im2.size());
  Iter(I1) p(im1);
  for_all(p)
    functor(distance(im1[p], im2[p]));
  return functor;
}
