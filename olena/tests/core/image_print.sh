#!/bin/sh

if test "x$VERBOSE" != x; then
  verb='echo'
  quiet=''
else
  verb=':'
  quiet='-q'
fi

exec 6>&2
. ../check/defs || exit 1
exec 2>&6

failures=0

rundir=runs
test -d $rundir || mkdir $rundir

input=type_list

if test -f $input; then 
  rm $input; 
fi

# If no tests were given on the command line, create the list
if test $# = 0; then
  cat << EOF > $input
float_s
float_d
unsigned char
unsigned int
unsigned long
int_u8
char
signed char
int
long
int_s16
bin
bool
range<int_u8, bounded_u<0, 210> >
range<long, bounded_s<-5200, 2560> >
rgb_8
yiq_8
vec<5, int_u8>
vec<5, float>
cplx<rect, int_s8
cplx<polar, int_u32>
cycle<float, bounded_s<-50, 50> >
EOF
else
  for test; do
    echo "$test" >> $input
  done
fi

while read type; do
  fail=0
  file=`echo "$type" | perl -pe 's/ //g; s/</_/g; s/>/_/g; s/,/_/g'`
  dir=$rundir/"$file"
  rm -rf "$dir"
  mkdir "$dir"
  # Build the source code to compile.
  src="$dir/$file.cc"
  cat >$src <<EOF
#include <oln/basics1d.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <ntg/all.hh>
#include <iostream>

using namespace oln;
using namespace ntg;

int main()
{
  image1d< $type > ima1d (5);
  std::cout << ima1d << std::endl;

  image2d< $type > ima2d (5, 10);
  std::cout << ima2d << std::endl;

  image3d< $type > ima3d (5, 10, 15);
  std::cout << ima3d << std::endl;
}
EOF

  # Build the compile script (using such a script make it easier
  # to debug failures);
  cat >"$dir/try" <<EOF
dir="\$2"
exec 6>&2
if test "x\$1" = "x-q"; then
  exec 2>"$dir"/cxx.errors
else
  echo $CXX $CPPFLAGS $CXXFLAGS $LDFLAGS "$dir/$file.cc"
fi
$CXX $CPPFLAGS $CXXFLAGS $LDFLAGS $dir/$file.cc -o $dir/test$EXEEXT || exit 50

if test "x\$1" = "x-q"; then
    exec 2>"$dir/run.errors"
else
    echo "$dir/test$EXEEXT"
fi
$dir/test$EXEEXT
estatus=\$?

exec 2>&6
echo \$estatus > "$dir/exit.status"
exit \$estatus
EOF
  chmod +x $dir/try

  xfail=no

  # Run the test.
  $verb ========== Running $test... >&2
  ($SHELL $dir/try "$quiet" "$dir")
  estatus=$?

  msg="";
  case $estatus in
    50) msg="(doesn't compile)" ;;
    134) msg="(aborts at run-time)" ;;
    *) rm -rf "$dir" ;;
  esac
  if test -d $dir; then
    if test "$xfail" = "no"; then
      echo "FAIL: $type $msg" >&2
      failures=`expr $failures + 1`
    else
      echo "XFAIL: $type $msg" >&2
    fi
  else
    echo "PASS: $type" >&2
  fi
done < $input

rm $input

echo ========================================>&2
if test $failures = 0; then
  echo No failures. >&2
  exit 0
else
  echo $failures failures. >&2
  exit 1
fi
