###
### Stuff for header files
###

# NTG_PATH_USER

# Checks for the location of Intègre sources specified with the
# user with the flag --with-integre.

AC_DEFUN([NTG_PATH_USER], 
[dnl
  AC_CACHE_CHECK([for Intègre in user-specified directory],
		 [ntg_cv_user_hint], 
                 [ntg_cv_user_hint=no
                  AC_ARG_WITH([integre],
                    [AC_HELP_STRING([--with-integre=DIR],
                    [Include directory where Intègre is installed (optional)])],
                    [if test -r "$withval/ntg/config/system.hh"; then
		       ntg_cv_user_hint=$withval
		     fi])])
])

# _NTG_CHECK_HEADERS

# Internal macro used by NTG_PATH_HEADERS.
# This macro checks the actual availability of Intègre headers after
# the other heuristics have tried setting NTG_INCLUDE_DIR and CPPFLAGS.

AC_DEFUN([_NTG_CHECK_HEADERS], 
[dnl
 AC_REQUIRE([AC_PROG_CXX])
 AC_LANG_PUSH([C++])

 have_integre=yes
 ntg_save_CPPFLAGS=$CPPFLAGS
 CPPFLAGS="$NTG_EXTRA_CPPFLAGS $NTG_CPPFLAGS $CPPFLAGS"
 # At this point, we can be in a situation where pconf.hh does not
 # exist _yet_. This is the case when we are building integre. In that 
 # particular case, we need a workaround.
 AC_CHECK_HEADER([ntg/config/pconf.hh], [], 
          [CPPFLAGS="$CPPFLAGS -DIGNORE_PCONF_HH"])
 AC_CHECK_HEADER([ntg/config/system.hh], [], [have_integre=no])
 CPPFLAGS=$ntg_save_CPPFLAGS

 AC_LANG_POP([C++])
])

# NTG_PATH_HEADERS

# Search for an Intègre location using user flags, environment variables or
# default path.

# This macro sets, if needed, NTG_CPPFLAGS before proceeding with
# _NTG_CHECK_HEADERS.

AC_DEFUN([NTG_PATH_HEADERS],
[dnl 
  AC_REQUIRE([NTG_PATH_USER])

  # User-specified directory overrides any other definition
  if test "x$ntg_cv_user_hint" != xno; then
     if test "x$NTG_INCLUDE_DIR" != x ; then
       AC_MSG_WARN([using $ntg_cv_user_hint instead of $NTG_INCLUDE_DIR])
     fi
     NTG_INCLUDE_DIR="$ntg_cv_user_hint"
  fi

  AC_ARG_VAR([NTG_INCLUDE_DIR], 
	     [location of Intègre (<include dir>, should be autodetected)])
  if test "x$NTG_INCLUDE_DIR" != x ; then
     NTG_CPPFLAGS="-I$NTG_INCLUDE_DIR $NTG_CPPFLAGS"
  fi
  AC_SUBST([NTG_INCLUDE_DIR])
  AC_SUBST([NTG_CPPFLAGS])

  _NTG_CHECK_HEADERS
])

# NTG_WITH_MLC

# Search for the metalic installation used by Intègre.
# FIXME: this macro should search for the metalic installation already used
# by intègre, using integre-config --metalic-includedir or something.

AC_DEFUN([NTG_WITH_MLC], 
[dnl
  AC_REQUIRE([AC_WITH_MLC])
  
  if test x$have_metalic = xno; then
    AC_MSG_ERROR([Metalic is required by Intègre. You can get it from http://www.lrde.epita.fr])
  fi
])

# NTG_CXX_NUMERIC_LIMITS

# Calls AC_CXX_NUMERICS_LIMITS and set the appropriate flags.

AC_DEFUN([NTG_CXX_NUMERIC_LIMITS], 
[dnl
  AC_REQUIRE([AC_CXX_NUMERIC_LIMITS])

  if test x$ac_cv_cxx_have_limits_infinity = xno; then
    # Usable std::numeric_limits were *not* found in <limits>.
    NTG_CPPFLAGS="-DNTG_USE_C_LIMITS $NTG_CPPFLAGS"
  fi
])

# NTG_CXX_TEMPLATE_DEPTH

# Calls AC_CXX_TEMPLATE_DEPTH and set the appropriate flags.

AC_DEFUN([NTG_CXX_TEMPLATE_DEPTH], 
[dnl
  AC_REQUIRE([AC_CXX_TEMPLATE_DEPTH])
  NTG_CXXFLAGS="$NTG_CXXFLAGS $ac_cv_cxx_template_flags"
])

# NTG_CXX_MATH_FUNC(FUNCTION, MACRO_NAME, TEST)

# Checks for the availability of a particular function from C++ using 
# AC_CXX_MATH_FUN, and add the adapted flags to NTG_CPPFLAGS.

AC_DEFUN([NTG_CXX_MATH_FUNC],
[dnl
  ntg_save_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $NTG_CPPFLAGS"
  AC_CXX_MATH_FUNC($1, $3)
  if test x$ac_cv_cxx_$1_flags = xno; then
    NTG_CPPFLAGS="$NTG_CPPFLAGS -DNTG_NEED_$2"
  elif test x$ac_cv_cxx_$1_flags != x; then
    NTG_CPPFLAGS="$NTG_CPPFLAGS $ac_cv_cxx_$1_flags"
  fi
  CPPFLAGS=$ntg_save_CPPFLAGS
])

# NTG_CXX_FLOAT_MATH

AC_DEFUN([NTG_CXX_FLOAT_MATH],
[dnl
  NTG_CXX_MATH_FUNC([roundf], [ROUNDF], [float f = roundf(0.1f)])  
])

# AC_WITH_NTG

# Invoke configuration code to test for Intègre and set a collection
# of appropriate flags.

AC_DEFUN([AC_WITH_NTG],
[dnl
  AC_REQUIRE([AC_CXX_FLAGS])
  AC_REQUIRE([NTG_WITH_MLC])
  AC_REQUIRE([NTG_CXX_TEMPLATE_DEPTH])
  AC_REQUIRE([NTG_CXX_FLOAT_MATH])
  AC_REQUIRE([NTG_PATH_HEADERS])

  echo "**** NTG_CPPFLAGS: $NTG_CPPFLAGS"
  echo "**** NTG_CXXFLAGS: $NTG_CXXFLAGS"

  AC_SUBST([NTG_CPPFLAGS])
  AC_SUBST([NTG_CXXFLAGS])
])

###
### Internal stuff for Intègre
###

m4_define([NTG_CONTACT], [olena-bugs@lrde.epita.fr])
