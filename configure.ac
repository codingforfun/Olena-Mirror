# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])
AC_INIT([olena], [0.5], [olena-bugs@lrde.epita.fr])
AC_CONFIG_SRCDIR([ChangeLog])
AC_CONFIG_AUX_DIR([tools])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

AC_CONFIG_HEADERS(oln/config/pconf.hh:oln/config/pconf-hh.in)

AC_CONFIG_SUBDIRS([src doc])

AC_CONFIG_FILES([
  Makefile
  oln/Makefile
])
AC_OUTPUT
