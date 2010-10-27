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

AC_INIT([Olena], [1.0a], [olena@lrde.epita.fr], [olena])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([build-aux/Makefile])

# Automake.
AM_INIT_AUTOMAKE([1.10 subdir-objects check-news dist-bzip2 nostdinc -Wall])

AC_CONFIG_FILES([Makefile])


## ------------------------------ ##
## ``Enable Everything'' Switch.  ##
## ------------------------------ ##

# Enable all bundled features (Swilena, Scribo, etc).  This
# option is useful to maintainers to ensure they do not break optional
# parts while modifying the core of the project. If both
# ``--enable-all'' is set and a given feature is disabled (say,
# ``--disable-apps''), then the disable flag has priority.

AC_ARG_ENABLE([all],
  [AS_HELP_STRING([--enable-all], [enable maintainer mode])],
  [if test "x$enable_all" != xno; then
     enable_swilena=yes
dnl<<lrde
     enable_scribo=yes
dnl>>
   fi])


## -------- ##
## Milena.  ##
## -------- ##

# Milena is always enabled.
AC_CONFIG_SUBDIRS([milena])


## --------- ##
## Swilena.  ##
## --------- ##

AC_ARG_ENABLE([swilena],
              [AS_HELP_STRING([--enable-swilena],
                              [build Swilena Python bindings])])
AM_CONDITIONAL([ENABLE_SWILENA], [test "x$enable_swilena" = xyes])
AC_CONFIG_SUBDIRS([swilena])


dnl<<lrde
## -------- ##
## Scribo.  ##
## -------- ##

AC_ARG_ENABLE([scribo],
              [AS_HELP_STRING([--enable-scribo],
                              [enable Scribo])])
AM_CONDITIONAL([ENABLE_SCRIBO], [test "x$enable_scribo" = xyes])
AC_CONFIG_SUBDIRS([scribo])
dnl>>


AC_OUTPUT
