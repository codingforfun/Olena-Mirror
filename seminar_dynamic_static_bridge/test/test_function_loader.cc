#include "function_loader.hh"
#include "config.hh"
#include "my_lib/lib.hh"

int main()
{
  dyn::function_loader.include_dir(SOURCE_DIR);
  dyn::function_loader.include_dir("fixtures");

  dyn::proc dfoo1("foo1", "my_lib/lib.hh");
  dyn::fun  dfoo2("foo2", "my_lib/lib.hh");
  dyn::fun  dfoo3("foo3", "my_lib/lib.hh");
  dyn::fun  dfoo4("foo4", "my_lib/lib.hh");
  dyn::fun  dsqr("my_lib::sqr", "my_lib/lib.hh");
  dyn::fun  dpower("my_lib::power", "my_lib/lib.hh");

  dfoo1();

  int* x1 = dfoo3(1, 2, 3);
  std::cout << x1 << std::endl;

  t<int, int> t12(1, 2);

  t<int, int>* x2 = dfoo3(t12, t<int, int>(3, 4), t<int, int>(5, 6));
  std::cout << x2 << std::endl;
  // var x2b = dfoo3(t12, t<int, int>(3, 4), t<int, int>(5, 6));

  int* x4 = dfoo2();
  std::cout << *x4 << std::endl;
  var x4b = dfoo2();
  std::cout << x4b << std::endl;

  float f2a = dsqr(3.4);
  std::cout << f2a << std::endl;
  double f2b = dsqr(3.4);
  std::cout << f2b << std::endl;
  var f2c = dsqr(3.4);
  std::cout << f2c << std::endl;
  int i1 = dsqr(3);
  std::cout << i1 << std::endl;

  return 0;
}
