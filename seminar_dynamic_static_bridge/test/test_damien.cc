#include "my_lib/damien.hh"
#include "dyn.hh"
#include "config.hh"

namespace dyn
{
  ::dyn::fun down("mk_down");
}

int main()
{
  dyn::include_dir(SOURCE_DIR);
  dyn::include_dir("fixtures");
  dyn::include("my_lib/damien.hxx");
  dyn::misc_compile_line("/home/lrde/lrde-2006/thivol_d/lang/c++/seminar_dynamic_static_bridge/lib/libiberty-linux.a");
  cplus_demangle_set_style(gnu_v3_demangling);

//   down<char>               a('x');
//   down<int>                b(10);
//   std::string              bar("bar");
//   down<std::string>        c(bar); // c("bar"); // it's worst
//   down<down<std::string> > d(c);
//   down<int>                e(20);

//   var d2 = d;

//   std::cout << d2 << std::endl;


  dyn::fun	dyn_sqr("sqr");
  dyn::proc	dyn_print("print");

  dyn_print(dyn_sqr(10));


  std::cout << "exiting" << std::endl;
  return 0;
}
