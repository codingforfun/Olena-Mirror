# Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011 EPITA Research and
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

AC_INIT([scribogimpplugin], [0.1], [olena@lrde.epita.fr], [scribo])

# M4 macros.
AC_CONFIG_MACRO_DIR([m4])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])

# Automake
AM_INIT_AUTOMAKE([1.10 subdir-objects check-news tar-ustar dist-bzip2 nostdinc -Wall])


# Package metadata.
AC_DEFINE_UNQUOTED([SCRIBO_PACKAGE_NAME], ["$PACKAGE_NAME"],
                   [Package Full name.])
AC_DEFINE_UNQUOTED([SCRIBO_PACKAGE_BUGREPORT], ["$PACKAGE_BUGREPORT"],
                   [Bug report address.])
AC_DEFINE_UNQUOTED([SCRIBO_PACKAGE_STRING], ["$PACKAGE_STRING"],
                   [Full name and version.])
AC_DEFINE_UNQUOTED([SCRIBO_PACKAGE_VERSION], ["$PACKAGE_VERSION"],
                   [Package Version.])

AC_ISC_POSIX
AC_LANG([C++])
AC_PROG_CXX
#AC_LANG([C])
#AC_PROG_CC
#AM_PROG_CC_STDC
AC_HEADER_STDC


ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

#AM_MAINTAINER_MODE

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

OLN_WITH_TESSERACT


# ----- #
# Gimp  #
# ----- #

GIMP_REQUIRED_VERSION=2.2.0
PKG_CHECK_MODULES(GIMP,
  gimp-2.0 >= $GIMP_REQUIRED_VERSION gimpui-2.0 >= $GIMP_REQUIRED_VERSION)
AC_SUBST(GIMP_CFLAGS)
AC_SUBST(GIMP_LIBS)
#GIMP_LIBDIR=`$PKG_CONFIG --variable=gimplibdir gimp-2.0`
GIMP_LIBDIR=`echo ~/.gimp-2.4`
AC_SUBST(GIMP_LIBDIR)


AM_CONFIG_HEADER(config.h)

# Project Files
AC_CONFIG_SRCDIR([src/main.cc])


# i18n stuff

GETTEXT_PACKAGE=gimp20-plugin-template
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE",
				    [The gettext translation domain.])

# Add the languages which your application supports here.
ALL_LINGUAS="az de fr sk sv zh_TW"

AC_PROG_INTLTOOL
AM_GLIB_GNU_GETTEXT

AC_CHECK_FUNCS(bind_textdomain_codeset)

LOCALEDIR='${datadir}/locale'
DATADIR='${datadir}/plugin_name'

AC_SUBST(LOCALEDIR)
AC_SUBST(DATADIR)


AC_MSG_CHECKING([if GTK+ is version 2.7.0 or newer])
if $PKG_CONFIG --atleast-version=2.7.0 gtk+-2.0; then
  have_gtk_2_7=yes
else
  have_gtk_2_7=no
fi
AC_MSG_RESULT($have_gtk_2_7)

if test "x$have_gtk_2_7" != "xyes"; then
  CPPFLAGS="$CPPFLAGS -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED"
fi


AC_MSG_CHECKING([if GIMP is version 2.3.0 or newer])
if $PKG_CONFIG --atleast-version=2.3.0 gimp-2.0; then
  have_gimp_2_3=yes
else
  have_gimp_2_3=no
fi
AC_MSG_RESULT($have_gimp_2_3)

if test "x$have_gimp_2_3" != "xyes"; then
  CPPFLAGS="$CPPFLAGS -DGIMP_DISABLE_DEPRECATED"
fi



AC_CONFIG_FILES([
Makefile
help/Makefile
help/en/Makefile
help/images/Makefile
src/Makefile
po/Makefile.in
])

AC_OUTPUT
