#include "verbose.hh"
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

void
verbose_setup ()
{
  // Be quiet if we are run from the Makefile and with VERBOSE unset.
  if (getenv("VERBOSE") == 0 && getenv("MAKEFLAGS") != 0)
      close(STDOUT_FILENO);
}
