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

  down<char>               a('x');
  down<int>                b(10);
  std::string              bar("bar"); 
  down<std::string>        c(bar); // c("bar"); // it's worst
  down<down<std::string> > d(c);
  down<int>                e(20);

  var d2 = d;

  std::cout << d2 << std::endl;

  dyn::proc        dyn_foo("foo");
  dyn::method_proc dyn_print_noarg("print_noarg");
  dyn::proc        dyn_change("change");
  dyn::ctor        mk_down_char("down<char>");
  dyn::ctor        mk_down_int("down<int>");
  dyn::method_fun  dyn_get_i("get_i");
  dyn::method_fun  dyn_get_t("get_t");
  dyn::method_fun  dyn_clone("clone");
  dyn::fun         dyn_deref("deref");

  var f = mk_down_char('x');

  std::cout << mlc_name_of(f) << std::endl;

  var g = mk_down_int(44);
  // var h = mk_down(e);

  var j = dyn::down(46);

  // std::cout is not printable
  // if you try to do this it will compile
  // but fail on assertion
  // you can use is_printable before draw it.
  var not_printable = std::cout;
  if (not_printable.is_printable())
    std::cout << not_printable << std::endl;

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

    dyn_change(c);

    var x1 = dyn_get_i(b);
    std::cout << "dyn_get_i(b) => " << x1 << std::endl;
    var x2 = dyn_deref(dyn_get_t(d));
    std::cout << "dyn_get_t(d) => " << x2 << std::endl;

    var x3 = dyn_deref(dyn_clone(a));
    std::cout << "*clone(a) => " << x3 << std::endl;

    // j("print_noarg");

    // j->print_noarg();
  }

  std::cout << "exiting" << std::endl;
  return 0;
}
