# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

AC_INIT([Olena], OLN_VERSION, OLN_CONTACT, [olena])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([config/oln.m4])
AM_INIT_AUTOMAKE([1.7 no-define foreign dist-bzip2])
AC_CONFIG_HEADERS([config.h:config.hin])

# FIXME: maybe not the best way to do this
if test ! ${CXXFLAGS+set}; then
   CXXFLAGS=""
fi

# Use included Olena version.
NTG_PATH_LOCAL([integre])
MLC_PATH_LOCAL([metalic])
OLN_PATH_LOCAL([olena])
AC_WITH_OLN


# Some library archives are used. Some C code used.
AC_PROG_RANLIB
AC_PROG_CC

# If available, use these.
AC_WITH_CXX_ZLIB
AC_WITH_CXX_FFTW

# Attempt to use strict language conformance checking.
AC_CXX_FLAGS

AC_CONFIG_FILES([Makefile
		 integre/Makefile
		 integre/ntg/Makefile
                 metalic/Makefile
                 metalic/mlc/Makefile
		 olena/Makefile
		 olena/oln/Makefile
                 olena/img/Makefile
		 olena/oln/config/pconf.hh:olena/oln/config/pconf-hh.in])


### Olena components.

OLN_COLLECTION([OLN_TOP],
[dnl

###
### Stuff pertaining to the main source tree
###

  OLN_COLLECTION([OLN_SRC],
  [dnl

###
### Experimental stuff. Read olena/conf/README
###

dnl ### Compiler-specific auto-configuration
dnl     AC_CACHE_CHECK([whether to enable auto-configuration for multiple compilers],
dnl                    [oln_cv_enable_config_sh],
dnl                    [oln_cv_enable_config_sh=yes
dnl                     AC_ARG_WITH([config-sh],
dnl                           [AC_HELP_STRING([--without-config-sh],
dnl                           [disable generation of oln-config-xxx.sh])],
dnl                           [oln_cv_enable_config_sh=$withval])])
dnl     AM_CONDITIONAL([OLN_CONFIG_SH], [test "x$oln_cv_enable_config_sh" != xno])


### The testsuite is a component.

    ##
    ## olena testsuite
    ##

    OLN_COMPONENT([olena/tests],
		  [oln-testsuite],
		  [oln_cv_build_tests],
		  [the olena's testsuite],
		  [OLN_SRC],
	          [olena/tests/Makefile
		   olena/tests/check/Makefile
		   olena/tests/check/defs],
    [dnl Configuration for the testsuite

      OLN_TESTS_SUBDIRS=""

      AC_CACHE_CHECK([for selection of tests],
                     [oln_cv_tests_selection],
                     [AC_ARG_WITH([oln-tests],
    	                          [AC_HELP_STRING([--with-oln-tests=<list>],
                                                  [all,none,core,sanity])],
                                  [oln_cv_tests_selection=$withval],
                                  [oln_cv_tests_selection=all])dnl
                     ])dnl


      # OLN_TESTS defined in oln-local.m4
      OLN_TESTS([sanity])
      OLN_TESTS([arith])
      OLN_TESTS([convert])
      OLN_TESTS([core])
      OLN_TESTS([io])
      OLN_TESTS([level])
      OLN_TESTS([morpho])
      OLN_TESTS([utils]) 
      
      AC_SUBST([OLN_TESTS_SUBDIRS])

    ], [tests])dnl End of OLN_COMPONENT([olena/tests])

  ])dnl End of OLN_COLLECTION([OLN_SRC])

  OLN_COLLECTION([NTG_SRC],
  [dnl

    ##
    ## integre testsuite
    ##

    OLN_COMPONENT([integre/tests],
		  [ntg-testsuite],
		  [ntg_cv_build_tests],
		  [the integre's testsuite],
		  [NTG_SRC],
	          [integre/tests/Makefile
		   integre/tests/check/defs],
    [dnl Configuration for the testsuite

      NTG_TESTS_SUBDIRS=""

      AC_CACHE_CHECK([for selection of tests],
                     [ntg_cv_tests_selection],
                     [AC_ARG_WITH([ntg-tests],
    	                          [AC_HELP_STRING([--with-ntg-tests=<list>],
                                                  [all,none,sanity])],
                                  [ntg_cv_tests_selection=$withval],
                                  [ntg_cv_tests_selection=all])dnl
                     ])dnl


      # NTG_TESTS defined in oln-local.m4
      NTG_TESTS([sanity])
      NTG_TESTS([types])
      NTG_TESTS([utils])

      AC_SUBST([NTG_TESTS_SUBDIRS])

    ], [tests])dnl End of OLN_COMPONENT([integre/tests])

  ])dnl End of OLN_COLLECTION([NTG_SRC])

  OLN_COLLECTION([MLC_SRC],
  [dnl

    ##
    ## metalic testsuite
    ##

    OLN_COMPONENT([metalic/tests],
		  [mlc-testsuite],
		  [mlc_cv_build_tests],
		  [the metalic's testsuite],
		  [MLC_SRC],
	          [metalic/tests/Makefile
                   metalic/tests/check/Makefile
		   metalic/tests/check/defs],
    [dnl Configuration for the testsuite

      MLC_TESTS_SUBDIRS=""

      AC_CACHE_CHECK([for selection of tests],
                     [mlc_cv_tests_selection],
                     [AC_ARG_WITH([mlc-tests],
    	                          [AC_HELP_STRING([--with-mlc-tests=<list>],
                                                  [all,none,sanity])],
                                  [mlc_cv_tests_selection=$withval],
                                  [mlc_cv_tests_selection=all])dnl
                     ])dnl


      # MLC_TESTS defined in oln-local.m4
      MLC_TESTS([sanity])
      MLC_TESTS([array])
      MLC_TESTS([main])

      AC_SUBST([MLC_TESTS_SUBDIRS])

    ], [tests])dnl End of OLN_COMPONENT([metalic/tests])

  ])dnl End of OLN_COLLECTION([MLC_SRC])

###
### Stuff pertaining to the documentation
###


### The documentation tree is both a component...

  OLN_NON_COMPONENT([doc],
	        [doc],
	        [oln_cv_build_doc],
	        [the documentation],
	        [OLN_TOP],
	        [doc/Makefile],
  [OLN_COLLECTION([OLN_DOC], dnl ... and a collection.
  [dnl

### The reference manuals directory is a component.

    OLN_NON_COMPONENT([doc/ref],
                      [doc-ref],
                      [oln_cv_build_ref],
                      [the Olena reference manuals],
                      [OLN_DOC],
                      [doc/ref/Makefile],
    [dnl Configuration for the manuals directory.

     ### The reference manual uses Doxygen
     ### FIXME: could be much better.

     AC_CONFIG_FILES([
                 doc/ref/out/exdoc.config:doc/ref/out/exdoc.config.in
                 doc/ref/exdoc.mk:doc/ref/exdoc.mk.in
                 doc/ref/doxygen.config:doc/ref/doxygen.config.in])


     AC_ARG_WITH([doc-ref],
                 [Doxygen is needed to build the doc],
                 [AC_CHECK_PROG([DOXYGEN], [doxygen], [doxygen])
                 AC_CHECK_DOXYGEN_VERSION],
                 [])

     AC_ARG_VAR([PERL], [Practical Extraction and Report Language])
     AC_CHECK_PROGS([PERL], [perl], [$am_aux_dir/missing perl])
     AC_ARG_VAR([TAR], [the tape archiver program])
     AC_CHECK_PROGS([TAR], [tar], [$am_aux_dir/missing tar])
     AC_ARG_VAR([DVIPS], [program to make Postscript from TeX DVI files])
     AC_CHECK_PROGS([DVIPS], [dvips], [$am_aux_dir/missing dvips])
     AC_ARG_VAR([TEXI2DVI], [program to create DVI from Texinfo files])
     AC_CHECK_PROGS([TEXI2DVI], [texi2dvi], [$am_aux_dir/missing texi2dvi])
     AC_ARG_VAR([GETDEFS], [the `getdefs' utility from Autogen])
     AC_CHECK_PROGS([GETDEFS], [getdefs], [$am_aux_dir/missing getdefs])
     AC_ARG_VAR([HEVEA], [program to make HTML pages from TeX])
     AC_CHECK_PROGS([HEVEA], [hevea], [$am_aux_dir/missing hevea])
     AC_ARG_VAR([HACHA], [program to make split HTML pages from TeX])
     AC_CHECK_PROGS([HACHA], [hacha], [$am_aux_dir/missing hacha])
     AC_ARG_VAR([EPSTOPDF], [program to convert Encapsulated Postscript images to PDF])
     AC_CHECK_PROGS([EPSTOPDF], [epstopdf], [$am_aux_dir/missing epstopdf])

   ], [ref])dnl End of OLN_COMPONENT([doc/ref])




  ])dnl End of OLN_COLLECTION([OLN_DOC])
  ])dnl End of OLN_COMPONENT([OLN_DOC])


   AM_CONDITIONAL([HAVE_DOC_DEV], [test "x$oln_cv_build_doc" != xno -a "x$oln_cv_build_devdoc" != xno])

])dnl End of OLN_COLLECTION([OLN_TOP])



AC_SUBST([DOC_CPPFLAGS])

AC_OUTPUT

if test "x$oln_cxxflags_clean" = xno; then
  AC_MSG_WARN([CXXFLAGS seems to contain both optimization and debugging flags ($CXXFLAGS).])
  AC_MSG_NOTICE([*])
  AC_MSG_NOTICE([* note that Olena sources are configured to add heavy optimization flags])
  AC_MSG_NOTICE([* by appending CXXFLAGS_OPTIMIZE ($CXXFLAGS_OPTIMIZE) to CXXFLAGS;])
  AC_MSG_NOTICE([* this can yield to compiler problems (bugs, invalid generated code)])
  AC_MSG_NOTICE([* when used with debugging options, or conflict with preset optimization])
  AC_MSG_NOTICE([* flags.])
  AC_MSG_NOTICE([*])
  AC_MSG_NOTICE([* Please read the file `BUILD' before continuing.])
  AC_MSG_NOTICE([*])
fi
