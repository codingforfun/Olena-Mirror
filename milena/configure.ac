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

AC_INIT([Milena], [1.0a], [olena@lrde.epita.fr], [milena])

# M4 macros.
AC_CONFIG_MACRO_DIR([m4])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([build-aux/Makefile])

# Automake.
AM_INIT_AUTOMAKE([1.10 subdir-objects check-news dist-bzip2 nostdinc -Wall])

# Package metadata.
AC_CONFIG_FILES([mln/version.hh])
AC_DEFINE_UNQUOTED([OLN_PACKAGE_NAME], ["$PACKAGE_NAME"],
                   [Package Full name.])
AC_DEFINE_UNQUOTED([OLN_PACKAGE_BUGREPORT], ["$PACKAGE_BUGREPORT"],
                   [Bug report address.])
AC_DEFINE_UNQUOTED([OLN_PACKAGE_STRING], ["$PACKAGE_STRING"],
                   [Full name and version.])
AC_DEFINE_UNQUOTED([OLN_PACKAGE_VERSION], ["$PACKAGE_VERSION"],
                   [Package Version.])

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
  # 4.2 on IA-32 (see also mln/memcpy_.hh).  We observed this
  # behavior with e.g. apps/graph-morpho/samples-image2d.cc.
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


# ------------------------------ #
# C++ compiler flags for tests.  #
# ------------------------------ #

# FIXME: We might want to write an Autoconf macro to factor this.

# Standard flags for tests.
AC_ARG_VAR([TESTS_CXXFLAGS], [C++ compiler flags for tests])
# We want no optimization for the tests (it slows down compiling
# times), and debugging information.
if test -z "$TESTS_CXXFLAGS"; then
  if test "$GXX" = yes; then
    # GNU C++ compiler setup.
    TESTS_CXXFLAGS="-O0 -ggdb $WARNINGS_CXXFLAGS"
  elif test "$ICPC" = yes; then
    # Intel C++ compiler setup.
    TESTS_CXXFLAGS="-O0 -g $WARNINGS_CXXFLAGS"
  fi
fi

# Flags for complex tests.
AC_ARG_VAR([TESTS_CXXFLAGS_SPEED],
           [C++ compiler optimization flags for (complex) tests])
# We want optimization for complex tests, and keep debugging flags
# (still useful).
if test -z "$TESTS_CXXFLAGS_SPEED"; then
  if test "$GXX" = yes; then
    # GNU C++ compiler setup.
    TESTS_CXXFLAGS_SPEED="-O3 -DNDEBUG -ggdb $WARNINGS_CXXFLAGS"
  elif test "$ICPC" = yes; then
    # Intel C++ compiler setup.
    TESTS_CXXFLAGS_SPEED="-O3 -DNDEBUG -g $WARNINGS_CXXFLAGS"
  fi
fi

# Flags for tests with with all debugging features turned on.
AC_ARG_VAR([TESTS_CXXFLAGS_DEBUG], [C++ compiler debug flags])
# We want no optimization for the tests (it slows down compiling
# times), and a lot of debugging features.
# * GNU C++ Library Debug Mode:
#   http://gcc.gnu.org/onlinedocs/libstdc++/manual/debug_mode.html
# * GNU C++ Library Compile Time Checks (a.k.a. concept checking):
#   http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt12ch29.html
if test -z "$TESTS_CXXFLAGS_DEBUG"; then
  if test "$GXX" = yes; then
    TESTS_CXXFLAGS_DEBUG="-O0 -ggdb $WARNINGS_CXXFLAGS -D_GLIBCXX_DEBUG -D_GLIBCXX_CONCEPT_CHECKS"
  elif test "$ICPC" = yes; then
    TESTS_CXXFLAGS_DEBUG="-O0 -g $WARNINGS_CXXFLAGS -D_GLIBCXX_DEBUG -D_GLIBCXX_CONCEPT_CHECKS"
  fi
fi


## ------------------------------ ##
## ``Enable Everything'' Switch.  ##
## ------------------------------ ##

# Enable all bundled features (trimesh2, apps, tools).  This
# option is useful to maintainers to ensure they do not break optional
# parts while modifying the core of the project. If both
# ``--enable-all'' is set and a given feature is disabled (say,
# ``--disable-apps''), then the disable flag has priority.

AC_ARG_ENABLE([all],
  [AS_HELP_STRING([--enable-all], [enable maintainer mode])],
  [if test "x$enable_all" != xno; then
     enable_trimesh=yes
     enable_apps=yes
     enable_tools=yes
   fi])


## ------------------ ##
## Libraries set up.  ##
## ------------------ ##

# Use Libtool.
# To be replaced by a call to LT_INIT as soon as Libtool 2.2 is used.
AC_PROG_LIBTOOL

# Check for Darwin.
AC_CANONICAL_HOST
AM_CONDITIONAL([DARWIN], [echo "$host_os" | grep '^darwin'])


## -------------------- ##
## External libraries.  ##
## -------------------- ##

# ------- #
# Boost.  #
# ------- #

BOOST_TUPLE
AM_CONDITIONAL([HAVE_BOOST_TUPLE],
               [test "x$ac_cv_header_boost_tuple_tuple_hpp" = xyes])

# ----------------- #
# CFITSIO library.  #
# ----------------- #

OLN_WITH_LIB([CFITSIO], [fitsio.h], [cfitsio])

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

# -------------- #
# TIFF library.  #
# -------------- #

OLN_WITH_LIB([TIFF], [tiff.h], [tiff])

# --------------------------------- #
# GDCM library (Grassroots DiCom).  #
# --------------------------------- #

OLN_WITH_LIB([GDCM], [gdcm-2.0/gdcmReader.h], [gdcmCommon], [gdcm],
             [GDCM],
             m4_do([-lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF],
                   [ -lgdcmexpat -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8],
                   [ -lgdcmopenjpeg -lgdcmuuid -lgdcmzlib]))


## ---------------------------- ##
## External, bundled projects.  ##
## ---------------------------- ##

AC_CONFIG_FILES([external/Makefile])

# ------------------ ##
# Trimesh2 library.  ##
# ------------------ ##

AC_ARG_ENABLE([trimesh],
              [AS_HELP_STRING([--enable-trimesh],
                              [build the (bundled) trimesh2 library])])
AM_CONDITIONAL([ENABLE_TRIMESH], [test "x$enable_trimesh" = xyes])
AC_CONFIG_SUBDIRS([external/trimesh])


## --------------- ##
## Configuration.  ##
## --------------- ##

# Ask for config.h creation.
AC_CONFIG_HEADERS([config.h])

# Ask for the creation of a doc/tool/data.hh, used to access to
# data (images) from the documentation.
AC_CONFIG_FILES([doc/tools/data.hh])

# Ask for the Makefile creations.
AC_CONFIG_FILES([
  Makefile
    doc/Makefile
    mesh/Makefile
])


## ------- ##
## Tests.  ##
## ------- ##

# Ask for the creation of a tests/data.hh, used to access to
# data (images) from tests.
AC_CONFIG_FILES([tests/data.hh])

# Ask for the creation of tests' Makefiles.
AC_CONFIG_FILES([
  tests/Makefile
    tests/accu/Makefile
    tests/accu/image/Makefile
    tests/accu/site_set/Makefile
    tests/accu/stat/Makefile
    tests/accu/math/Makefile
    tests/accu/shape/Makefile
    tests/algebra/Makefile
    tests/arith/Makefile
    tests/binarization/Makefile
    tests/border/Makefile
    tests/canvas/Makefile
      tests/canvas/browsing/Makefile
      tests/canvas/morpho/Makefile
    tests/convert/Makefile
    tests/convert/impl/Makefile
    tests/core/Makefile
      tests/core/alias/Makefile
      tests/core/image/Makefile
        tests/core/image/dmorph/Makefile
        tests/core/image/imorph/Makefile
        tests/core/image/vmorph/Makefile
      tests/core/other/Makefile
      tests/core/routine/Makefile
      tests/core/site_set/Makefile
    tests/data/Makefile
      tests/data/approx/Makefile
      tests/data/naive/Makefile
    tests/debug/Makefile
    tests/display/Makefile
    tests/draw/Makefile
    tests/estim/Makefile
    tests/extension/Makefile
    tests/fun/Makefile
      tests/fun/i2v/Makefile
      tests/fun/p2b/Makefile
      tests/fun/p2p/Makefile
      tests/fun/p2v/Makefile
      tests/fun/stat/Makefile
      tests/fun/v2v/Makefile
      tests/fun/vv2v/Makefile
      tests/fun/x2x/Makefile
    tests/geom/Makefile
    tests/graph/Makefile
      tests/graph/attribute/Makefile
    tests/histo/Makefile
    tests/io/Makefile
      tests/io/dicom/Makefile
      tests/io/dump/Makefile
      tests/io/fits/Makefile
      tests/io/fld/Makefile
      tests/io/magick/Makefile
      tests/io/off/Makefile
      tests/io/pbm/Makefile
      tests/io/pbms/Makefile
      tests/io/pfm/Makefile
      tests/io/pgm/Makefile
      tests/io/pgms/Makefile
      tests/io/pnm/Makefile
      tests/io/ppm/Makefile
      tests/io/ppms/Makefile
      tests/io/tiff/Makefile
    tests/labeling/Makefile
    tests/linear/Makefile
      tests/linear/local/Makefile
    tests/literal/Makefile
    tests/logical/Makefile
    tests/make/Makefile
    tests/math/Makefile
    tests/metal/Makefile
      tests/metal/make/Makefile
      tests/metal/math/Makefile
    tests/morpho/Makefile
      tests/morpho/approx/Makefile
      tests/morpho/attribute/Makefile
      tests/morpho/closing/Makefile
      tests/morpho/closing/approx/Makefile
      tests/morpho/elementary/Makefile
      tests/morpho/opening/Makefile
      tests/morpho/opening/approx/Makefile
      tests/morpho/reconstruction/Makefile
        tests/morpho/reconstruction/by_dilation/Makefile
        tests/morpho/reconstruction/by_erosion/Makefile
      tests/morpho/tree/Makefile
        tests/morpho/tree/filter/Makefile
      tests/morpho/watershed/Makefile
    tests/norm/Makefile
    tests/opt/Makefile
    tests/pw/Makefile
    tests/set/Makefile
    tests/tag/Makefile
    tests/test/Makefile
    tests/topo/Makefile
      tests/topo/skeleton/Makefile
    tests/trace/Makefile
    tests/trait/Makefile
      tests/trait/image/Makefile
      tests/trait/op/Makefile
      tests/trait/value/Makefile
    tests/transform/Makefile
    tests/unit_test/Makefile
    tests/upscaling/Makefile
      tests/upscaling/art/Makefile
    tests/util/Makefile
    tests/value/Makefile
      tests/value/builtin/Makefile
      tests/value/concept/Makefile
    tests/win/Makefile
    tests/world/Makefile
      tests/world/binary_2d/Makefile
      tests/world/inter_pixel/Makefile
        tests/world/inter_pixel/dim2/Makefile
])

dnl<<lrde
AC_CONFIG_FILES([
  tests/extract/Makefile
  tests/linear/gaussian/Makefile
  tests/registration/Makefile
  tests/subsampling/Makefile
])
dnl>>


## -------------- ##
## Applications.  ##
## -------------- ##

# Ask for the creation of a apps/data.hh, used to access to
# data (images) from apps.
AC_CONFIG_FILES([apps/data.hh])

AC_ARG_ENABLE([apps],
              [AS_HELP_STRING([--enable-apps],
                              [enable application])])
AM_CONDITIONAL([ENABLE_APPS], [test "x$enable_apps" = "xyes"])

# Ask for the creation of applications' Makefiles.
AC_CONFIG_FILES([
  apps/Makefile
    apps/constrained-connectivity/Makefile
    apps/graph-morpho/Makefile
    apps/mesh-segm-skel/Makefile
    apps/papers/Makefile
      apps/papers/levillain.09.ismm/Makefile
])

# Configure tests.
# FIXME: Consider using `sed' instead of `configure' to create these
# tests for the sake of speed.
AC_CONFIG_FILES([apps/mesh-segm-skel/test-mesh-max-curv],
                [chmod +x apps/mesh-segm-skel/test-mesh-max-curv])
AC_CONFIG_FILES([apps/mesh-segm-skel/test-mesh-complex-max-curv],
                [chmod +x apps/mesh-segm-skel/test-mesh-complex-max-curv])
AC_CONFIG_FILES([apps/mesh-segm-skel/test-mesh-segm],
                [chmod +x apps/mesh-segm-skel/test-mesh-segm])
AC_CONFIG_FILES([apps/mesh-segm-skel/test-mesh-complex-segm],
                [chmod +x apps/mesh-segm-skel/test-mesh-complex-segm])
AC_CONFIG_FILES([apps/mesh-segm-skel/test-mesh-complex-max-curv-segm],
                [chmod +x apps/mesh-segm-skel/test-mesh-complex-max-curv-segm])
AC_CONFIG_FILES([apps/mesh-segm-skel/test-mesh-complex-skel],
                [chmod +x apps/mesh-segm-skel/test-mesh-complex-skel])

AC_CONFIG_FILES([apps/constrained-connectivity/test-constrained-connectivity],
                [chmod +x apps/constrained-connectivity/test-constrained-connectivity])

# Flags for apps.
AC_ARG_VAR([APPS_CXXFLAGS], [C++ compiler flags for applications])
# We want fast binaries for apps.
if test -z "$APPS_CXXFLAGS"; then
  if test "$GXX" = yes; then
    APPS_CXXFLAGS="-O3 -DNDEBUG -ggdb $WARNINGS_CXXFLAGS"
  elif test "$ICPC" = yes; then
    APPS_CXXFLAGS="-O3 -DNDEBUG -g $WARNINGS_CXXFLAGS"
  fi
fi

## ------- ##
## Tools.  ##
## ------- ##

AC_ARG_ENABLE([tools],
              [AS_HELP_STRING([--enable-tools], [enable tools])])
AM_CONDITIONAL([ENABLE_TOOLS], [test "x$enable_tools" = "xyes"])

# Ask for the creation of tools' Makefiles.
AC_CONFIG_FILES([tools/Makefile])

# Flags for tools.
AC_ARG_VAR([TOOLS_CXXFLAGS], [C++ compiler flags for tools])
# We want fast binaries for tools.
if test -z "$TOOLS_CXXFLAGS"; then
  if test "$GXX" = yes; then
    TOOLS_CXXFLAGS="-O3 -DNDEBUG -ggdb $WARNINGS_CXXFLAGS"
  elif test "$ICPC" = yes; then
    TOOLS_CXXFLAGS="-O3 -DNDEBUG -g $WARNINGS_CXXFLAGS"
  fi
fi


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
Boost Tuple             --with-boost=@<:@=DIR@:>@     $oln_have_boost_tuple
CFITSIO                 --with-cfitsio@<:@=DIR@:>@    $oln_have_cfitsio
GDCM                    --with-gdcm@<:@=DIR@:>@       $oln_have_gdcm
Magick++                --with-magickxx         $oln_have_magickxx
Qt                      --with-qt               $oln_have_qt
TIFF                    --with-tiff@<:@=DIR@:>@       $oln_have_tiff
Trimesh                 --enable-trimesh        enable_trimesh
-------------------------------------------------------------------------------




=============
| Utilities |
=============


                        Option                  Enabled
-------------------------------------------------------------------------------
Apps                    --enable-apps           $enable_apps
Tools                   --enable-tools          $enable_tools
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
  BOOST_ROOT      = $BOOST_ROOT
  MAGICKXX_CFLAGS = $MAGICKXX_CPPFLAGS
  MAGICKXX_LIBS   = $MAGICKXX_LDFLAGS
  APPS_CXXFLAGS   = $APPS_CXXFLAGS
  TOOLS_CXXFLAGS  = $TOOLS_CXXFLAGS



*******************************************************************************
Olena is configured as stated above.  Please verify that this configuration
matches your expectations.

Then, type 'make' to build Olena and 'make install' to install it on
your system.
])
fi])


## -------- ##
## Output.  ##
## -------- ##

AC_OUTPUT
