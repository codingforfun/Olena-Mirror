# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

source oln-version

AC_INIT([$OLN_UTILITIES_PNAME], [$OLN_UTILITIES_VERSION], [$OLN_UTILITIES_CONTACT])
AC_CONFIG_SRCDIR([oln-version])
AM_INIT_AUTOMAKE([1.6 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

AC_CONFIG_HEADERS([config.h:config.hin])
AC_CONFIG_FILES([Makefile])

oln_dir=no
if test -r $srcdir/../oln/config/pconf.hh; then
  oln_dir=..
fi
AM_PATH_OLN([$oln_dir])
AC_WITH_OLN

# Utilities may use FFTW and ZLIB
AC_WITH_ZLIB
AC_WITH_FFTW

OLN_UTILITIES_SUBDIRS=""

if test -d $srcdir/morpho; then
   AC_ARG_WITH([morpho-utilities],
               AC_HELP_STRING([--without-morpho-utilities],
                              [disable building the morpho utilities])
               [ac_cv_oln_utils_morpho], [ac_cv_oln_utils_morpho=yes])
   AC_CACHE_CHECK([whether to build the morpho utilities],
                  [ac_cv_oln_utils_morpho], [ac_cv_oln_utils_morpho=yes])

  if x$ac_cv_oln_utils_morpho != xno; then
      OLN_UTILITIES_SUBDIRS="$OLN_UTILITIES_SUBDIRS morpho"
      AC_CONFIG_FILES([morpho/Makefile])
  fi
fi

AC_OUTPUT