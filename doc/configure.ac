# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.57])

m4_include(../oln-version.m4)

AC_INIT([oln-doc], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_SRCDIR([ChangeLog])
AM_INIT_AUTOMAKE([1.7.2 no-define foreign dist-bzip2])

AC_CONFIG_FILES([Makefile])

oln_dir=no
if test -r $srcdir/../oln/oln/config/pconf.hh; then
  oln_dir=../oln
fi
OLN_PATH_LOCAL([$oln_dir])
AC_WITH_OLN

OLN_COLLECTION([OLN_DOC], [dnl

OLN_COMPONENT([doc], [manuals], [oln_cv_build_ref], [the Olena manuals],
              [OLN_DOC], [],
              [dnl
      oln_doc_dir=doc

      dnl Check for tools needed for formatting the documentation.
      AC_CHECK_PROGS([DVIPS], [dvips], [$am_aux_dir/missing dvips])

      AC_CHECK_PROGS([TEXI2DVI], [texi2dvi])
      if test -z "$TEXI2DVI"; then
         oln_doc_dir=''
         AC_MSG_WARN([texi2dvi not found -- documentation cannot be built])
      fi

      AC_CHECK_PROGS([GETDEFS], [getdefs])
      if test -z "$GETDEFS"; then
         oln_doc_dir=''
         AC_MSG_WARN([getdefs not found -- documentation cannot be built])
      fi

      AC_CHECK_PROGS([HEVEA], [hevea])
      if test -z "$HEVEA"; then
         oln_doc_dir=''
         AC_MSG_WARN([hevea not found -- documentation cannot be built])
      fi

      if test -n "$oln_doc_dir"; then
         OLN_DOC_SUBDIRS="$OLN_DOC_SUBDIRS $oln_doc_dir"
         AC_CONFIG_FILES([doc/Makefile])
      fi])

OLN_COMPONENT([demo], [demo], [oln_cv_build_demo], [the demonstration programs],
              [OLN_DOC], [demo/Makefile],
              [AC_WITH_CXX_WARNINGS])
])

AC_CONFIG_HEADERS([config.h:config.hin])

AC_OUTPUT
