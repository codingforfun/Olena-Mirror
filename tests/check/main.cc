#include <iostream>
#include "verbose.hh"
#include "check.hh"

int
main (int argc, char *argv[])
{
  (void) argc;
  verbose_setup();
  srcdir_setup();
  std::cout << "=== Running " << argv[0] << std::endl;
  return check();
}
