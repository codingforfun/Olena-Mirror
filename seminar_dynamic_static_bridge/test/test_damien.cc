#include "../src/function_loader.hh"
#include "../src/mlc.hh"
#include "fixtures/my_lib/damien.hh"


int main()
{
  function_loader.include_dir("../src");
  function_loader.include_dir("fixtures");
  down<int> a(10);

  dyn::mk_data(a, "a");

  function_loader.call("my_lib/damien.hh", "foo", "a");
  return 0;
}
