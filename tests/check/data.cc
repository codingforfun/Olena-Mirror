#include "srcdir.hh"
#include <unistd.h>


// Try ../data/file,
// else return $(srcdir)/../data/file.

std::string
data(const std::string file)
{
  std::string s = std::string("../data/") + file;
  if (access(s.c_str(), R_OK) != -1)
    return s;

  return srcdir + s;
}
