###
### These macros pertain to the Olena distribution only, i.e.
### are of little or no use to the Olena user.
###

# This macro is used by the test selection system.

AC_DEFUN([OLN_TESTS], dnl Auxiliary function for test categories selection
         [AC_CACHE_CHECK([whether to enable tests in `$1'],
                         [oln_cv_tests_$1],
                         [oln_cv_tests_$1=no
                          if test -d "$srcdir/oln/tests/$1"; then
                            case "$oln_cv_tests_selection" in
                              *all* | *$1* )
                                 oln_cv_tests_$1=yes
                                ;;
                            esac
                          fi])
          if test x$oln_cv_tests_$1 != xno; then
            OLN_TESTS_SUBDIRS="$OLN_TESTS_SUBDIRS $1"
            AC_CONFIG_FILES([oln/tests/$1/Makefile])
          fi])dnl End of OLN_TESTS

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
