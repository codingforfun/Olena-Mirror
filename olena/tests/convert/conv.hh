#include <ntg/color.hh>
#include <oln/convert/conversion.hh>
#include "check.hh"
#include "data.hh"
#include <iostream>

using namespace ntg;
using namespace oln;

template< class Input >
int
dist (Input l, Input r)
{
  return std::abs(l[0] - r[0]) +
	 std::abs(l[1] - r[1]) +
	 std::abs(l[2] - r[2]);
}
