#! /bin/sh

{
  find . -name \*~ \
     -o -name  \*\# \
     -o -name  .\#\* \
     -o -name \*.log \
     -o -name diffs.patch
  echo "olena/oln/config/pconf-inc.in"
  echo "olena/oln/config/pconf.inc"
  echo "olena/oln/config/pconf.hh"
} | xargs rm -f

find . -name autom4te.cache \
   -o -name .libs \
   -o -name .deps \
 | xargs rm -rf
