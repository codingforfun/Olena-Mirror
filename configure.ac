# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.52])
AC_INIT([olena], [0.4], [olena-bugs@lrde.epita.fr])
AC_CONFIG_SRCDIR([ChangeLog])
AC_CONFIG_AUX_DIR([tools])
AM_INIT_AUTOMAKE([no-define])

AM_CONFIG_HEADER(src/config/config.h:src/config/config.hin)

AC_PROG_CXX
AC_PROG_RANLIB

AC_LANG(C++)
AC_CHECK_HEADERS([hash_map ext/hash_map])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  demo/Makefile
  tests/Makefile
  tests/check/Makefile
  tests/convert/Makefile
  tests/io/Makefile
  tests/morpho/Makefile
])
AC_OUTPUT
