#include "verbose.hh"
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>

void
verbose_setup ()
{
  // Be quiet if we are run from the Makefile and with VERBOSE unset.
  if (getenv("VERBOSE") == 0 && getenv("MAKEFLAGS") != 0)
    {
      // If you know a way to inhibit stdout which is less
      // ugly, fell free to improve this.
      int dn = open("/dev/null", O_WRONLY);
      dup2(dn, 1);
      close(dn);
    }
}
