#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/transformed_image.hh>
#include <mln/fun/p2p/translation.hh>

int main()
{
  using namespace mln;

//! [Translate image domain]
  /*
    (0,0)----.
      |      |
      .-----(2,2)
   */
  typedef mln::image2d<bool> I;
  I ima(3, 3);

  // Translation delta-point of row+2 and col-1.
  mln::dpoint2d translate_dp(+2, -1);
  typedef mln::fun::p2p::translation_t<point2d> F;
  F translate_fun(translate_dp);

  /*
    (2,-1)---.
      |      |
      .-----(4,1)
   */
  mln::transformed_image<I,F>
    ext_ima = mln::transform_domain(ima, translate_fun);
//! [Translate image domain]
}
