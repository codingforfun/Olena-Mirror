#include <ntg/int.hh>
#include <oln/basics2d.hh>
#include <oln/utils/histogram.hh>

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

  typedef image2d<int_u8> input;

  input src = load(rdata("lena256.pgm"));

  std::vector<point2d> v(src.nrows() * src.ncols());

  distrib_sort<input>(src, v);

  return fail;
}
