#include <ntg/bin.hh>
#include <oln/basics2d.hh>
#include <oln/topo/tarjan/flat-zone.hh>

#include <iostream>

#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace ntg;
using namespace topo::tarjan;

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

  typedef image2d<bin> input_t;

  input_t src = load(rdata("object"));

  flat_zone<input_t> cc(src);

  if (cc.nlabels() == 79)
    OK_OR_FAIL;

  return fail;
}
