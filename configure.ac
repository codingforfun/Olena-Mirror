# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

AC_INIT([oln], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_AUX_DIR([oln/config])
AC_CONFIG_SRCDIR([oln/config/oln.m4])
AM_INIT_AUTOMAKE([1.7 no-define foreign dist-bzip2])
AC_CONFIG_HEADERS([config.h:config.hin])

# Use included Olena version.
OLN_PATH_LOCAL([oln])
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
                 oln/Makefile 
		 oln/oln/Makefile 
		 oln/img/Makefile 
		 oln/oln/config/pconf.hh:oln/oln/config/pconf-hh.in])

### Olena components.

OLN_COLLECTION([OLN_TOP], 
[dnl

###
### Stuff pertaining to the main source tree
###

  OLN_COLLECTION([OLN_SRC], 
  [dnl

### Checks pertaining to the generation of auxiliary images

     AC_CHECK_PROGS([GZIP], [gzip], [$am_aux_dir/missing gzip])
     AC_CHECK_PROGS([PNGTOPNM], [pngtopnm], [$am_aux_dir/missing pngtopnm])
     AC_CHECK_PROGS([PPMTOPGM], [ppmtopgm], [$am_aux_dir/missing ppmtopgm])
     AC_CHECK_PROGS([PGMTOPBM], [pgmtopbm], [$am_aux_dir/missing pgmtopbm])
     AC_CHECK_PROGS([PNMTOPLAINPNM], [pnmtoplainpnm], [$am_aux_dir/missing pnmtoplainpnm])
     AC_CHECK_PROGS([IMAGEMAGICK_CONVERT], [convert], [$am_aux_dir/missing convert])

### Compiler-specific auto-configuration
    AC_CACHE_CHECK([whether to enable auto-configuration for multiple compilers],
                   [oln_cv_enable_config_sh],
                   [oln_cv_enable_config_sh=yes
                    AC_ARG_WITH([config-sh],
                          [AC_HELP_STRING([--without-config-sh],
                          [disable generation of oln-config-xxx.sh])],
                          [oln_cv_enable_config_sh=$withval])])
    AM_CONDITIONAL([OLN_CONFIG_SH], [test "x$oln_cv_enable_config_sh" != xno])
       

### The testsuite is a component.

    OLN_COMPONENT([oln/tests], 
		  [testsuite], 
		  [oln_cv_build_tests], 
		  [the testsuite], 
		  [OLN_SRC], 
	          [oln/tests/Makefile
                   oln/tests/check/defs
	           oln/tests/check/Makefile],
    [dnl Configuration for the testsuite

      OLN_TESTS_SUBDIRS=""

      AC_CACHE_CHECK([for selection of tests],
                     [oln_cv_tests_selection],
                     [AC_ARG_WITH([tests],
    	                          [AC_HELP_STRING([--with-tests=<list>],
                                                  [none,all,convert,convol,io,meta,morpho,sanity,transforms,types])],
                                  [oln_cv_tests_selection=$withval],
                                  [oln_cv_tests_selection=all])dnl
                     ])dnl
      

      # OLN_TESTS defined in oln-local.m4
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

    ], [tests])dnl End of OLN_COMPONENT([oln/tests])

  ])dnl End of OLN_COLLECTION([OLN_SRC])

###
### Stuff pertaining to the documentation
###  

### The documentation tree is both a component...

  OLN_COMPONENT([doc], 
	        [doc], 
	        [oln_cv_build_doc], 
	        [the documentation], 
	        [OLN_TOP],
	        [doc/Makefile],
  [OLN_COLLECTION([OLN_DOC], dnl ... and a collection.
  [dnl

### The reference manuals directory is a component.

    OLN_COMPONENT([doc/ref], 
	          [doc-ref], 
	          [oln_cv_build_ref], 
	          [the Olena reference manuals], 
	          [OLN_DOC], 
		  [doc/ref/Makefile],
    [dnl Configuration for the manuals directory.

     AC_CHECK_PROGS([TAR], [tar], [$am_aux_dir/missing tar])
     AC_CHECK_PROGS([DVIPS], [dvips], [$am_aux_dir/missing dvips])
     AC_CHECK_PROGS([TEXI2DVI], [texi2dvi], [$am_aux_dir/missing texi2dvi])
     AC_CHECK_PROGS([GETDEFS], [getdefs], [$am_aux_dir/missing getdefs])
     AC_CHECK_PROGS([HEVEA], [hevea], [$am_aux_dir/missing hevea])
     AC_CHECK_PROGS([IMAGEMAGICK_CONVERT], [convert], [$am_aux_dir/missing convert])
     AC_CHECK_PROGS([EPSTOPDF], [epstopdf], [$am_aux_dir/missing epstopdf])

    ], [ref])dnl End of OLN_COMPONENT([doc/ref])

### The development manuals directory is a component.

    OLN_COMPONENT([doc/dev], 
	          [doc-dev], 
	          [oln_cv_build_devdoc], 
	          [the Olena development manual], 
	          [OLN_DOC], 
		  [doc/dev/Makefile],
    [dnl Configuration for the dev directory.

     AC_CHECK_PROGS([DVIPS], [dvips], [$am_aux_dir/missing dvips])
     AC_CHECK_PROGS([TEXI2DVI], [texi2dvi], [$am_aux_dir/missing texi2dvi])

    ], [dev])dnl End of OLN_COMPONENT([doc/dev])

### The demonstration programs are a component.

    OLN_COMPONENT([doc/demo], 
	          [demo], 
	          [oln_cv_build_demo], 
	          [the demonstration programs], 
	          [OLN_DOC], 
	          [doc/demo/Makefile],
                  [], [demo])

  ])dnl End of OLN_COLLECTION([OLN_DOC])
  ])dnl End of OLN_COMPONENT([OLN_DOC])

###
### Stuff pertaining to the tools
###

### The tools tree is both a component...
  OLN_COMPONENT([tools], 
	        [tools], 
	        [oln_cv_build_tools], 
	        [the run-time tools], 
	        [OLN_TOP], 
                [tools/Makefile tools/lib/Makefile],
  [OLN_COLLECTION([OLN_TOOLS],dnl ... and a collection.
  [dnl 

### The utilities tree is both a component...
    OLN_COMPONENT([tools/utilities], 
	          [utilities], 
	          [oln_cv_build_utilities], 
	          [the command-line utilities], 
	          [OLN_TOOLS], 
                  [tools/utilities/Makefile],
    [OLN_COLLECTION([OLN_UTILITIES], dnl ... and a collection.
    [dnl

### The morpho utilities are a component.
      OLN_COMPONENT([tools/utilities/morpho], 
	            [morpho-utilities], 
	            [oln_cv_build_morpho_utils], 
	            [the morpho utilities], 
	            [OLN_UTILITIES], 
	            [tools/utilities/morpho/Makefile], 
                    [], [morpho])

    ])dnl End of OLN_COLLECTION([OLN_UTILITIES])
    ], [utilities])dnl End of OLN_COMPONENT([tools/utilities])

   ])dnl End of OLN_COLLECTION([OLN_TOOLS])
   ])dnl End of OLN_COMPONENT([tools])

])dnl End of OLN_COLLECTION([OLN_TOP])


AC_OUTPUT


