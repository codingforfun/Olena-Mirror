AC_PREREQ([2.54])

AC_INIT([olena], [0.5], [olena-bugs@lrde.epita.fr])

AC_CONFIG_SRCDIR([../ChangeLog])
AC_CONFIG_AUX_DIR([../tools])
AM_INIT_AUTOMAKE([1.7.1 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

AC_PROG_CXX
AC_PROG_RANLIB

###### utilities check #######
AC_ARG_ENABLE(utilities,
  [AC_HELP_STRING([--disable-utilities], [disable utilities compilation])])

UTILITIES=''
if test "x$enable_utilities" != xno; then
  UTILITIES=utilities
fi
AC_SUBST([UTILITIES])
##############################

####### morpho utilities check ######
AC_ARG_ENABLE(morpho_utilities,
  [AC_HELP_STRING([--disable-morpho-utilities], [disable morpho utilities compilation])])


MORPHO_UTILITIES=''
if test "x$enable_morpho_utilities" != xno; then
  MORPHO_UTILITIES=morpho
fi
AC_SUBST([MORPHO_UTILITIES])
######################################


AM_PATH_OLN([..])
AC_WITH_OLN

# Tests and other sources may use FFTW and ZLIB
AC_WITH_ZLIB
AC_WITH_FFTW


AC_CONFIG_FILES([
	Makefile
        tests/defs
        tests/Makefile
        tests/check/Makefile
        tests/convert/Makefile
        tests/convol/Makefile
        tests/io/Makefile
        tests/meta/Makefile
        tests/morpho/Makefile
        tests/sanity/Makefile
        tests/transforms/Makefile
        tests/types/Makefile
	demo/Makefile
        utilities/Makefile
        utilities/morpho/Makefile
	])

AC_OUTPUT