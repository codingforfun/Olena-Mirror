#! /bin/sh

{
  find . -name \*~ \
     -or -name  \*\# \
     -or -name  .\#\* \
     -or -name \*.log \
     -or -name diffs.patch
  echo "oln/oln/config/pconf-inc.in"
  echo "oln/oln/config/pconf.inc"
  echo "oln/oln/config/pconf.hh"
} | xargs rm -f

find . -name autom4te.cache \
   -or -name .libs \
   -or -name .deps \
 | xargs rm -rf


