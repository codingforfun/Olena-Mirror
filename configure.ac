# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])
AC_INIT([olena], [0.5], [olena-bugs@lrde.epita.fr])
AC_CONFIG_SRCDIR([ChangeLog])
AC_CONFIG_AUX_DIR([tools])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])

AC_CONFIG_HEADERS(oln/config/config.h:oln/config/config.hin)

AC_PROG_CXX
AC_PROG_RANLIB

AC_LANG(C++)
AC_CHECK_HEADERS([limits stl_config.h])

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

# Check for fftw.
AC_ARG_WITH(fftw,
  [AC_HELP_STRING([--with-fftw=DIR], [prefix for zlib installation])])
FFTW_CFLAGS=''
FFTW_LDFLAGS=''
if test "x$with_fftw" != xno; then
  if test -n "$with_fftw"; then
    FFTW_CFLAGS="-I${with_zlib}/include"
    FFTW_LDFLAGS="-L${with_zlib}/lib"
  fi
  oln_save_CFLAGS=$CFLAGS
  oln_save_LDFLAGS=$LDFLAGS
  CFLAGS="$CFLAGS $FFTW_CFLAGS"
  LDFLAGS="$CFLAGS $FFTW_LDFLAGS"
  AC_CHECK_HEADER([fftw.h],
    [AC_CHECK_LIB([fftw], [fftw2d_create_plan],
       [FFTW_LDFLAGS="${FFTW_LDFLAGS} -lfftw -lrfftw"
        AC_DEFINE([HAVE_FFTW], 1, [Define to 1 if we can use fftw])])])
  CFLAGS=$oln_save_CFLAGS
  LDFLAGS=$oln_save_LDFLAGS
fi
AC_SUBST([FFTW_CFLAGS])
AC_SUBST([FFTW_LDFLAGS])

AC_DEFINE_UNQUOTED([CXX], ["$CXX"], [The C++ compiler])

AC_CONFIG_FILES([
  Makefile
  oln/Makefile
  demo/Makefile
  tests/defs
  tests/Makefile
  tests/check/Makefile
  tests/convert/Makefile
  tests/convol/Makefile
  tests/io/Makefile
  tests/meta/Makefile
  tests/morpho/Makefile
  tests/sanity/Makefile
  tests/transforms/Makefile
  tests/types/Makefile
  utilities/Makefile
  utilities/morpho/Makefile
])
AC_OUTPUT
