#include <mlc/array/all.hh>
#include <iostream>
using std::cout;
using std::endl;

#include "check.hh"

using namespace mlc;

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

  array2d< array2d_info< 3, 4, 2>, int > foo = (ints_2d =  1, 2, x(3), 4, lbrk,
						5, 6, 7, 8,
						9, 10, 11, 12, end);

  cout << endl << "Source:\n\n" << foo << endl;
  cout << "After a transposition:\n\n" << foo.transpose() << endl;
  cout << "After a central symmetry:\n\n" << -foo << endl;

  if (1)
    OK_OR_FAIL;

  return fail;
}
