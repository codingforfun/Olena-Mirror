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

AC_INIT([Swilena], [1.0a], [olena@lrde.epita.fr], [swilena])

# M4 macros.
AC_CONFIG_MACRO_DIR([m4])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])

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

# The current approach to providing Swilena a path to Milena's headers
# is naive: for instance, we cannot provide two paths (source and
# build dirs) in the case of an Olena build using a parallel build
# (where srcdir != builddir).  Improve if needed.

AC_ARG_WITH([milena],
  [AC_HELP_STRING([--with-milena=DIR], [path to Milena headers])],
  [if test x"$with_milena" = xno; then
     AC_MSG_ERROR([Milena is required for Swilena.  Try to invoke configure
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


## --------- ##
## Swilena.  ##
## --------- ##

# Use Libtool.
# To be replaced by a call to LT_INIT as soon as Libtool 2.2 is used.
AC_PROG_LIBTOOL

# Ensure dynamic libraries are enabled.
if test "x$enable_shared" = xno; then
  AC_MSG_ERROR([Dynamic libraries are disabled.  Try to invoke
configure with `--enable-shared'.])
fi
# Check for SWIG.
AC_PROG_SWIG([1.3.35])
if (eval "$SWIG -version") >/dev/null 2>&1; then :; else
  AC_MSG_ERROR([SWIG 1.3.35 is required for Swilena])
fi
# Check for Python.
AM_PATH_PYTHON([2.4], [],
               [AC_MSG_ERROR([Python 2.4 is required for Swilena.])])
AS_VERSION_COMPARE([2.5], [$am_cv_python_version],
  [AC_MSG_WARN([Python 2.6 and greater may not work with Swilena.])])
adl_CHECK_PYTHON
# Check for Python headers.
save_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS -I$PYTHONINC"
AC_CHECK_HEADERS([Python.h], [],
                 [AC_MSG_ERROR([`Python.h' is required for Swilena.
Try adding `-I <Python include path>' to `CPPFLAGS'.])])
CPPFLAGS=$save_CPPFLAGS

AC_ARG_VAR([SWILENA_CXXFLAGS], [C++ compiler flags for tools])
# We want fast binaries for tools.
if test -z "$SWILENA_CXXFLAGS"; then
  if test "$GXX" = yes; then
    SWILENA_CXXFLAGS="-O3 -DNDEBUG -ggdb $WARNINGS_CXXFLAGS"
  elif test "$ICPC" = yes; then
    SWILENA_CXXFLAGS="-O3 -DNDEBUG -g $WARNINGS_CXXFLAGS"
  fi
fi

AC_CONFIG_FILES([Makefile
                 python/Makefile])

AC_CONFIG_FILES([run], [chmod +x run])

AC_CONFIG_FILES([python/sps:python/sps-common.in:python/sps.in],
                [chmod +x python/sps])
AC_CONFIG_FILES([python/sps-local:python/sps-common.in:python/sps-local.in],
                [chmod +x python/sps-local])

AC_OUTPUT
