#include "srcdir.hh"
#include "data.hh"
#include <unistd.h>

// Try OLN_IMG_AUX_DIR/file, then OLN_IMG_DIR/file, 
// else return OLN_IMG_AUX_DIR/file.

std::string
wdata(const std::string& file)
{
  std::string s = std::string(OLN_IMG_AUX_DIR) + '/' + file;
  if (access(s.c_str(), R_OK) != -1) 
    return s;

  std::string s2 = std::string(OLN_IMG_DIR) + '/' + file;
  if (access(s2.c_str(), R_OK) != -1)
    return s2;

  return s;
}

// Try OLN_IMG_AUX_DIR/file, else return OLN_IMG_DIR/file, 

std::string
rdata(const std::string& file)
{
  std::string s = std::string(OLN_IMG_AUX_DIR) + '/' + file;
  if (access(s.c_str(), R_OK) != -1) 
    return s;

  return std::string(OLN_IMG_DIR) + '/' + file;
}
