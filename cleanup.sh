#! /bin/sh

{
  find . -name Makefile.in
  find . -name \*~
  find . -name \*\#
  find . -name .\#\*
  find . -name configure
  find . -name aclocal.m4 
  find . -name mkinstalldirs
  find . -name depcomp
  find . -name install-sh
  find . -name missing
  find . -name diffs.patch
  echo "oln/oln/config/pconf-inc.in"
  echo "oln/oln-version"
  echo "doc/oln-version"
  echo "tools/oln-version"
  find . -name \*.log
} | xargs rm -rf

{
  find . -name autom4te.cache

} | xargs rm -rf