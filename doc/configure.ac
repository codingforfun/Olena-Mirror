# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])
AC_INIT([olena-doc], [0.5], [olena-bugs@lrde.epita.fr])
AC_CONFIG_SRCDIR([ChangeLog])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

oln_dir=no
if test -r $srcdir/../oln/config/pconf.hh; then
  oln_dir=..
fi
AM_PATH_OLN([$oln_dir])
AC_WITH_OLN

DOC_DIR=''
AC_ARG_ENABLE(doc-build,
  [AC_HELP_STRING([--disable-doc-build], [disable generation of documentation])])

if test "x$enable_doc_build" != xno; then
   DOC_DIR='doc'

   dnl Check for tools needed for formatting the documentation.
   AC_CHECK_PROGS([DVIPS], [dvips], [$am_aux_dir/missing dvips])

   AC_CHECK_PROGS([TEXI2DVI], [texi2dvi])
   if test -z "$TEXI2DVI"; then
      DOC_DIR=''
      AC_MSG_WARN([texi2dvi not found -- documentation cannot be built])
   fi

   AC_CHECK_PROGS([GETDEFS], [getdefs])
   if test -z "$GETDEFS"; then
      DOC_DIR=''
      AC_MSG_WARN([getdefs not found -- documentation cannot be built])
   fi

   AC_CHECK_PROGS([HEVEA], [hevea])
   if test -z "$HEVEA"; then
      DOC_DIR=''
      AC_MSG_WARN([hevea not found -- documentation cannot be built])
   fi

else
  AC_MSG_WARN([documentation build disabled on user request])
fi

AC_SUBST([DOC_DIR])

AC_CONFIG_FILES([Makefile doc/Makefile])
AC_OUTPUT
