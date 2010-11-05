# Copyright (C) 2005, 2009, 2010 EPITA Research and Development
# Laboratory (LRDE).
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

AC_PREREQ([2.61])

# Catch some macros that are not expanded.
m4_pattern_forbid([^AC_LIBLTDL_])
m4_pattern_forbid([^DYN_])
m4_pattern_allow([^DYN_FIXTURES$])
m4_pattern_forbid([^ERB$])
m4_pattern_forbid([^RBCONFIG$])

## ---------------- ##
## Package set up.  ##
## ---------------- ##

AC_INIT([Extatica], [0.1], [olena@lrde.epita.fr], [extatica])

# Auxiliary files.
AC_CONFIG_AUX_DIR([_config])

# Automake.
AM_INIT_AUTOMAKE([1.10 check-news dist-bzip2 -Wall nostdinc])


## --------------------- ##
## C++ Compiler Set up.  ##
## --------------------- ##

# Look for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

# Speed the compilation up.
if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -pipe"
fi

# Use good warnings.
DYN_CXX_WARNINGS([[-Wall],
                  [-W],
                  [-Wcast-align],
                  [-Wcast-qual],
                  [-Wformat],
                  [-Wmissing-prototypes],
                  [-Wstrict-prototypes],
                  [-Wwrite-strings],
                  [-Wbad-function-cast],
                  [-Wmissing-declarations],
                  [-Wnested-externs]])

## -------- ##
## Libtool. ##
## -------- ##

# Configure libltdl
AC_CONFIG_SUBDIRS(libltdl)

AC_DISABLE_STATIC
# Use the installable version of ltdl

# AC_LIBLTDL_INSTALLABLE
# INSTALL_LTDL

AC_LIBLTDL_CONVENIENCE

# Check for dlopen support
AC_LIBTOOL_DLOPEN
# We don't need static libraries, speed the compilation up.
AC_DISABLE_STATIC
# Configure libtool
AC_PROG_LIBTOOL

AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

## ----------- ##
## Libiberty.  ##
## ----------- ##

DYN_LIBIBERTY

# Are we running on Darwin (Mac OS X)?  If so, we'll have to deal with
# MacPorts' libiberty's issues w.r.t `environ'.
AM_CONDITIONAL([DARWIN], [echo "$host_os" | grep '^darwin'])

## ------ ##
## Boost. ##
## ------ ##

BOOST_FILESYSTEM
BOOST_STRING_ALGO

## ---------------------- ##
## SWIG Python bindings.  ##
## ---------------------- ##

AC_ARG_ENABLE([swig-python-bindings],
  [AS_HELP_STRING([--enable-swig-python-bindings],
                  [build SWIG Python bindings @<:@default=auto@:>@])],
  [],
  [enable_swig_python_bindings=auto])

# Trick borrowed from autotroll.m4 to get a decent flow control with 'break'.
for xtc_ignored in once; do

  # Ensure dynamic libraries are enabled.
  if test "x$enable_shared" = xno; then
    if test "x$enable_swig_python_bindings" != xauto; then
      AC_MSG_ERROR([`--enable-swig-python-bindings' was passed, but dynamic
libraries are disabled.  Try to invoke configure with `--enable-shared'.])
    fi
    break
  fi

  # Check for SWIG.
  AC_PROG_SWIG([1.3.35])
  if (eval "$SWIG -version") >/dev/null 2>&1; then :; else
    if test "x$enable_swig_python_bindings" != xauto; then
      AC_MSG_ERROR([`--enable-swig-python-bindings' was passed, but SWIG 1.3.35
(or greater) cannot be found.])
    fi
    break
  fi

  # Check for Python.
  AM_PATH_PYTHON([2.4], [], [xtc_have_python=no])
  if test "x$xtc_have_python" = xno; then
    if test "x$enable_swig_python_bindings" != xauto; then
      AC_MSG_ERROR([`--enable-swig-python-bindings' was passed, but Python 2.4
(or greater) cannot be found.])
    fi
    break
  fi

  # Warn about Python 2.6 which probably won't work with our bindings
  # (for the moment).
  AS_VERSION_COMPARE([2.5], [$am_cv_python_version],
    [AC_MSG_WARN([Python 2.6 and greater may not work with SWIG Python bindings.])])

  # Set PYTHONINC path.
  adl_CHECK_PYTHON
  # Check for Python headers.
  save_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS -I$PYTHONINC"
  AC_CHECK_HEADERS([Python.h], [], [xtc_have_python_h=no])
  CPPFLAGS=$save_CPPFLAGS
  if test "x$xtc_have_python_h" = xno; then
    if test "x$enable_swig_python_bindings" != xauto; then
      AC_MSG_ERROR([`--enable-swig-python-bindings' was passed, but `Python.h'
cannot be found.  Try adding `-I <Python include path>' to `CPPFLAGS'.])
    fi
    break
  fi

  # All tests passed: enable SWIG Python bindings.
  xtc_swig_python_bindings=yes
done

AC_MSG_CHECKING([whether to build SWIG Python bindings])
if test "x$xtc_swig_python_bindings" = "xyes"; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([SWIG_PYTHON_BINDINGS],
               [test "x$xtc_swig_python_bindings" = "xyes"])

AC_CONFIG_FILES([swig/Makefile
		 swig/python/Makefile])

AC_CONFIG_FILES([swig/run], [chmod +x swig/run])



# FIXME: Doesn't Autoconf provide something like abs_top_srcdir?
my_abs_srcdir=`cd $srcdir && pwd`
# FIXME: Likewise.
my_abs_builddir=`pwd`

# FIXME: Remove me when function_loader will be pure C++
AC_SUBST([DYNDIR], [$my_abs_srcdir/src])

AC_SUBST([DYN_DATADIR], [$my_abs_builddir/data])

AC_SUBST([DYN_FIXTURES], [$my_abs_srcdir/test/fixtures])

# Absolute path to Milena.
AC_SUBST([MILENA_DIR], [$my_abs_srcdir/../milena])

## ------------------- ##
## Ruby Configuration. ##
## ------------------- ##

# Make available Config::CONFIG
# AC_DEFINE and AC_SUBST everything in this hash
# All keys are prefixed by RUBY_.
RBCONFIG()

# ...
echo $srcdir/config/local-config.rb.in
MK_LOCAL_CONFIG_RB_IN([$srcdir/_config/local-config.rb.in])

## --------------- ##
## Configuration.  ##
## --------------- ##

# Ask for config.h.
AM_CONFIG_HEADER([_config/config.h])

# Ask for the Makefile creations.
AC_CONFIG_FILES([
  Makefile
  bin/Makefile
  bin/dyn-config
  libmd5/Makefile
  src/Makefile
  src/config.hh
    src/wrappers/Makefile
  data/Makefile
  config/Makefile
  _config/local-config.rb
  test/Makefile
    test/vaucanson/Makefile
    test/wrappers/Makefile
], [chmod +x bin/dyn-config])

# Instantiate the output files.
AC_OUTPUT
