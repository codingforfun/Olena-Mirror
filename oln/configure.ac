# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

include (../oln-version.m4)

AC_INIT([oln], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_SRCDIR([oln.m4])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

# OLN_SOURCE_DIR back-tells Olena users where its headers are installed.
real_includedir=`eval echo $includedir`
AC_DEFINE_UNQUOTED([OLN_INCLUDE_DIR], ["$real_includedir"], [the directory where Olena headers are installed])
AC_CONFIG_HEADERS(oln/config/pconf.inc:oln/config/pconf-inc.in)

AC_CONFIG_FILES([Makefile oln/Makefile])

OLN_SRC_SUBDIRS=""
if test -d $srcdir/tests; then
   AC_ARG_WITH([tests],
               AC_HELP_STRING([--without-tests],
                              [disable the testsuite]),
               [ac_cv_oln_tests=$withval], [ac_cv_oln_tests=yes])
   AC_CACHE_CHECK([whether to enable the testsuite],
                  [ac_cv_oln_tests], [ac_cv_oln_tests=yes])
   
   if test x$ac_cv_oln_tests != xno; then
	OLN_SRC_SUBDIRS="$OLN_SRC_SUBDIRS tests"
	AC_CONFIG_SUBDIRS([tests])
   fi
fi

AC_SUBST([OLN_SRC_SUBDIRS])

AC_OUTPUT
