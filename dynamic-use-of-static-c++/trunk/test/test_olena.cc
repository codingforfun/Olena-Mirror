#include "../proto-stl-style/all.hh"
#include "dyn.hh"
#include "config.hh"

int main()
{
  dyn::include_dir(DYNDIR);
  dyn::include_dir("fixtures");
  dyn::include_dir("../proto-stl-style");
  dyn::include("all.hh");
  dyn::include("oln_name.hh");

  dyn::proc dyn_iota("iota");
  dyn::proc dyn_println_2d("println_2d");
  dyn::ctor        mk_image2d_int("image2d<int>");


//   image2d<int> ima(3, 3);
  var ima = mk_image2d_int(3, 3);

//   image2d<int> im2(3, 3);
//   println_2d(std::cout, "ima_before=", ima);
  dyn_iota(ima);
//   println_2d(std::cout, "ima-after=", ima);




}
