#include <mlc/array.hh>
#include <iostream>
using std::cout;
using std::endl;

#include "check.hh"

using namespace oln;
using namespace meta;

#define OK_OR_FAIL				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	fail = true;				\
      }

bool
check()
{
  bool fail = false;

  array1d< array1d_info< 3 >, int > foo = (ints_1d = 1, 2, 3, end);
  cout << endl << "Source:\n\n" << foo << endl;
  cout << "After a central symmetry:\n\n" << -foo << endl;

  if (1)
    OK_OR_FAIL;

  return fail;
}
