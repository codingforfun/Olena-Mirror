#include "my_lib/damien.hh"
#include "function_loader.hh"
#include "config.hh"


int main()
{
  dyn::include_dir(SOURCE_DIR);
  dyn::include_dir("fixtures");
  dyn::include("my_lib/damien.hh");

  down<char>               a('x');
  down<int>                b(10);
  down<std::string>        c("bar");
  down<down<std::string> > d(c);
  down<int>                e(20);

  dyn::proc        dyn_foo("foo");
  dyn::method_proc dyn_print_noarg("print_noarg");
  dyn::ctor        mk_down_char("down<char>");
  dyn::ctor        mk_down_int("down<int>");

  var f = mk_down_char('x');
  var g = mk_down_int(42);
  // var h = mk_down(e);

  for ( int i = 0; i < 5; ++i )
  {
    std::cout << "*** Turn " << i << " ***" << std::endl;
    dyn_print_noarg(a);

    dyn_foo(a);

    dyn_foo(b);
    dyn_foo(c);
    dyn_foo(d);
    dyn_foo(e);
    dyn_foo(42);

    dyn_print_noarg(f);

    dyn_foo(g);

    // h->foo();
  }

  std::cout << "exiting" << std::endl;
}
