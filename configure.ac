# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.53])

AC_INIT([oln], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_AUX_DIR([oln/config])
AC_CONFIG_SRCDIR([oln/config/oln.m4])
AM_INIT_AUTOMAKE([1.6 no-define foreign dist-bzip2])
AC_CONFIG_HEADERS([config.h:config.hin])

# Use included Olena version.
OLN_PATH_LOCAL([oln])
OLN_PATH_LOCAL_IMGS([oln/img])
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

### The testsuite is a component.

    OLN_COMPONENT([oln/tests], [testsuite], [oln_cv_build_tests], [the testsuite], [OLN_SRC], 
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

  OLN_COMPONENT([doc], [doc], [oln_cv_build_doc], [the documentation], [OLN_TOP],
	        [doc/Makefile],
  [OLN_COLLECTION([OLN_DOC], dnl ... and a collection.
  [dnl

### The manuals directory is a component.

    OLN_COMPONENT([doc/doc], [manuals], [oln_cv_build_ref], [the Olena manuals], [OLN_DOC], 
		  [doc/doc/Makefile],
    [dnl Configuration for the manuals directory.

     # DOC_CHECK_PROG defined in oln-local.m4
     DOC_CHECK_PROG([DVIPS], [dvips])
     DOC_CHECK_PROG([TEXI2DVI], [texi2dvi])
     DOC_CHECK_PROG([GETDEFS], [getdefs])
     DOC_CHECK_PROG([HEVEA], [hevea])
     DOC_CHECK_PROG([IMAGEMAGICK_CONVERT], [convert])
     DOC_CHECK_PROG([EPSTOPDF], [epstopdf])

    ], [doc])dnl End of OLN_COMPONENT([doc/doc])

### The demonstration programs are a component.

    OLN_COMPONENT([doc/demo], [demo], [oln_cv_build_demo], [the demonstration programs], [OLN_DOC], 
	          [doc/demo/Makefile],
                  [], [demo])

  ])dnl End of OLN_COLLECTION([OLN_DOC])
  ])dnl End of OLN_COMPONENT([OLN_DOC])

###
### Stuff pertaining to the tools
###

### The tools tree is both a component...
  OLN_COMPONENT([tools], [tools], [oln_cv_build_tools], [the run-time tools], [OLN_TOP], 
                [tools/Makefile tools/lib/Makefile],
  [OLN_COLLECTION([OLN_TOOLS],dnl ... and a collection.
  [dnl 

### The utilities tree is both a component...
    OLN_COMPONENT([tools/utilities], [utilities], [oln_cv_build_utilities], [the command-line utilities], [OLN_TOOLS], 
                  [tools/utilities/Makefile],
    [OLN_COLLECTION([OLN_UTILITIES], dnl ... and a collection.
    [dnl

### The morpho utilities are a component.
      OLN_COMPONENT([tools/utilities/morpho], [morpho-utilities], [oln_cv_build_morpho_utils], [the morpho utilities], [OLN_UTILITIES], 
	            [tools/utilities/morpho/Makefile], 
                    [], [morpho])

    ])dnl End of OLN_COLLECTION([OLN_UTILITIES])
    ], [utilities])dnl End of OLN_COMPONENT([tools/utilities])

   ])dnl End of OLN_COLLECTION([OLN_TOOLS])
   ])dnl End of OLN_COMPONENT([tools])

])dnl End of OLN_COLLECTION([OLN_TOP])


AC_OUTPUT


