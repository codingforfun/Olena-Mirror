# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

m4_include(../oln-version.m4)

AC_INIT([oln-tools], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_SRCDIR([ChangeLog])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

oln_dir=no
if test -r $srcdir/../oln/oln/config/pconf.hh; then
  oln_dir=../oln
fi
OLN_PATH_LOCAL([$oln_dir])
AC_WITH_OLN

AC_WITH_CXX_ZLIB
AC_WITH_CXX_FFTW

AC_WITH_CXX_WARNINGS

AC_CONFIG_HEADERS([config.h:config.hin])

OLN_COLLECTION([OLN_TOOLS], [dnl
OLN_COMPONENT([utilities], [utilities], [oln_cv_build_utilities], [the command-line utilities],
              [OLN_TOOLS], [utilities/Makefile], 
	      [OLN_COLLECTION([OLN_UTILITIES], [dnl
               OLN_COMPONENT([utilities/morpho], [morpho-utilities], [oln_cv_build_morpho_utils], [the morpho utilities],
                             [OLN_UTILITIES], [utilities/morpho/Makefile], [], [morpho])
               ])])
], [Makefile])

AC_OUTPUT
