#include "function_loader.hh"
#include "config.hh"
#include "my_lib/lib.hh"

int main()
{
  dyn::function_loader.include_dir(SOURCE_DIR);
  dyn::function_loader.include_dir("fixtures");

  dyn::proc dfoo1("my_lib/lib.hh", "foo1");
  dyn::fun dfoo2("my_lib/lib.hh", "foo2");
  dyn::proc dfoo3("my_lib/lib.hh", "foo3");
//   dyn::fun dfoo4("my_lib/lib.hh", "foo4");

  dfoo1();

  dfoo3(1, 2, 3);

  t<int, int> t12(1, 2);

  dfoo3(t12, t<int, int>(3, 4), t<int, int>(5, 6));

//  dfoo4(u<float>(4.4), t<t<char, char>, u<int> >(t<char, char>('a', 'b'), u<int>(42)));

  int* x1 = dfoo2();
//  var x2 = dfoo2.call_ret();
//  dyn::data ret;
//  ret.type_ = "int*";
//  dfoo2.call_ret(ret);

  return 0;
}
