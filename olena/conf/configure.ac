# Process this file with autoconf to produce a configure script.

# Copyright (C) 2003  EPITA Research and Development Laboratory
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


AC_PREREQ([2.53])

AC_INIT([oln-conf], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_SRCDIR([oln-config.shin])

AC_WITH_OLN

if test "x$have_olena" = xyes; then
  AC_MSG_CHECKING([whether Olena works with the selected development environment])
  AC_LANG_PUSH([C++])
  AC_LINK_IFELSE([@%:@include <ntg/all.hh>
                  int main()
                  {
	              oln::float_s f = 3.14;
                      oln::int_u8 i = f;
                      return (int)i;
                  }],
                  [AC_MSG_RESULT([yes])], 
                  [AC_MSG_RESULT([no])
                   have_olena=no])
  AC_LANG_POP([C++])
fi


if test "x$have_olena" != xyes; then
  echo "Olena not found" 1>&2
  exit 1
fi

AC_CXX_FLAGS

AC_CONFIG_FILES([oln-config.sh:oln-config.shin], [chmod +x oln-config.sh])
AC_OUTPUT
