# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

m4_include(oln-version.m4)

AC_INIT([oln-meta], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_AUX_DIR([oln])
AC_CONFIG_SRCDIR([oln-version.m4])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

AC_CONFIG_SUBDIRS([oln])

OLN_COLLECTION([OLN_TOP], [dnl

OLN_COMPONENT([doc], [doc], [oln_cv_build_doc], [the documentation],
              [OLN_TOP], [],
              [AC_CONFIG_SUBDIRS([doc])])

OLN_COMPONENT([tools], [tools], [oln_cv_build_tools], [the run-time tools],
	      [OLN_TOP], [],
              [AC_CONFIG_SUBDIRS([tools])])
])

#### Complementary configure arguments, report from subdirectories ####
AC_ARG_WITH([fftw], [AC_HELP_STRING([--with-fftw@<:@=DIR@:>@], [using fftw (DIR = prefix for fftw installation)])])
AC_ARG_WITH([zlib], [AC_HELP_STRING([--with-zlib@<:@=DIR@:>@], [using zlib (DIR = prefix for zlib installation)])])
AC_ARG_WITH([testsuite], [AC_HELP_STRING([--without-testsuite], [disable the testsuite])])
AC_ARG_WITH([tests], [AC_HELP_STRING([--with-tests=<list>], 
                                     [none,all,convert,convol,io,meta,morpho,sanity,transforms,types (default all)])])
AC_ARG_WITH([manuals], [AC_HELP_STRING([--without-manuals], [do not build the Olena manuals])])
AC_ARG_WITH([demo], [AC_HELP_STRING([--without-demo], [do not build the demonstration programs])])
AC_ARG_WITH([utilities], AC_HELP_STRING([--without-utilities], [disable building the command-line utilities]))
AC_ARG_WITH([swilena], AC_HELP_STRING([--without-swilena], [disable generation of Olena SWIG modules]))

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
