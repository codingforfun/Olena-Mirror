#! /bin/sh

die() {
  echo "$@" 1>&2
  exit 1
}

{ [ -d "$1" ] && [ -d "$2" ] && [ -d "$3" ]; } || die "usage: $0 <includedir> <srcdir> <builddir>"
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

rm config.status

