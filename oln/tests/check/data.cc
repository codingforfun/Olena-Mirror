#include "srcdir.hh"
#include <unistd.h>


// Try WDATADIR/file, then RDATADIR/file, 
// else return WDATADIR/file.

std::string
data(const std::string file)
{
  std::string s = std::string(WDATADIR) + '/' + file;
  if (access(s.c_str(), R_OK) != -1) 
    return s;

  std::string s2 = std::string(RDATADIR) + '/' + file;
  if (access(s2.c_str(), R_OK) != -1)
    return s2;

  return s;
}
