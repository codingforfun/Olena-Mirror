#include "srcdir.hh"
#include "data.hh"

#include <unistd.h>

#include <iostream>

std::string
rdata(const std::string& file)
{
  return OLN_IMG_DIR "/" + file; // FIXME
  // this is not satisfactory, since in some circumstances we would like
  // images specific to a particular test not appear in OLN_IMG_DIR, while
  // still using the rdata() function.
}
