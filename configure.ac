# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

AC_INIT([Olena], OLN_VERSION, OLN_CONTACT, [olena])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([config/oln.m4])
AM_INIT_AUTOMAKE([1.9.5 no-define foreign dist-bzip2])
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

# Use Libtool to build Swilena's dynamic modules.
AC_PROG_LIBTOOL

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
		 olena/img/Makefile
		 olena/oln/Makefile
		 olena/oln/config/pconf.hh:olena/oln/config/pconf-hh.in])

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
     AC_ARG_VAR([PNMDEPTH], [program to change depth of Portable Anymaps])
     AC_CHECK_PROGS([PNMDEPTH], [pnmdepth], [$am_aux_dir/missing pnmdepth])
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
                                                  [none,all,convert,convol,io,morpho,sanity,transforms])],
                                  [oln_cv_tests_selection=$withval],
                                  [oln_cv_tests_selection=all])dnl
                     ])dnl


      # OLN_TESTS defined in oln-local.m4
      OLN_TESTS([arith])
      OLN_TESTS([convert])
      OLN_TESTS([convol])
      OLN_TESTS([core])
      OLN_TESTS([io])
      OLN_TESTS([math])
      OLN_TESTS([morpho])
      OLN_TESTS([level])
      OLN_TESTS([sanity])
      OLN_TESTS([topo])
      OLN_TESTS([transforms])
      OLN_TESTS([utils])
      OLN_TESTS([morpher])

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
                                                  [none,types])],
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
                                                  [none,array,main])],
                                  [mlc_cv_tests_selection=$withval],
                                  [mlc_cv_tests_selection=all])dnl
                     ])dnl


      # MLC_TESTS defined in oln-local.m4
      MLC_TESTS([array])
      MLC_TESTS([main])
      MLC_TESTS([sanity])

      AC_SUBST([MLC_TESTS_SUBDIRS])

    ], [tests])dnl End of OLN_COMPONENT([metalic/tests])

  ])dnl End of OLN_COLLECTION([MLC_SRC])

###
### Stuff pertaining to the documentation
###

### The reference manual uses Doxygen
### FIXME: could be much better.

  AC_ARG_WITH([doc],
 	      [Doxygen is needed to build the doc],
	      [],
	      [AC_CHECK_PROG([DOXYGEN], [doxygen], [doxygen])
	       AC_CHECK_DOXYGEN_VERSION
	     ])

## if test -z "$DOXYGEN"; then
##   AC_MSG_WARN([`doxygen' not found -- `doxygen' must be installed to generate the documentation])

## fi

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
		  [doc/ref/Makefile
                   doc/ref/doxygen.config
                   doc/ref/out/exdoc.config
		   doc/ref/exdoc.mk],
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
     AC_ARG_VAR([HACHA], [program to make split HTML pages from TeX])
     AC_CHECK_PROGS([HACHA], [hacha], [$am_aux_dir/missing hacha])
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

### The convert utilities are a component.
      OLN_COMPONENT([tools/utilities/convert],
	            [convert-utilities],
	            [oln_cv_build_convert_utils],
	            [the convert utilities],
	            [OLN_UTILITIES],
	            [tools/utilities/convert/Makefile],
                    [], [convert])

    ])dnl End of OLN_COLLECTION([OLN_UTILITIES])
    ], [utilities])dnl End of OLN_COMPONENT([tools/utilities])

### Swilena is a component...
    OLN_COMPONENT([tools/swilena],
	          [swilena],
                  [oln_cv_build_swilena],
	          [the SWIG interface to Olena],
                  [OLN_TOOLS],
                  [tools/swilena/Makefile
	           tools/swilena/meta/Makefile
	           tools/swilena/src/Makefile
	           tools/swilena/doc/Makefile],
    [OLN_COLLECTION([OLN_SWILENA], dnl ... and a collection.
    [dnl

      AC_CHECK_SWIG13 dnl definition in oln-local.m4

     # We cannot enable swilena without swig because we need the dynamic
     # library (swig{py,rb}).
     if test "x$oln_cv_recent_swig" != "xyes"; then
       AC_MSG_ERROR([You need swig >= 1.3 to compile swilena. You can disable swilena by specifying --without-swilena to configure.])
     fi

      AC_CHECK_SWIG_FLAGS

      AC_ARG_VAR([DVIPS], [program to make Postscript from TeX DVI files])
      AC_CHECK_PROGS([DVIPS], [dvips], [$am_aux_dir/missing dvips])
      AC_ARG_VAR([TEXI2DVI], [program to create DVI from Texinfo files])
      AC_CHECK_PROGS([TEXI2DVI], [texi2dvi], [$am_aux_dir/missing texi2dvi])

      OLN_COMPONENT([tools/swilena/python],
                    [pylena],
                    [oln_cv_build_pylena],
                    [the Python interface to Olena],
                    [OLN_SWILENA],
                    [tools/swilena/python/Makefile
		     tools/swilena/python/tests/Makefile],
      [
         AM_PATH_PYTHON([2.1])
	 save_CPPFLAGS=$CPPFLAGS

	 AC_ARG_VAR([PYTHON_CPPFLAGS], [Path to Python.h])
	 AC_ARG_WITH([python-includedir], [Include path for Python.h],
	             [PYTHON_CPPFLAGS="-I$withval"],
                     [PYTHON_CPPFLAGS="-I"`echo "import sys; print sys.prefix" | python`"/include/python$PYTHON_VERSION"])

	 CPPFLAGS="$CPPFLAGS $PYTHON_CPPFLAGS"
         AC_CHECK_HEADERS([Python.h],,
	                  AC_MSG_ERROR(
			  [You need Python development files to compile the Python interface. Specify --with-python-includedir to set it manually or --without-pylena to disable it.]))

	 CPPFLAGS=$save_CPPFLAGS
      ], [python])dnl End of OLN_COMPONENT([tools/swilena/python])

      OLN_COMPONENT([tools/swilena/ruby],
                    [rblena],
                    [oln_cv_build_rblena],
                    [the Ruby interface to Olena],
                    [OLN_SWILENA],
                    [tools/swilena/ruby/Makefile
		     tools/swilena/ruby/tests/Makefile],
      [
         AC_CHECK_PROGS([RUBY], [ruby], [$am_aux_dir/missing ruby])
	 if test "x$RUBY" = "x$am_aux_dir/missing ruby"; then
	   AC_MSG_ERROR([You need ruby to compile the ruby interface. Specify --without-rblena to disable the ruby interface.])
         fi

	 AC_ARG_VAR([RUBY_CPPFLAGS], [Path to ruby.h])
	 AC_ARG_WITH([ruby-includedir], [Include path for ruby.h],
	             [RUBY_CPPFLAGS="-I$withval"],
                     [RUBY_CPPFLAGS="-I`ruby -e 'puts $:.join("\n")' | while read line; do
    				if [[ -f ${line}/ruby.h ]]; then
					echo $line
    				fi
			done`"])
	 save_CPPFLAGS=$CPPFLAGS
	 CPPFLAGS="$CPPFLAGS $RUBY_CPPFLAGS"
         AC_CHECK_HEADERS([ruby.h],,
	                  AC_MSG_ERROR(
			  [You need Ruby development files to compile the Ruby interface. Specify --with-ruby-includedir to set it manually or --without-rblena to disable it.]))

	 CPPFLAGS=$save_CPPFLAGS
      ], [ruby])dnl End of OLN_COMPONENT([tools/swilena/ruby])

    ])dnl End of OLN_COLLECTION([OLN_SWILENA])
    ], [swilena])dnl End of OLN_COMPONENT([tools/swilena])

   ])dnl End of OLN_COLLECTION([OLN_TOOLS])
   ])dnl End of OLN_COMPONENT([tools])

   AM_CONDITIONAL([HAVE_DOC_DEV], [test "x$oln_cv_build_doc" != xno -a "x$oln_cv_build_devdoc" != xno])

])dnl End of OLN_COLLECTION([OLN_TOP])

AC_SUBST([DOC_CPPFLAGS])
AC_SUBST([TOOLS_LDFLAGS])

## -- FIXME: Improve integration.  ----------------------------------------
##
## The contents of `olena/lrde' should be moved to subdirectory of
## `olena/oln', and the contents of `olena/lrde/ufmt/bin to `olena/tests'
## or to `tools'.
AC_CONFIG_FILES([olena/oln/lrde/Makefile
		 olena/oln/lrde/efigi/Makefile
		 olena/oln/lrde/ufmt/Makefile
		 olena/oln/lrde/ufmt/bin/Makefile])
## ---------------------------------------- FIXME: Improve integration.  --

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
