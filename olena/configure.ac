# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

AC_INIT([Olena], 0.8, OLN_CONTACT, [olena])
AC_CONFIG_AUX_DIR([config])
#FIXME
AC_CONFIG_SRCDIR([m4/olena.m4])
AM_INIT_AUTOMAKE([1.7 no-define foreign dist-bzip2])
AC_CONFIG_HEADERS([config.h:config.hin])

# FIXME: Insert stuff related to CXX FLAGS

# AC_WITH_MLC
AC_WITH_NTG

if test x$have_integre = xno; then
   AC_MSG_ERROR([The Intègre library is required. You can get from http://www.lrde.epita.fr])
fi

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
