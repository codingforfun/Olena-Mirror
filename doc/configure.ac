# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])
AC_INIT([olena-doc], [0.5], [olena-bugs@lrde.epita.fr])
AC_CONFIG_SRCDIR([ChangeLog])
AC_CONFIG_AUX_DIR([../tools])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])

dnl Check for tools needed for formatting the documentation.
AC_CHECK_PROGS([DVIPS], [dvips], [$am_aux_dir/missing dvips])

AC_CHECK_PROGS([TEXI2DVI], [texi2dvi])
if test -z "$TEXI2DVI"; then
   AC_MSG_ERROR([texi2dvi not found -- please install Texinfo])
fi

AC_CHECK_PROGS([GETDEFS], [getdefs])
if test -z "$GETDEFS"; then
   AC_MSG_ERROR([getdefs not found -- please install AutoGen])
fi

AC_CHECK_PROGS([HEVEA], [hevea])
if test -z "$HEVEA"; then
   AC_MSG_ERROR([hevea not found -- please install HeVeA])
fi

AC_PROG_CXX

AC_CONFIG_FILES([Makefile doc/Makefile])
AC_OUTPUT
