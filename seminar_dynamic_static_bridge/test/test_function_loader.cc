#include "../src/function_loader.hh"
/*dyn_include("../src/mlc.hh")
dyn_include("fixtures/my_lib/lib.hh")
*/

#include "../src/mlc.hh"
#include "fixtures/my_lib/lib.hh"

int main()
{
  function_loader.include_dir("../src");
  function_loader.include_dir("fixtures");
  void (*foo1_)() = function_loader.load("fixtures/my_lib/lib.hh", mlc_name_of(foo1));
  foo1_();
  return 0;
}
