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

