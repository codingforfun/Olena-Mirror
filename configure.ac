# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

AC_INIT([Olena], OLN_VERSION, OLN_CONTACT, [oln])
AC_CONFIG_AUX_DIR([oln/config])
AC_CONFIG_SRCDIR([oln/config/oln.m4])
AM_INIT_AUTOMAKE([1.7 no-define foreign dist-bzip2])
AC_CONFIG_HEADERS([config.h:config.hin])

# Use included Olena version.
OLN_PATH_LOCAL([oln])
AC_WITH_OLN
# Import local version of Metalic.
CPPFLAGS="-I\$(top_srcdir)/metalic $CPPFLAGS"

# Some library archives are used. Some C code used.
AC_PROG_RANLIB
AC_PROG_CC

# If available, use these.
AC_WITH_CXX_ZLIB
AC_WITH_CXX_FFTW

# Attempt to use strict language conformance checking.
AC_CXX_FLAGS

AC_CONFIG_FILES([Makefile
                 lib/Makefile
                 lib/check/defs
                 lib/check/Makefile
                 metalic/Makefile 
                 metalic/tests/Makefile
                 metalic/tests/array/Makefile
                 oln/Makefile 
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

     AC_ARG_VAR([GZIP], [the gzip program])
     AC_CHECK_PROGS([GZIP], [gzip], [$am_aux_dir/missing gzip])
     AC_ARG_VAR([PNGTOPNM], [program to convert Portable Network Graphics files to Portable Anymaps])
     AC_CHECK_PROGS([PNGTOPNM], [pngtopnm], [$am_aux_dir/missing pngtopnm])
     AC_ARG_VAR([PPMTOPGM], [program to convert Portable Pixmaps to Portable Greymaps])
     AC_CHECK_PROGS([PPMTOPGM], [ppmtopgm], [$am_aux_dir/missing ppmtopgm])
     AC_ARG_VAR([PGMTOPBM], [program to convert Portable Greymaps to Portable Bitmaps])
     AC_CHECK_PROGS([PGMTOPBM], [pgmtopbm], [$am_aux_dir/missing pgmtopbm])
     AC_ARG_VAR([PNMTOPLAINPNM], [program to convert Portable Anymaps to plain text])
     AC_CHECK_PROGS([PNMTOPLAINPNM], [pnmtoplainpnm], [$am_aux_dir/missing pnmtoplainpnm])
     AC_ARG_VAR([IMAGEMAGICK_CONVERT], [the `convert' utility from ImageMagick])
     AC_CHECK_PROGS([IMAGEMAGICK_CONVERT], [convert], [$am_aux_dir/missing convert])

### Checks pertaining to the generation of man pages

     AC_ARG_VAR([HELP2MAN], [utility to make man pages from program options])
     AC_CHECK_PROGS([HELP2MAN], [help2man], [$am_aux_dir/missing help2man])
     AC_CACHE_CHECK([whether to re-generate manual pages from programs],
                    [oln_cv_regen_manpages],
                    [oln_cv_regen_manpages=yes
                     AC_ARG_ENABLE([manpages-regen],
                                   [AC_HELP_STRING([--disable-manpages-regen],
                                                   [force disable regeneration of manual pages])],
                                   [oln_cv_regen_manpages=$enableval])
                    ])
                     

     AM_CONDITIONAL([REGEN_MANPAGES], [test "x$oln_cv_regen_manpages" != xno])

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
	          [oln/tests/Makefile],
    [dnl Configuration for the testsuite

      OLN_TESTS_SUBDIRS=""

      AC_CACHE_CHECK([for selection of tests],
                     [oln_cv_tests_selection],
                     [AC_ARG_WITH([tests],
    	                          [AC_HELP_STRING([--with-tests=<list>],
                                                  [none,all,convert,convol,io,morpho,sanity,transforms,types])],
                                  [oln_cv_tests_selection=$withval],
                                  [oln_cv_tests_selection=all])dnl
                     ])dnl
      

      # OLN_TESTS defined in oln-local.m4
      OLN_TESTS([arith])
      OLN_TESTS([convert])
      OLN_TESTS([convol])
      OLN_TESTS([io])
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
     AC_ARG_VAR([EPSTOPDF], [program to convert Encapsulated Postscript images to PDF])
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

     AC_ARG_VAR([DVIPS], [program to make Postscript from TeX DVI files])
     AC_CHECK_PROGS([DVIPS], [dvips], [$am_aux_dir/missing dvips])
     AC_ARG_VAR([TEXI2DVI], [program to create DVI from Texinfo files])
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

### Swilena is a component...
    OLN_COMPONENT([tools/swilena],
	          [swilena],
                  [oln_cv_build_swilena],
	          [the SWIG interface to Olena],
                  [OLN_TOOLS],
                  [tools/swilena/Makefile tools/swilena/src/Makefile tools/swilena/doc/Makefile],
    [OLN_COLLECTION([OLN_SWILENA], dnl ... and a collection.
    [dnl
       
      AC_CHECK_SWIG13 dnl definition in oln-local.m4
 
      AC_ARG_VAR([DVIPS], [program to make Postscript from TeX DVI files])
      AC_CHECK_PROGS([DVIPS], [dvips], [$am_aux_dir/missing dvips])
      AC_ARG_VAR([TEXI2DVI], [program to create DVI from Texinfo files])
      AC_CHECK_PROGS([TEXI2DVI], [texi2dvi], [$am_aux_dir/missing texi2dvi])

      OLN_COMPONENT([tools/swilena/python],
                    [pylena],
                    [oln_cv_build_pylena],
                    [the Python interface to Olena],
                    [OLN_SWILENA],
                    [tools/swilena/python/Makefile],
      [
         AM_PATH_PYTHON([2.1])
      ], [python])dnl End of OLN_COMPONENT([tools/swilena/python])
        

    ])dnl End of OLN_COLLECTION([OLN_SWILENA])
    ], [swilena])dnl End of OLN_COMPONENT([tools/swilena])

   ])dnl End of OLN_COLLECTION([OLN_TOOLS])
   ])dnl End of OLN_COMPONENT([tools])

   AM_CONDITIONAL([HAVE_DOC_DEV], [test "x$oln_cv_build_doc" != xno -a "x$oln_cv_build_devdoc" != xno])

])dnl End of OLN_COLLECTION([OLN_TOP])


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

