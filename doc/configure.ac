# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

include (../oln-version.m4)

AC_INIT([oln-doc], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_SRCDIR([ChangeLog])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

AC_CONFIG_FILES([Makefile])

oln_dir=no
if test -r $srcdir/../oln/oln/config/pconf.hh; then
  oln_dir=../oln
fi
AM_PATH_OLN([$oln_dir])
AC_WITH_OLN

OLN_DOC_SUBDIRS=""

if test -d $srcdir/doc; then
   AC_ARG_WITH([manuals],
               AC_HELP_STRING([--without-manuals],
                              [disable building the Olena manuals]),
               [ac_cv_oln_ref=$withval], [ac_cv_oln_ref=yes])
   AC_CACHE_CHECK([whether to build the manuals],
                  [ac_cv_oln_ref], [ac_cv_oln_ref=yes])
   
   if test x$ac_cv_oln_ref != xno; then

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
 
      OLN_DOC_SUBDIRS="$OLN_DOC_SUBDIRS $oln_doc_dir"
      AC_CONFIG_FILES([doc/Makefile])
  fi
fi

if test -d $srcdir/demo; then
   AC_ARG_WITH([demo],
               AC_HELP_STRING([--without-demo],
                              [disable building the demonstration programs]),
               [ac_cv_oln_demo=$withval], [ac_cv_oln_demo=yes])
   AC_CACHE_CHECK([whether to build the demonstration programs],
                  [ac_cv_oln_demo], [ac_cv_oln_demo=yes])

  if test x$ac_cv_oln_demo != xno; then
      OLN_DOC_SUBDIRS="$OLN_DOC_SUBDIRS demo"
      AC_CONFIG_FILES([demo/Makefile])
  fi
fi

AC_SUBST([OLN_DOC_SUBDIRS])
AC_CONFIG_HEADERS([config.h:config.hin])

AC_OUTPUT
