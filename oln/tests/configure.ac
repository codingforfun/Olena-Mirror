# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

m4_include(../../oln-version.m4)

AC_INIT([oln], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_AUX_DIR([..])
AC_CONFIG_SRCDIR([defs.in])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

# check for Olena sources and flags
OLN_PATH_LOCAL([..])
AC_WITH_OLN

# libcheck.a creation requires ranlib.
AC_PROG_RANLIB

# Tests may use FFTW and ZLIB
AC_WITH_CXX_ZLIB
AC_WITH_CXX_FFTW

# Tests auto-generate stuff using the C++ compiler and its flags
AC_DEFINE_UNQUOTED([CXX], ["$CXX"], [The C++ compiler])
AC_DEFINE_UNQUOTED([CXXFLAGS], ["$CXXFLAGS"], [The C++ compiler flags])
AC_DEFINE_UNQUOTED([CPPFLAGS], ["$CPPFLAGS"], [The C/C++ preprocessor flags])
AC_DEFINE_UNQUOTED([OLN_CXXFLAGS], ["$OLN_CXXFLAGS"], [The Olena-specific C++ compiler flags])
AC_DEFINE_UNQUOTED([OLN_CPPFLAGS], ["$OLN_CPPFLAGS"], [The Olena-specific C/C++ preprocessor flags])

AC_CONFIG_HEADERS([config.h:config.hin])

OLN_TESTS_SUBDIRS=""

AC_CACHE_CHECK([for selection of tests],
               [oln_cv_tests_selection],
               [AC_ARG_WITH([tests],
	                    [AC_HELP_STRING([--with-tests=<list>],
                                            [none,all,convert,convol,io,meta,morpho,sanity,transforms,types])],
                            [oln_cv_tests_selection=$withval],
                            [oln_cv_tests_selection=all])])

AC_DEFUN([OLN_TESTS], [dnl
AC_CACHE_CHECK([whether to enable tests in `$1'],
               [oln_cv_tests_$1],
               [oln_cv_tests_$1=no
                if test -d $srcdir/$1; then
                  case "$oln_cv_tests_selection" in
                    *all* | *$1* )
                      oln_cv_tests_$1=yes
                      ;;
                  esac
                fi])
if test x$oln_cv_tests_$1 != xno; then
   OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS $1"
   AC_CONFIG_FILES([$1/Makefile])
fi])

OLN_TESTS([arith])
OLN_TESTS([convert])
OLN_TESTS([convol])
OLN_TESTS([io])
OLN_TESTS([meta])
OLN_TESTS([morpho])
OLN_TESTS([sanity])
OLN_TESTS([transforms])
OLN_TESTS([types])

AC_SUBST([OLN_TESTS_SUBDIRS])

AC_CONFIG_FILES([defs
	Makefile
	check/Makefile])

AC_OUTPUT
