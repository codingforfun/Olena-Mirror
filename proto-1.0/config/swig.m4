# Doxygen >= 1.3.4 is needed.

AC_DEFUN([AC_CHECK_SWIG_VERSION],
[dnl
  AC_CACHE_CHECK([for swig >= 1.3],
                  [ac_cv_swig_version],
                  [[case `$SWIG -version | grep -i version` in
                  *1.3*	| \
   		  *1.[4-9]* | \
                  *[2-9].*  )
      		       ac_cv_swig_version=recent;;
                  *)
                       ac_cv_swig_version=old;;
                  esac]])
  fi
])


# Recent versions of SWIG use -nountime instead of -c

AC_DEFUN([AC_CHECK_SWIG_FLAGS],
[dnl
  AC_CACHE_CHECK([for SWIG supports for -noruntime],
                  [ac_cv_swig_flags],
                  [if $SWIG -help 2>&1 | grep -- "-noruntime" > /dev/null 2>&1; then
			ac_cv_swig_flags=recent
                      else
                        ac_cv_swig_flags=old
                   fi])
   case "$ac_cv_swig_flags" in
     recent)
	SWIG_FLAGS="-noruntime"
	;;
     old)
	SWIG_FLAGS="-c"
      	;;
   esac

   AC_SUBST([SWIG_FLAGS])
])
