# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.57])

AC_INIT([oln-tools], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_SRCDIR([ChangeLog])
AM_INIT_AUTOMAKE([1.7.2 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

oln_rel_dir=no
if test -r "$srcdir/../oln/oln/config/system.hh"; then
  oln_rel_dir=../oln
fi
OLN_PATH_LOCAL([$oln_rel_dir])
OLN_PATH_LOCAL([$oln_rel_dir/img])
AC_WITH_OLN

AC_WITH_CXX_ZLIB
AC_WITH_CXX_FFTW

AC_CXX_FLAGS

AC_CONFIG_HEADERS([config.h:config.hin])
AC_CONFIG_FILES([Makefile])

OLN_COLLECTION([OLN_TOOLS], [dnl
OLN_COMPONENT([utilities], [utilities], 
	      [oln_cv_build_utilities], 
              [the command-line utilities],
              [OLN_TOOLS], [utilities/Makefile],
	      [OLN_COLLECTION([OLN_UTILITIES], [dnl
               OLN_COMPONENT([utilities/morpho], [morpho-utilities], 
			     [oln_cv_build_morpho_utils], 
	                     [the morpho utilities],
                             [OLN_UTILITIES], [utilities/morpho/Makefile], 
                             [], [morpho])
               ])])
])

AC_OUTPUT
