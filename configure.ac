# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

include (oln-version.m4)

AC_INIT([oln-meta], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_AUX_DIR([oln])
AC_CONFIG_SRCDIR([oln-version.m4])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

AC_CONFIG_SUBDIRS([oln])

OLN_SUBDIRS=""
if test -d $srcdir/doc; then
  AC_ARG_WITH([doc], 
              AC_HELP_STRING([--without-doc],
		             [disable building the documentation]),
              [ac_cv_oln_doc=$withval], [ac_cv_oln_doc=yes])
  AC_CACHE_CHECK([whether to build the documentation],
                 [ac_cv_oln_doc], [ac_cv_oln_doc=yes])

  if test x$ac_cv_oln_doc != xno; then 
     OLN_SUBDIRS="$OLN_SUBDIRS doc"
     AC_CONFIG_SUBDIRS([doc])
  fi
fi

if test -d $srcdir/tools; then
  AC_ARG_WITH([tools], 
              AC_HELP_STRING([--without-tools],
		             [disable building the runtime tools]),
              [ac_cv_oln_tools=$withval], [ac_cv_oln_tools=yes])
  AC_CACHE_CHECK([whether to build the tools],
                 [ac_cv_oln_tools], [ac_cv_oln_tools=yes])


  if test x$ac_cv_oln_tools != xno; then
     OLN_SUBDIRS="$OLN_SUBDIRS tools"
     AC_CONFIG_SUBDIRS([tools])
  fi
fi

#### Complementary configure arguments, report from subdirectories ####
AC_ARG_WITH([fftw], AC_HELP_STRING([--with-fft[=DIR]], [using fftw (DIR = prefix for fftw installation)]))
AC_ARG_WITH([zlib], AC_HELP_STRING([--with-zlib[=DIR]], [using zlib (DIR = prefix for zlib installation)]))
AC_ARG_WITH([tests], AC_HELP_STRING([--without-tests], [disable the testsuite]))
AC_ARG_WITH([manuals], AC_HELP_STRING([--without-manuals], [disable building the Olena manuals]))
AC_ARG_WITH([utilities], AC_HELP_STRING([--without-utilities], [disable building the command-line utilities]))
AC_ARG_WITH([swilena], AC_HELP_STRING([--without-swilena], [disable generation of Olena SWIG modules]))

AC_SUBST([OLN_SUBDIRS])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
