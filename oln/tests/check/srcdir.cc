#include <string>
#include <cstdlib>

std::string srcdir;

void
srcdir_setup ()
{
  const char *s = getenv("srcdir");

  if (s == 0)
    // Assume we are run from the current directory.
    srcdir = ".";
  else
    srcdir = std::string(s);
}


// Returns `-I${srcdir}/ARG'.
std::string Isrcdir (const std::string& arg)
{
  return std::string("-I") + srcdir + '/' + arg;
}
