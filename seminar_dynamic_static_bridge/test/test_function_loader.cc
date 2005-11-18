#include "dyn.hh"
#include "config.hh"
#include "my_lib/lib.hh"
#include <sstream>
#include <algorithm>

int main()
{
  dyn::include_dir(SOURCE_DIR);
  dyn::include_dir("fixtures");

  dyn::include("my_lib/lib.hh"); // setup a default include
  dyn::include("vector");

  dyn::proc dfoo1("foo1", "my_lib/lib.hh"); // With the include

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
  std::cout << "f2a: " << f2a << std::endl;
  double f2b = dsqr(3.4);
  std::cout << "f2b: " << f2b << std::endl;
  assert(fabs(f2a - f2b) < 0.00001);
  var f2c = dsqr(3.4);
  std::cout << f2c << std::endl;
  int i1 = dsqr(3);
  std::cout << i1 << std::endl;

  echo("Test istreams");
  std::istringstream istr("32");
  int uii = 4;
  u<int> ui(uii);
  var dui = ui;
  std::cout << dui << std::endl;
  istr >> dui;
  std::cout << dui << std::endl;

  echo("Test affectations");
  var count = 0;
  ++count;
  echo(count);
  int counti = count;
  assert(counti == 1);

  echo("Test containers");
  std::vector<int> v(4);
  std::generate(v.begin(), v.end(), rand);
  std::vector<int>::iterator it;
  stl_each(v, it)
    echo(*it);
  it = v.begin();

  dyn::method_fun begin("begin");
  dyn::method_fun end("end");
  var dv = v;
  var dit = it;
  echo(dv[2]);
  v[2] = 51;
  echo(v[2]);
  echo(dv[2]);
  dv[2] = 42;
  int dv_2 = dv[2];
  echo(dv_2);
  assert(dv_2 == 42);
  std::cout << "v[0] => " << *dit << std::endl;
  int my_begin_i = *begin(dv);
  int ref_begin_i = dv[0];
  echo(my_begin_i);
  echo(ref_begin_i);
  assert(my_begin_i == ref_begin_i);
  int my_end_i = *end(dv);
  int ref_end_i = *v.end();
  echo(my_end_i);
  echo(ref_end_i);
  assert(my_end_i == ref_end_i);
  bool r;
  assert(! (r = (my_end_i != ref_end_i)));
  assert(! (r = (end(dv) != v.end())));
  // int i = 0;
  // stl_each(v, i)
  // for (dit = begin(dv); diff(dit, end(dv)) && i < 42; incr(dit), ++i)
  // std::vector<int>::iterator tmp;
  // for (dit = begin(dv); tmp = dit, v.end() != tmp && i < 42; incr(dit), ++i)
  // {
    // std::cout << i << ": " << *dit << std::endl;
  // }

  echo("exiting");
  return 0;
}
