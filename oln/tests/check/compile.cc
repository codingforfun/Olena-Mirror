#include "compile.hh"

using namespace std;

string compile_cmd(const string& src,
		   const string& dst,
		   const string& extra_flags)
{
  return string(CXX) + ' '
    + CXXFLAGS + ' '
    + CPPFLAGS + ' '
    + OLN_EXTRA_CPPFLAGS + ' '
    + extra_flags + ' '
    + (dst.size() ? "-o " : " ") + dst + ' '
    + src;
}

const char* cxxflags_optimize()
{
  return CXXFLAGS_OPTIMIZE;
}

const char* cxxflags_strict()
{
  return CXXFLAGS_STRICT;
}

const char* cxxflags_strict_errors()
{
  return CXXFLAGS_STRICT_ERRORS;
}
