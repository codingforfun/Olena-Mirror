# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.52])
AC_INIT([olena], [0.4.1], [olena-bugs@lrde.epita.fr])
AC_CONFIG_SRCDIR([ChangeLog])
AC_CONFIG_AUX_DIR([tools])
AM_INIT_AUTOMAKE([no-define 1.6.2])

AM_CONFIG_HEADER(oln/config/config.h:oln/config/config.hin)

AC_PROG_CXX
AC_PROG_RANLIB

AC_LANG(C++)
AC_CHECK_HEADERS([stl_config.h])

# Check for zlib.
AC_ARG_WITH(zlib,
  [AC_HELP_STRING([--with-zlib=DIR], [prefix for zlib installation])])
ZLIB_CFLAGS=''
ZLIB_LDFLAGS=''
if test "x$with_zlib" != xno; then
  if test -n "$with_zlib"; then
    ZLIB_CFLAGS="-I${with_zlib}/include"
    ZLIB_LDFLAGS="-L${with_zlib}/lib"
  fi
  oln_save_CFLAGS=$CFLAGS
  oln_save_LDFLAGS=$LDFLAGS
  CFLAGS="$CFLAGS $ZLIB_CFLAGS"
  LDFLAGS="$CFLAGS $ZLIB_LDFLAGS"
  AC_CHECK_HEADER([zlib.h],
    [AC_CHECK_LIB([z], [gzopen],
       [ZLIB_LDFLAGS="${ZLIB_LDFLAGS} -lz"
        AC_DEFINE([HAVE_ZLIB], 1, [Define to 1 if we can use zlib])])])
  CFLAGS=$oln_save_CFLAGS
  LDFLAGS=$oln_save_LDFLAGS
fi
AC_SUBST([ZLIB_CFLAGS])
AC_SUBST([ZLIB_LDFLAGS])

AC_DEFINE_UNQUOTED([CXX], ["$CXX"], [The C++ compiler])

AC_CONFIG_FILES([
  Makefile
  oln/Makefile
  demo/Makefile
  tests/defs
  tests/Makefile
  tests/array/Makefile
  tests/check/Makefile
  tests/convert/Makefile
  tests/convol/Makefile
  tests/io/Makefile
  tests/morpho/Makefile
  tests/sanity/Makefile
])
AC_OUTPUT
