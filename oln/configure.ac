# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.54])

m4_include(../oln-version.m4)

AC_INIT([oln], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_SRCDIR([oln.m4])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

# OLN_SOURCE_DIR back-tells Olena users where its headers are installed.
real_includedir=`eval echo $includedir`
AC_DEFINE_UNQUOTED([OLN_INCLUDE_DIR], ["$real_includedir"], [the directory where Olena headers are installed])
AC_CONFIG_HEADERS(oln/config/pconf.inc:oln/config/pconf-inc.in)


AC_CONFIG_FILES([Makefile oln/Makefile img/Makefile])

OLN_COLLECTION([OLN_SRC], [dnl
OLN_COMPONENT([tests], [testsuite], [oln_cv_build_tests], [the testsuite],
              [OLN_SRC], [],
              [AC_CONFIG_SUBDIRS([tests])])
])

AC_OUTPUT
