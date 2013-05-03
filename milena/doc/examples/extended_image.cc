#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/extended.hh>
#include <mln/fun/p2p/translation.hh>

int main()
{
  using namespace mln;

//! [Input initialization]
  /*
 (-3,-3)--------.
   |            |
   |(0,0)----.  |
   |  |      |---------> Effective domain
   |  .-----(3,3)
   |            |----> Image border (3 pixels, by default)
   .----------(6,6)
   */
  typedef mln::image2d<bool> I;
  I ima(4, 4);
//! [Input initialization]

//! [Shrink image domain]
  mln::box2d b_shrink = ima.domain().to_smaller(1);
  /*
 (-3,-3)--------.
   |            |
   |(1,1)----.  |
   |  |      |---------> Effective domain
   |  .-----(2,2)
   |            |----> Image border (4 pixels)
   .----------(6,6)
   */
  mln::extended<I> ima_shrink = mln::extended_to(ima, b_shrink);
//! [Shrink image domain]

//! [Extend image domain]
  mln::box2d b_extend = ima.domain().to_larger(1);
  /*
 (-3,-3)--------.
   |            |
   (-1,-1)---.  |
   |  |      |---------> Effective domain
   |  .-----(4,4)
   |            |----> Image border (2 pixels)
   .----------(6,6)
   */
  mln::extended<I> ima_extend = mln::extended_to(ima, b_extend);
//! [Extend image domain]

}
