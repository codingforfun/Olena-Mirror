# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

m4_include(../../oln-version.m4)

AC_INIT([oln], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_AUX_DIR([..])
AC_CONFIG_SRCDIR([defs.in])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

# check for Olena sources and flags
OLN_PATH_LOCAL([..])
AC_WITH_OLN

# libcheck.a creation requires ranlib.
AC_PROG_RANLIB

# Tests may use FFTW and ZLIB
AC_WITH_CXX_ZLIB
AC_WITH_CXX_FFTW

# Tests auto-generate stuff using the C++ compiler and its flags
AC_DEFINE_UNQUOTED([CXX], ["$CXX"], [The C++ compiler])
AC_DEFINE_UNQUOTED([CXXFLAGS], ["$CXXFLAGS"], [The C++ compiler flags])
AC_DEFINE_UNQUOTED([CPPFLAGS], ["$CPPFLAGS"], [The C/C++ preprocessor flags])
AC_DEFINE_UNQUOTED([OLN_CXXFLAGS], ["$OLN_CXXFLAGS"], [The Olena-specific C++ compiler flags])
AC_DEFINE_UNQUOTED([OLN_CPPFLAGS], ["$OLN_CPPFLAGS"], [The Olena-specific C/C++ preprocessor flags])

AC_CONFIG_HEADERS([config.h:config.hin])

OLN_TESTS_SUBDIRS=""

AC_CACHE_CHECK([for selection of tests],
               [oln_cv_tests_selection],
               [AC_ARG_WITH([tests],
	                    [AC_HELP_STRING([--with-tests=<list>],
                                            [none,all,convert,convol,io,meta,morpho,sanity,transforms,types])],
                            [oln_cv_tests_selection=$withval],
                            [oln_cv_tests_selection=all])])

AC_CACHE_CHECK([whether to enable tests in `convert'],
               [oln_cv_tests_convert],
               [oln_cv_tests_convert=no
                if test -d $srcdir/convert; then
                   case "$oln_cv_tests_selection" in
                     *all* | *convert* )
                         oln_cv_tests_convert=yes
                         ;;
                   esac
                fi])
if test x$oln_cv_tests_convert != xno; then
   OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS convert"
   AC_CONFIG_FILES([convert/Makefile])
fi                       

AC_CACHE_CHECK([whether to enable tests in `convol'],
               [oln_cv_tests_convol],
               [oln_cv_tests_convol=no
                if test -d $srcdir/convol; then
                   case "$oln_cv_tests_selection" in
                     *all* | *convol* )
                         oln_cv_tests_convol=yes
                         ;;
                   esac
                fi])
if test x$oln_cv_tests_convol != xno; then
   OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS convol"
   AC_CONFIG_FILES([convol/Makefile])
fi                       

AC_CACHE_CHECK([whether to enable tests in `io'],
               [oln_cv_tests_io],
               [oln_cv_tests_io=no
                if test -d $srcdir/io; then
                   case "$oln_cv_tests_selection" in
                     *all* | *io* )
                         oln_cv_tests_io=yes
                         ;;
                   esac
                fi])
if test x$oln_cv_tests_io != xno; then
   OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS io"
   AC_CONFIG_FILES([io/Makefile])
fi                       

AC_CACHE_CHECK([whether to enable tests in `meta'],
               [oln_cv_tests_meta],
               [oln_cv_tests_meta=no
                if test -d $srcdir/meta; then
                   case "$oln_cv_tests_selection" in
                     *all* | *meta* )
                         oln_cv_tests_meta=yes
                         ;;
                   esac
                fi])
if test x$oln_cv_tests_meta != xno; then
   OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS meta"
   AC_CONFIG_FILES([meta/Makefile])
fi                       

AC_CACHE_CHECK([whether to enable tests in `morpho'],
               [oln_cv_tests_morpho],
               [oln_cv_tests_morpho=no
                if test -d $srcdir/morpho; then
                   case "$oln_cv_tests_selection" in
                     *all* | *morpho* )
                         oln_cv_tests_morpho=yes
                         ;;
                   esac
                fi])
if test x$oln_cv_tests_morpho != xno; then
   OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS morpho"
   AC_CONFIG_FILES([morpho/Makefile])
fi                       

AC_CACHE_CHECK([whether to enable tests in `sanity'],
               [oln_cv_tests_sanity],
               [oln_cv_tests_sanity=no
                if test -d $srcdir/sanity; then
                   case "$oln_cv_tests_selection" in
                     *all* | *sanity* )
                         oln_cv_tests_sanity=yes
                         ;;
                   esac
                fi])
if test x$oln_cv_tests_sanity != xno; then
   OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS sanity"
   AC_CONFIG_FILES([sanity/Makefile])
fi                       

AC_CACHE_CHECK([whether to enable tests in `transforms'],
               [oln_cv_tests_transforms],
               [oln_cv_tests_transforms=no
                if test -d $srcdir/transforms; then
                   case "$oln_cv_tests_selection" in
                     *all* | *transforms* )
                         oln_cv_tests_transforms=yes
                         ;;
                   esac
                fi])
if test x$oln_cv_tests_transforms != xno; then
   OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS transforms"
   AC_CONFIG_FILES([transforms/Makefile])
fi                       

AC_CACHE_CHECK([whether to enable tests in `types'],
               [oln_cv_tests_types],
               [oln_cv_tests_types=no
                if test -d $srcdir/types; then
                   case "$oln_cv_tests_selection" in
                     *all* | *types* )
                         oln_cv_tests_types=yes
                         ;;
                   esac
                fi])
if test x$oln_cv_tests_types != xno; then
   OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS types"
   AC_CONFIG_FILES([types/Makefile])
fi                       


AC_SUBST([OLN_TESTS_SUBDIRS])

AC_CONFIG_FILES([defs
	Makefile
	check/Makefile])

AC_OUTPUT

