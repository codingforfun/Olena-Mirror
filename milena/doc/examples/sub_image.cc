#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/sub_image.hh>

// Example specific header.
#include <doc/tools/sample_utils.hh>

int main()
{
  using namespace mln;

  //! [Input image]
  bool vals[5][5] = { { 0, 0, 0, 0, 0 },
		      { 0, 1, 1, 1, 0 },
		      { 0, 1, 1, 1, 0 },
		      { 0, 1, 1, 1, 0 },
		      { 0, 0, 0, 0, 0 } };
  typedef image2d<bool> I;
  I ima = make::image(vals);
  //! [Input image]

  doc::pbmsave(ima, "sub_image");

  //! [Sub domain]
  // Sub domain considered for further processing.
  box2d sub_domain = make::box2d(1,1, 3,3);
  //! [Sub domain]

  //! [Operator pipe]
  // Create a sub image using '|' operator (short way).
  mln::sub_image<I,mln::box2d> sub = ima | sub_domain;
  //! [Operator pipe]

  //! [Sub_image constructor]
  // Create a sub image using sub_image constructor.
  mln::sub_image<I,mln::box2d> sub2 = sub_image<I,box2d>(ima, sub_domain);
  //! [Sub_image constructor]

  doc::pbmsave(sub, "sub_image");

  /* sub == sub2 == {
     { 2, 2, 2 },
     { 2, 2, 2 },
     { 2, 2, 2 },
   }
   */
}
