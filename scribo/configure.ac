# Copyright (C) 2006, 2007, 2008, 2009, 2010 EPITA Research and
# Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

m4_pattern_forbid([^OLN_])

AC_PREREQ([2.61])


## ---------------- ##
## Package set up.  ##
## ---------------- ##

AC_INIT([Olena-Scribo], [1.0a], [olena@lrde.epita.fr], [olena-scribo])

# M4 macros.
AC_CONFIG_MACRO_DIR([m4])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])
### AC_CONFIG_FILES([build-aux/Makefile])

# Automake.
AM_INIT_AUTOMAKE([1.10 subdir-objects check-news dist-bzip2 nostdinc -Wall])

## --------------------- ##
## C++ compiler set up.  ##
## --------------------- ##

# If the user doesn't provide any CXXFLAGS, prevent Autoconf from
# settings its own default ones (e.g., `-g -O2' for g++).
if test ! ${CXXFLAGS+set}; then
   CXXFLAGS=""
fi

# Look for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

# Set `ICPC' to `yes' if the Intel C++ compiler is used.
test $CXX --version 2>/dev/null | grep '\bICC\b' >/dev/null 2>&1 && ICPC=yes

# GNU C++ compiler setup.
if test "$GXX" = yes; then
  # Speed up compiling times.
  CXXFLAGS="$CXXFLAGS -pipe"

  # The code generated for mln::data::impl::memcpy__() by g++ 4.2 with
  # a high optimization level (`-O3') and without
  # `-fno-strict-aliasing' might be wrong, at least with Debian's g++
  # 4.2 on IA-32 (see also milena/mln/memcpy_.hh).  We observed this
  # behavior with e.g. milena/apps/graph-morpho/samples-image2d.cc.
  # Note that Debian's g++ 4.0, 4.1, 4.3 and 4.4 are fine.
  #
  # So, when the C++ compiler is g++ 4.2, set STRICT_ALIASING_CXXFLAGS
  # to `-fno-strict-aliasing'.
  if $CXX --version | head -n 1 | grep '\b4\.2' >/dev/null 2>&1; then
    STRICT_ALIASING_CXXFLAGS=-fno-strict-aliasing
  fi
fi
AC_SUBST([STRICT_ALIASING_CXXFLAGS])


# Adjusting warning options according to compilers.
AC_ARG_VAR([WARNINGS_CXXFLAGS], [C++ compiler warning flags])
case "$CXX" in
     # Intel compiler
     *icpc*)
        WARNINGS_CXXFLAGS="-Wall -wd111,193,279,383,444,522,654,810,981,1418"
        ;;
     *)
        WARNINGS_CXXFLAGS="-Wall -W"
        ;;
esac


## -------- ##
## Milena.  ##
## -------- ##

# The current approach to providing Scribo a path to Milena's headers
# is naive: for instance, we cannot provide two paths (source and
# build dirs) in the case of an Olena build using a parallel build
# (where srcdir != builddir).  Improve if needed.

AC_ARG_WITH([milena],
  [AC_HELP_STRING([--with-milena=DIR], [path to Milena headers])],
  [if test x"$with_milena" = xno; then
     AC_MSG_ERROR([Milena is required for Scribo.  Try to invoke configure
without `--with-milena=no' nor `--without-milena.'])
   fi])
if test -n "$with_milena" && test x"$with_milena" != xyes; then
  MILENA_CPPFLAGS="-I$with_milena"
fi
swl_save_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$MILENA_CPPFLAGS $CPPFLAGS"
AC_CHECK_HEADER([mln/core/essential.hh], [],
  AC_MSG_ERROR([Cannot find Milena's headers.  Try to invoke configure with
`--with-milena=DIR'.]))
CPPFLAGS=$swl_save_CPPFLAGS
AC_SUBST([MILENA_CPPFLAGS])


## ------------------ ##
## Libraries set up.  ##
## ------------------ ##

# Use Libtool.
# To be replaced by a call to LT_INIT as soon as Libtool 2.2 is used.
AC_PROG_LIBTOOL


## -------------------- ##
## External libraries.  ##
## -------------------- ##

# ---------- #
# Magick++.  #
# ---------- #

OLN_WITH_LIB_PKG_CONFIG([Magick++], [Magick++.h], [Magick++], [magickxx],
                        [MAGICKXX], [], [ImageMagick++])

# ---- #
# Qt.  #
# ---- #

AT_WITH_QT([xml], [], [], [:], [oln_have_qt=yes])
if test x$oln_have_qt = xyes; then
  AT_REQUIRE_QT_VERSION([4],
    AC_MSG_WARN([Qt-dependent programs will be disabled.]),
    oln_have_expected_qt_version=yes)
fi
AM_CONDITIONAL([HAVE_QT], [test "x$oln_have_expected_qt_version" = xyes])

# ----------- #
# Tesseract.  #
# ----------- #

OLN_WITH_LIB([TESSERACT], [tesseract/baseapi.h], [tesseract_full],
             [tesseract], [TESSERACT])

# -------------- #
# TIFF library.  #
# -------------- #

OLN_WITH_LIB([TIFF], [tiff.h], [tiff])


dnl<<lrde
## -------- ##
## Scribo.  ##
## -------- ##

AC_ARG_ENABLE([scribo],
              [AS_HELP_STRING([--enable-scribo],
                              [enable Scribo])])
AM_CONDITIONAL([ENABLE_SCRIBO], [test "x$enable_scribo" = xyes])

AC_CONFIG_FILES([
  Makefile
    build-aux/Makefile
    demo/Makefile
      demo/review/Makefile
      demo/viewer/Makefile
    src/Makefile
      src/binarization/Makefile
      src/contest/Makefile
      src/contest/hdibco-2010/Makefile
      src/debug/Makefile
      src/filter/Makefile
      src/misc/Makefile
      src/postprocessing/Makefile
      src/preprocessing/Makefile
      src/primitive/Makefile
        src/primitive/extract/Makefile
        src/primitive/find/Makefile
        src/primitive/group/Makefile
      src/table/Makefile
      src/text/Makefile
])

AC_CONFIG_FILES([tests/data.hh])

AC_CONFIG_FILES([
  tests/Makefile
    tests/binarization/Makefile
    tests/core/Makefile
    tests/filter/Makefile
    tests/preprocessing/Makefile
    tests/table/Makefile
    tests/text/Makefile
    tests/toolchain/Makefile
    tests/toolchain/nepomuk/Makefile
    tests/unit_test/Makefile
])

# Flags for Scribo
AC_ARG_VAR([SCRIBO_CXXFLAGS], [C++ compiler flags for Scribo binaries])
AC_ARG_VAR([SCRIBO_TESTS_CXXFLAGS], [C++ compiler flags for Scribo tests])
if test "$GXX" = yes || test "$ICPC" = yes; then
  test -z "$SCRIBO_CXXFLAGS" && SCRIBO_CXXFLAGS="-DNDEBUG -O2"
  # FIXME: We might want to use -ggdb when g++ is used.
  test -z "$SCRIBO_TESTS_CXXFLAGS" && SCRIBO_TESTS_CXXFLAGS="-g -O0"
fi
dnl>>

## ----------------- ##
## Verbose display.  ##
## ----------------- ##

# Do not print this information by default, since `configure' outputs
# should be kept short.  Longer explanations are always available in
# config.log.

AC_ARG_ENABLE([verbose],
  [AS_HELP_STRING([--enable-verbose], [enable verbose display])],
  [if test "x$enable_verbose" != xno; then
     AC_MSG_RESULT([
-------------------------------------------------------------------------------
Configuration summary.


Host system type: $host
Build system type: $build

================
| Dependencies |
================


                        Option                  Enabled and available
-------------------------------------------------------------------------------
Magick++                --with-magickxx         $oln_have_magickxx
Qt                      --with-qt               $oln_have_qt
Tesseract               --with-tesseract@<:@=DIR@:>@  $oln_have_tesseract
TIFF                    --with-tiff@<:@=DIR@:>@       $oln_have_tiff
-------------------------------------------------------------------------------




Options used to compile and link:
  PREFIX          = $PREFIX_DIR
  EXEC-PREFIX     = $EXEC_PREFIX_DIR
  VERSION         = $PACKAGE_VERSION
  CC              = $CC
  CFLAGS          = $CFLAGS
  CPPFLAGS        = $MAGICK_CPPFLAGS
  DEFS            = $DEFS
  LDFLAGS         = $LDFLAGS
  LIBS            = $MAGICK_LIBS
  CXX             = $CXX
  CXXFLAGS        = $CXXFLAGS
  PKG_CONFIG      = $PKG_CONFIG
  QT_PATH         = $QT_PATH
  QMAKE           = $QMAKE
  MOC             = $MOC
  UIC             = $UIC
  RCC             = $RCC
  MAGICKXX_CFLAGS = $MAGICKXX_CPPFLAGS
  MAGICKXX_LIBS   = $MAGICKXX_LDFLAGS
  SCRIBO_CXXFLAGS = $SCRIBO_CXXFLAGS



*******************************************************************************
Scribo is configured as stated above.  Please verify that this configuration
matches your expectations.

Then, type 'make' to build Olena and 'make install' to install it on
your system.
])
fi])


## -------- ##
## Output.  ##
## -------- ##

AC_OUTPUT
