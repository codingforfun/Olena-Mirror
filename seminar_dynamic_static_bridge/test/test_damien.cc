#include "my_lib/damien.hh"
#include "dyn.hh"
#include "config.hh"

namespace dyn
{
  namespace down
  {
    ::dyn::fun mk("mk_down");
  }
}

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

  std::cout << mlc_name_of(f) << std::endl;

  var g = mk_down_int(44);
  // var h = mk_down(e);

  var j = dyn::down::mk(46);

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

    // j("print_noarg");

    // j->print_noarg();
  }

  std::cout << "exiting" << std::endl;
}
