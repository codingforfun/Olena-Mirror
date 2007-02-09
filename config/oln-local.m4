## Copyright (C) 2003, 2004, 2005, 2006 EPITA Research and Development
## Laboratory
##
## This file is part of the Olena Library.  This library is free
## software; you can redistribute it and/or modify it under the terms
## of the GNU General Public License version 2 as published by the
## Free Software Foundation.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this library; see the file COPYING.  If not, write to
## the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
## Boston, MA 02110-1301, USA.
##
## As a special exception, you may use this file as part of a free
## software library without restriction.  Specifically, if other files
## instantiate templates or use macros or inline functions from this
## file, or you compile this file and link it with other files to
## produce an executable, this file does not by itself cause the
## resulting executable to be covered by the GNU General Public
## License.  This exception does not however invalidate any other
## reasons why the executable file might be covered by the GNU General
## Public License.

###
### These macros pertain to the Olena distribution only, i.e.
### are of little or no use to the Olena user.
###

# This macro is used by the test selection system.

AC_DEFUN([OLN_TESTS], dnl Auxiliary function for test categories selection
         [AC_CACHE_CHECK([whether to enable tests in `$1'],
                         [oln_cv_tests_$1],
                         [oln_cv_tests_$1=no
                          if test -d "$srcdir/olena/tests/$1"; then
                            case "$oln_cv_tests_selection" in
                              *all* | *$1* )
                                 oln_cv_tests_$1=yes
                                ;;
                            esac
                          fi])
          if test x$oln_cv_tests_$1 != xno; then
            OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS $1"
            AC_CONFIG_FILES([olena/tests/$1/Makefile])
          fi])dnl End of OLN_TESTS

# Same macro tuned for itegre

AC_DEFUN([NTG_TESTS], dnl Auxiliary function for test categories selection
         [AC_CACHE_CHECK([whether to enable tests in `$1'],
                         [ntg_cv_tests_$1],
                         [ntg_cv_tests_$1=no
                          if test -d "$srcdir/integre/tests/$1"; then
                            case "$ntg_cv_tests_selection" in
                              *all* | *$1* )
                                 ntg_cv_tests_$1=yes
                                ;;
                            esac
                          fi])
          if test x$ntg_cv_tests_$1 != xno; then
            NTG_TESTS_SUBDIRS="$NTG_TESTS_SUBDIRS $1"
            AC_CONFIG_FILES([integre/tests/$1/Makefile])
          fi])dnl End of NTG_TESTS

# Same macro tuned for metalic

AC_DEFUN([MLC_TESTS], dnl Auxiliary function for test categories selection
         [AC_CACHE_CHECK([whether to enable tests in `$1'],
                         [mlc_cv_tests_$1],
                         [mlc_cv_tests_$1=no
                          if test -d "$srcdir/metalic/tests/$1"; then
                            case "$mlc_cv_tests_selection" in
                              *all* | *$1* )
                                 mlc_cv_tests_$1=yes
                                ;;
                            esac
                          fi])
          if test x$mlc_cv_tests_$1 != xno; then
            MLC_TESTS_SUBDIRS="$MLC_TESTS_SUBDIRS $1"
            AC_CONFIG_FILES([metalic/tests/$1/Makefile])
          fi])dnl End of MLC_TESTS

# FIXME: Improve this (see TC's swig.m4).
AC_DEFUN([AC_CHECK_SWIG13],
[dnl
     AC_ARG_VAR([SWIG], [the SWIG interface generator])
     AC_CHECK_PROGS([SWIG], [swig], [$am_aux_dir/missing swig])
     AC_CACHE_CHECK([for SWIG >= 1.3],
                    [oln_cv_recent_swig],
                    [oln_cv_recent_swig=no
		     if test "x$SWIG" != "x$am_aux_dir/missing swig"; then
                     if $SWIG -version 2>&1 | \
	                grep 'SWIG Version @<:@0-9@:>@@<:@0-9@:>@*\.@<:@0-9@:>@@<:@0-9@:>@*' \
	                >/dev/null 2>&1; then
                        swig_ver=`$SWIG -version 2>&1 | grep 'SWIG Version'`
                        swig_major=`echo "$swig_ver" | sed -e 's,^.*Version \(@<:@0-9@:>@@<:@0-9@:>@*\).*$,\1,g'`
                        swig_minor=`echo "$swig_ver" | sed -e 's,^.*Version @<:@0-9@:>@@<:@0-9@:>@*\.\(@<:@0-9@:>@@<:@0-9@:>@*\).*$,\1,g'`
	                if test `expr $swig_major \* 100 + $swig_minor` -ge 103; then
                           oln_cv_recent_swig=yes
                        fi
		     fi
                     fi])

])
