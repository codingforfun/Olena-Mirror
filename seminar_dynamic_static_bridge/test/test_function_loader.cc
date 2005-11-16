#include "dyn.hh"
#include "config.hh"
#include "my_lib/lib.hh"
#include <sstream>

int main()
{
  dyn::include_dir(SOURCE_DIR);
  dyn::include_dir("fixtures");

  dyn::include("my_lib/lib.hh"); // setup a default include

  // dyn::proc dfoo1("foo1", "my_lib/lib.hh"); // With the include
  dyn::proc dfoo1("foo1"); // , "my_lib/lib.hh"); // With the include

  dyn::fun  dfoo2("foo2"); // use the default includes
  dyn::fun  dfoo2b("foo2b");
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

  var x4c = dfoo2b();
  std::cout << x4c << std::endl;
  int x4b = 2;
  x4b = dfoo2b();
  std::cout << x4b << std::endl;
  int* x4 = dfoo2();
  std::cout << *x4 << std::endl;
  var x4d = dfoo2();
  int* x4e = x4d;
  std::cout << x4d << std::endl;
  std::cout << *x4e << std::endl;

  float f2a = dsqr(3.4);
  std::cout << f2a << std::endl;
  double f2b = dsqr(3.4);
  std::cout << f2b << std::endl;
  var f2c = dsqr(3.4);
  std::cout << f2c << std::endl;
  int i1 = dsqr(3);
  std::cout << i1 << std::endl;

  // Test istreams
  std::istringstream istr("32");
  // std::vector<int> tab(10);
  // tab[0] = 4;
  // u<int> ui(tab[0]);
  int uii = 4;
  u<int> ui(uii);
  dyn::fun reader("operator>>");
  std::cout << ui << std::endl;
  // istr >> ui;
  reader(istr, ui);
  std::cout << ui << std::endl;

  return 0;
}
