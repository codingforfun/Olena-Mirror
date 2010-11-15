#! /bin/sh

# This `configure.gnu' script is invoked by the parent `configure'
# instead of `configure' in recursive configurations (see
# ``Configuring Other Packages in Subdirectories'' in Autoconf's
# manual).  This is convenient to pass extra options, like the path to
# Milena's source directory.
#
# This idea was taken and adapted from this message:
# http://lists.gnu.org/archive/html/autoconf/2002-12/msg00116.html

# Is option checking disabled?
no_option_checking_p=false
# Is `--disable-shared' (or `--enable-shared=no') not provided?
no_disable_shared_p=true

for i; do
  case "$i" in
    # First try to get SRCDIR from a possible `--srcdir' argument.
    --srcdir=*) srcdir=`echo "$i" | sed 's/^--srcdir=//'`;;
    --disable-option-checking) no_option_checking_p=true;;
    --disable-shared|--enable-shared=no) no_disable_shared_p=false;;
  esac
done

# Then try to guess it from this script's path.
test x"${srcdir+set}" != xset && srcdir=`dirname $0`

# Disabled option checking is a hint that this `configure.gnu' was
# called from a parent `configure'.  If so, and if no
# `--disable-shared' (or `--enable-shared=no') option was provided,
# pass our own `--enable-shared' option to the recursive call to the
# actual `configure'.
if $no_option_checking_p && $no_disable_shared_p; then
  exec "$srcdir/configure" ${1+"$@"} --enable-shared
else
  exec "$srcdir/configure" ${1+"$@"}
fi
