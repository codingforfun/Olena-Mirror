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

  array3d< array3d_info< 2, 2, 3, 1>, int > foo = (ints_3d =  1, x(2), 3, lbrk,
						   4, 5, 6, pbrk,

						   7, 8, 9,
						   10, 11, 12, end);

  cout << endl << "Source:\n\n" << foo << endl;

  try {
    cout << "After a transposition:\n\n" << foo.transpose() << endl;
  }
  catch (not_implemented_yet) {
    cout << endl;
  }

  cout << "After a central symmetry:\n\n" << -foo << endl;

  if (1)
    OK_OR_FAIL;

  return fail;
}
