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

# This macro is used to detect tools necessary to build the
# reference manual.

AC_DEFUN([DOC_CHECK_PROG], [dnl Auxiliary function for program checking
  AC_CHECK_PROGS([$1], [$2], [$am_aux_dir/missing $2])
  if test -z "$[]$1"; then
    oln_cv_build_ref=no
    AC_MSG_WARN([$2 not found -- cannot build documentation])
  fi
])dnl End of DOC_CHECK_PROG
