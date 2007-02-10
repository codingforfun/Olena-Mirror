# Process this file with autoconf to produce a configure script.

# Copyright (C) 2002, 2003  EPITA Research and Development Laboratory
#
# This file is part of the Olena Library.  This library is free
# software; you can redistribute it and/or modify it under the terms
# of the GNU General Public License version 2 as published by the
# Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
# As a special exception, you may use this file as part of a free
# software library without restriction.  Specifically, if other files
# instantiate templates or use macros or inline functions from this
# file, or you compile this file and link it with other files to
# produce an executable, this file does not by itself cause the
# resulting executable to be covered by the GNU General Public
# License.  This exception does not however invalidate any other
# reasons why the executable file might be covered by the GNU General
# Public License.


AC_PREREQ([2.54])

source oln-version

AC_INIT([$OLN_UTILITIES_PNAME], [$OLN_UTILITIES_VERSION], [$OLN_UTILITIES_CONTACT])
AC_CONFIG_SRCDIR([oln-version])
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

OLN_UTILITIES_SUBDIRS=""

if test -d $srcdir/morpho; then
   AC_ARG_WITH([morpho-utilities],
               AC_HELP_STRING([--without-morpho-utilities],
                              [disable building the morpho utilities])
               [ac_cv_oln_utils_morpho], [ac_cv_oln_utils_morpho=yes])
   AC_CACHE_CHECK([whether to build the morpho utilities],
                  [ac_cv_oln_utils_morpho], [ac_cv_oln_utils_morpho=yes])

  if x$ac_cv_oln_utils_morpho != xno; then
      OLN_UTILITIES_SUBDIRS="$OLN_UTILITIES_SUBDIRS morpho"
      AC_CONFIG_FILES([morpho/Makefile])
  fi
fi

AC_OUTPUT