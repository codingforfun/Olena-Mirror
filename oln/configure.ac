# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.57])

m4_include(../oln-version.m4)
AC_INIT([oln], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_SRCDIR([oln.m4])
AM_INIT_AUTOMAKE([1.7.2 no-define foreign dist-bzip2])

### Tests require these features :

OLN_PATH_LOCAL([.])
OLN_PATH_LOCAL_IMGS([img])
AC_WITH_OLN
	
# libcheck.a creation requires ranlib.
AC_PROG_RANLIB

# Tests may use FFTW and ZLIB
AC_WITH_CXX_ZLIB
AC_WITH_CXX_FFTW

# Use warnings
AC_CXX_FLAGS

### End of feature tests.

AC_CONFIG_FILES([Makefile 
		 oln/Makefile 
		 img/Makefile 
		 oln/config/pconf.hh:oln/config/pconf-hh.in])

### Optional components :

OLN_COLLECTION([OLN_SRC], [dnl
OLN_COMPONENT([tests], [testsuite], [oln_cv_build_tests], [the testsuite],
              [OLN_SRC], [tests/Makefile
                          tests/defs
	                  tests/check/Makefile],
[dnl
  # Tests auto-generate stuff using the C++ compiler and its flags
  AC_DEFINE_UNQUOTED([CXX], ["$CXX"], [The C++ compiler])
  AC_DEFINE_UNQUOTED([CXXFLAGS], ["$CXXFLAGS"], [The C++ compiler flags])
  AC_DEFINE_UNQUOTED([CPPFLAGS], ["$CPPFLAGS"], [The C/C++ preprocessor flags])
  AC_DEFINE_UNQUOTED([OLN_CXXFLAGS], ["$OLN_CXXFLAGS"], 
                     [Define to Olena-specific C++ compiler flags])
  AC_DEFINE_UNQUOTED([OLN_CPPFLAGS], ["$OLN_CPPFLAGS"], 
                     [Define to Olena-specific C/C++ preprocessor flags])
  AC_DEFINE_UNQUOTED([CXXFLAGS_OPTIMIZE], ["$CXXFLAGS_OPTIMIZE"], 
		     [Define to C++ compiler flags for optimization])
  AC_DEFINE_UNQUOTED([CXXFLAGS_STRICT], ["$CXXFLAGS_STRICT"], 
		     [Define to C++ compiler flags for strict conformance])
  AC_DEFINE_UNQUOTED([CXXFLAGS_STRICT_ERRORS], ["$CXXFLAGS_STRICT_ERRORS"], 
		     [Define to C++ compiler flags for strict conformance checking])
  AC_DEFINE_UNQUOTED([OLN_IMG_DIR], ["$OLN_IMG_DIR"], 
	             [Define to the location of Olena images])
  AC_DEFINE_UNQUOTED([OLN_IMG_AUX_DIR], ["$OLN_IMG_AUX_DIR"], 
	             [Define to the writable location of Olena images])

  AC_CONFIG_HEADERS([config.h:config.hin])

  OLN_TESTS_SUBDIRS=""

  AC_CACHE_CHECK([for selection of tests],
                 [oln_cv_tests_selection],
                 [AC_ARG_WITH([tests],
  	                    [AC_HELP_STRING([--with-tests=<list>],
          [none,all,convert,convol,io,meta,morpho,sanity,transforms,types])],
                              [oln_cv_tests_selection=$withval],
                              [oln_cv_tests_selection=all])])

  AC_DEFUN([OLN_TESTS], [dnl
  AC_CACHE_CHECK([whether to enable tests in `$1'],
                 [oln_cv_tests_$1],
                 [oln_cv_tests_$1=no
                  if test -d $srcdir/tests/$1; then
                    case "$oln_cv_tests_selection" in
                      *all* | *$1* )
                        oln_cv_tests_$1=yes
                        ;;
                    esac
                  fi])
  if test x$oln_cv_tests_$1 != xno; then
     OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS $1"
     AC_CONFIG_FILES([tests/$1/Makefile])
  fi])

  OLN_TESTS([arith])
  OLN_TESTS([convert])
  OLN_TESTS([convol])
  OLN_TESTS([io])
  OLN_TESTS([meta])
  OLN_TESTS([morpho])
  OLN_TESTS([sanity])
  OLN_TESTS([transforms])
  OLN_TESTS([types])

  AC_SUBST([OLN_TESTS_SUBDIRS])
  ])
])

AC_OUTPUT
