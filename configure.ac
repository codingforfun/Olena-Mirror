# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

source oln-version

AC_INIT([$OLN_PNAME], [$OLN_VERSION], [$OLN_CONTACT])
AC_CONFIG_SRCDIR([oln-version])
AM_INIT_AUTOMAKE([1.6 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

AC_CONFIG_SUBDIRS([oln])

OLN_SUBDIRS=""
if test -d $srcdir/doc; then
  AC_ARG_WITH([doc], 
              AC_HELP_STRING([--without-doc],
		             [disable building the documentation]),
              [ac_cv_oln_doc], [ac_cv_oln_doc=yes])
  AC_CACHE_CHECK([whether to build the documentation],
                 [ac_cv_oln_doc], [ac_cv_oln_doc=yes])

  if test x$ac_cv_oln_doc != xno; then
     if test -w $srcdir/doc/
        cp -f $srcdir/oln-version $srcdir/doc/
     fi
     OLN_SUBDIRS="$OLN_SUBDIRS doc"
     AC_CONFIG_SUBDIRS([doc])
  fi
fi

if test -d $srcdir/utilities; then
  AC_ARG_WITH([utilities], 
              AC_HELP_STRING([--without-utilities],
		             [disable building the documentation]),
              [ac_cv_oln_utils], [ac_cv_oln_utils=yes])
  AC_CACHE_CHECK([whether to build the utilities],
                 [ac_cv_oln_utils], [ac_cv_oln_utils=yes])


  if test x$ac_cv_oln_utils != xno; then
     if test -w $srcdir/utilities/
        cp -f $srcdir/oln-version $srcdir/utilities/
     fi
     OLN_SUBDIRS="$OLN_SUBDIRS utilities"
     AC_CONFIG_SUBDIRS([utilities])
  fi
fi

if test -d $srcdir/swilena; then
  AC_ARG_WITH([swilena], 
              AC_HELP_STRING([--without-swilena],
		             [disable building Swilena]),
              [ac_cv_swilena], [ac_cv_swilena=yes])
  AC_CACHE_CHECK([whether to build the swilena],
                 [ac_cv_swilena], [ac_cv_swilena=yes])


  if test x$ac_cv_swilena != xno; then
     if test -w $srcdir/swilena/
        cp -f $srcdir/oln-version $srcdir/swilena/
     fi
     OLN_SUBDIRS="$OLN_SUBDIRS swilena"
     AC_CONFIG_SUBDIRS([swilena])
  fi
fi

AC_SUBST([OLN_SUBDIRS]
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
