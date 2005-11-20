#include "dyn.hh"
#include "my_lib/lib.hh"
#include <algorithm>

int main()
{
  dyn::include_dir(DYNDIR);
  dyn::include_dir("fixtures");
  dyn::include("my_lib/lib.hh");

  echo("Test containers");
  std::vector<int> v(4);
  std::generate(v.begin(), v.end(), rand);
  std::vector<int>::iterator it;
  stl_each(v, it)
    echo(*it);
  it = v.begin();

  dyn::fun begin("begin", "method");
  dyn::fun end("end", "method");
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
  var my_begin = *begin(dv);
  int my_begin_i = *begin(dv);
  int my_begin_i2 = my_begin;
  int ref_begin_i = *v.begin();
  int ref_begin_i2 = v[0];
  echo("my_begin: " << my_begin);
  echo("my_begin_i: " << my_begin_i);
  echo("my_begin_i2: " << my_begin_i2);
  echo("ref_begin_i: " << ref_begin_i);
  echo("ref_begin_i2: " << ref_begin_i2);
  assert(my_begin_i == ref_begin_i);
  const std::vector<int>::iterator& tmp1b(begin(dv));
  const std::vector<int>::iterator& tmp2b(v.begin());
  const std::vector<int>::iterator& tmp3b(v.begin());
  var tmp4b = v.begin();
  assert((bool)(tmp3b == tmp2b));
  echo("a: " << dv[4]);
  echo("b: " << *begin(dv));
  echo("c: " << *begin(v));
  echo("e: " << *tmp1b);
  echo("f: " << *tmp2b);
  assert((bool)(*tmp1b == *tmp2b));
  assert((bool)(tmp1b == tmp2b));
  assert((bool)(begin(dv) == v.begin()));
  assert((bool)(ref_begin_i2 == my_begin_i2));

  // end
  int my_end_i = *end(dv);
  int ref_end_i = *v.end();
  echo(my_end_i);
  echo(ref_end_i);
  assert(my_end_i == ref_end_i);
  const std::vector<int>::iterator& tmp1(end(dv));
  const std::vector<int>::iterator& tmp2(v.end());
  const std::vector<int>::iterator& tmp3(v.end());
  var tmp4 = v.end();
  assert((bool)(tmp3 == tmp2));

  echo("a: " << dv[4]);
  echo("b: " << *end(dv));
  echo("c: " << *end(v));
  echo("e: " << *tmp1);
  echo("f: " << *tmp2);
  assert((bool)(*tmp1 == *tmp2));
  assert((bool)(tmp1 == tmp2));
  assert((bool)(end(dv) == v.end()));
  // int i = 0;
  // stl_each(v, i)
  // for (dit = begin(dv); diff(dit, end(dv)) && i < 42; incr(dit), ++i)
  // std::vector<int>::iterator tmp;
  // for (dit = begin(dv); v.end() != dit; ++dit, ++i)
  // {
    // std::cout << i << ": " << *dit << std::endl;
  // }

  echo("exiting");
  return 0;
}
