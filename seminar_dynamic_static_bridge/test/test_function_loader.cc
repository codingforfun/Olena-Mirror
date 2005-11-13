#include "function_loader.hh"
#include "config.hh"
#include "my_lib/lib.hh"

int main()
{
  dyn::include_dir(SOURCE_DIR);
  dyn::include_dir("fixtures");

  dyn::proc dfoo1("foo1"); // With the include

  dyn::include("my_lib/lib.hh"); // setup a default include

  dyn::fun  dfoo2("foo2"); // use the default includes
  dyn::fun  dfoo3("foo3");
  dyn::fun  dfoo4("foo4");
  dyn::fun  dsqr("my_lib::sqr");
  dyn::fun  dpower("my_lib::power");

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
