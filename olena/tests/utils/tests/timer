#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/utils/timer.hh>

#include <iostream>

#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace ntg;
using namespace utils;

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

  timer t;

  t.start();

  t.stop();

  float b = t.value();

  t.resume();

  for (int i = 0; i < 1000000000; )
    ++i;

  b = t.value() - b;

  if (b >= 0)
    OK_OR_FAIL;

  return fail;
}
