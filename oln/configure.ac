# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

source oln-version

AC_INIT([$OLN_SRC_PNAME], [$OLN_SRC_VERSION], [$OLN_SRC_CONTACT])
AC_CONFIG_SRCDIR([oln-version])
AM_INIT_AUTOMAKE([1.6 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

real_includedir=`eval echo $includedir`
AC_DEFINE_UNQUOTED([OLN_SOURCE_DIR], ["$real_includedir/oln"], [the directory where Olena headers are installed])
AC_CONFIG_HEADERS(oln/config/pconf.inc:oln/config/pconf-inc.in)

AC_CONFIG_FILES([Makefile])

OLN_SRC_SUBDIRS=""
if test -d $srcdir/tests; then
   AC_ARG_WITH([tests],
               AC_HELP_STRING([--without-tests],
                              [disable the testsuite]),
               [ac_cv_oln_tests], [ac_cv_oln_tests=yes])
   AC_CACHE_CHECK([whether to enable the testsuite],
                  [ac_cv_oln_tests], [ac_cv_oln_tests=yes])
   
   if test x$ac_cv_oln_tests != xno; then

      # check for Olena sources and flags
      AM_PATH_OLN([.])
      AC_WITH_OLN


      # libcheck.a creation requires ranlib.
      AC_PROG_RANLIB

      # Tests may use FFTW and ZLIB
      AC_WITH_ZLIB
      AC_WITH_FFTW

      # Tests auto-generate stuff using the C++ compiler and its flags
      AC_DEFINE_UNQUOTED([CXX], ["$CXX"], [The C++ compiler])
      AC_DEFINE_UNQUOTED([CXXFLAGS], ["$CXXFLAGS"], [The C++ compiler flags])
      AC_DEFINE_UNQUOTED([CPPFLAGS], ["$CPPFLAGS"], [The C/C++ preprocessor flags])
      AC_DEFINE_UNQUOTED([OLN_CXXFLAGS], ["$OLN_CXXFLAGS"], [The Olena-specific C++ compiler flags])
      AC_DEFINE_UNQUOTED([OLN_CPPFLAGS], ["$OLN_CPPFLAGS"], [The Olena-specific C/C++ preprocessor flags])

      OLN_SRC_SUBDIRS="$OLN_SRC_SUBDIRS tests"
      AC_CONFIG_HEADERS([config.h:config.hin])
      AC_CONFIG_FILES([tests/defs
		       tests/Makefile
		       tests/check/Makefile
		       tests/sanity/Makefile
		       tests/meta/Makefile
		       tests/types/Makefile
                       tests/convol/Makefile
                       tests/transforms/Makefile
                       tests/morpho/Makefile
	               tests/io/Makefile
	               tests/convert/Makefile])
   fi
fi

AC_SUBST([OLN_SRC_SUBDIRS])

AC_OUTPUT
