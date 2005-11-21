#include "my_lib/lib.hh"

#include "dyn.hh"
using namespace dyn::language;

int main()
{
  dyn::include_dir(DYNDIR);
  dyn::include_dir("fixtures");
  dyn::include("my_lib/lib.hh");

  echo("Test vars and vals");
  val i = 4;
  val j = 3 + i;
  var k(i);
  // var k = i;
  // var l = "foo";
  // var l("foo");
  echo("i: " << i);
  echo("j: " << j);
  echo("k: " << k);

  echo("exiting");
  return 0;
}
