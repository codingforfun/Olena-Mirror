AC_PREREQ([2.54])

source oln-version

AC_INIT([$OLN_SWIG_PNAME], [$OLN_SWIG_VERSION], [$OLN_SWIG_CONTACT])
AC_CONFIG_SRCDIR(oln-version)
AM_INIT_AUTOMAKE([1.6 no-define foreign dist-bzip2])
AM_MAINTAINER_MODE

AC_CONFIG_HEADERS([config.h:config.hin])
AC_CONFIG_FILES([Makefile])

oln_dir=no
if test -r $srcdir/../olena/oln/config/pconf.hh; then
  oln_dir=..
fi
AM_PATH_OLN([$oln_dir])
AC_WITH_OLN

# Utilities may use FFTW and ZLIB
AC_WITH_ZLIB
AC_WITH_FFTW

AC_DEFINE_UNQUOTED([CXX], ["$CXX"], [The C++ compiler])

AM_CONFIG_HEADER(config.h:config.hin)
AC_CONFIG_FILES([Makefile])
dnl         Makefile
dnl 	doc/Makefile
dnl         src/Makefile
dnl 	python/Makefile)

AC_OUTPUT

