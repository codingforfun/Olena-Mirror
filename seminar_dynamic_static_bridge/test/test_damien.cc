#include "my_lib/damien.hh"
#include "function_loader.hh"
#include "config.hh"


int main()
{
  dyn::include_dir(SOURCE_DIR);
  dyn::include_dir("fixtures");
  dyn::include("my_lib/damien.hh");

  down<char>               a('x');
//   down<int>                b(10);
//   down<std::string>        c("bar");
//   down<down<std::string> > d(c);
//   down<int>                e(20);

  dyn::method_proc method_foo("print_noarg");

  method_foo(a);
//   for ( int i = 0; i < 5; ++i )
//   {
//     std::cout << "*** Turn " << i << " ***" << std::endl;
//     dyn_foo(a);

//     dyn_foo(b);
//     dyn_foo(c);
//     dyn_foo(d);
//     dyn_foo(e);
//     dyn_foo(42);
//   }

  std::cout << "exiting" << std::endl;
}
