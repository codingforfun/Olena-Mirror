#! /bin/sh

# gen_scripts.sh

# Copyright (C) 2003  EPITA Research and Development Laboratory
#
# This file is part of the Olena Library.  This library is free
# software; you can redistribute it and/or modify it under the terms
# of the GNU General Public License version 2 as published by the
# Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
# As a special exception, you may use this file as part of a free
# software library without restriction.  Specifically, if other files
# instantiate templates or use macros or inline functions from this
# file, or you compile this file and link it with other files to
# produce an executable, this file does not by itself cause the
# resulting executable to be covered by the GNU General Public
# License.  This exception does not however invalidate any other
# reasons why the executable file might be covered by the GNU General
# Public License.


die() {
  echo "$@" 1>&2
  exit 1
}

[ -d "$1" -a -d "$2" -a -d "$3" ] || die "usage: $0 <includedir> <srcdir> <builddir>"
includedir=`cd "$1" && pwd`
srcdir=`cd "$2" && pwd`
destdir="$3"
defs="$srcdir/compilers.def"

[ -x "$srcdir"/configure ] || die "$srcdir/configure not available"
[ -r "$srcdir"/oln-config.shin ] || die "$srcdir/oln-config.shin not available"
[ -r "$srcdir"/compilers.def ] || die "$defs not available"

nconf=`wc -l < "$defs"`
n=1
while [ $n -le $nconf ]; do
  line=`sed -n -e "$n{p;q;}" <"$defs"`
  name=`echo "$line" | cut -d: -f1`
  compilers=`echo "$line" | cut -d: -f2`

  echo -n "Configuring for $name ... "
  supported=no
  for c in $compilers; do
    CXX="$c"; export CXX
    CXXFLAGS=""; export CXXFLAGS
    if "$srcdir/configure" --with-oln="$includedir" >"$destdir/configure.log-$c" 2>&1; then
       supported=$c
       mv config.log "$destdir/config.log-$c"
       break
    fi
    mv config.log "$destdir/config.log-$c" 
  done
  if [ "x$supported" = xno ]; then
     echo "unsupported"
  else
     echo "supported as $supported -> oln-config-$name.sh"
     mv oln-config.sh "$destdir/oln-config-$name.sh"
  fi
  n=`expr $n + 1`
done

rm -f config.status

