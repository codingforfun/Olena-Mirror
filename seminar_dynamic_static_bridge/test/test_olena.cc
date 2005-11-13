#include "../proto-stl-style/all.hh"
#include "function_loader.hh"
#include "config.hh"

mlc_set_name_TC(image2d);

int main()
{
  dyn::include_dir(SOURCE_DIR);
  dyn::include_dir("../proto-stl-style");
  dyn::include("all.hh");


  dyn::proc dyn_iota("iota");
  dyn::proc dyn_println_2d("iota");
  dyn::ctor        mk_image2d_int("image2d<int>");


//   image2d<int> ima(3, 3);
  var ima = mk_image2d_int(3, 3);

//   println_2d(std::cout, "ima_before=", ima);
  dyn_iota(ima);
//   println_2d(std::cout, "ima-after=", ima);




}
