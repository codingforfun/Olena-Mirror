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

OLN_TOOLS_SUBDIRS=""

AC_CONFIG_FILES([Makefile])

AC_SUBST([OLN_TOOLS_SUBDIRS])

AC_OUTPUT
