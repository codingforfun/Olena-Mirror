###
### Stuff for header files
###

# MLC_PATH_USER

# Checks for the location of Metalic sources specified with the
# user with the flag --with-metalic.

AC_DEFUN([MLC_PATH_USER], 
[dnl
  AC_CACHE_CHECK([for Metalic in user-specified directory],
		 [mlc_cv_user_hint], 
                 [mlc_cv_user_hint=no
                  AC_ARG_WITH([metalic],
                    [AC_HELP_STRING([--with-metalic=DIR],
                    [Include directory where Metalic is installed (optional)])],
                    [if test -r "$withval/mlc/config/system.hh"; then
		       mlc_cv_user_hint=$withval
		     fi])])
])

# _MLC_CHECK_HEADERS

# Internal macro used by MLC_PATH_HEADERS.
# This macro checks the actual availability of Metalic headers after
# the other heuristics have tried setting MLC_INCLUDE_DIR and CPPFLAGS.

AC_DEFUN([_MLC_CHECK_HEADERS], 
[dnl
 AC_REQUIRE([AC_PROG_CXX])
 AC_LANG_PUSH([C++])

 have_metalic=yes
 mlc_save_CPPFLAGS=$CPPFLAGS
 CPPFLAGS="$MLC_EXTRA_CPPFLAGS $MLC_CPPFLAGS $CPPFLAGS"
 echo "CPPFLAGS: $CPPFLAGS"
 # At this point, we can be in a situation where pconf.hh does not
 # exist _yet_. This is the case when we are building metalic. In that 
 # particular case, we need a workaround.
 AC_CHECK_HEADER([mlc/config/pconf.hh], [], 
          [CPPFLAGS="$CPPFLAGS -DIGNORE_PCONF_HH"])
 AC_CHECK_HEADER([mlc/config/system.hh], [], [have_metalic=no])
 CPPFLAGS=$mlc_save_CPPFLAGS

 AC_LANG_POP([C++])
])

# MLC_PATH_HEADERS

# Search for a Metalic location using user flags, environment variables or
# default path.

# This macro sets, if needed, MLC_CPPFLAGS before proceeding with
# _MLC_CHECK_HEADERS.

AC_DEFUN([MLC_PATH_HEADERS],
[dnl 
  AC_REQUIRE([MLC_PATH_USER])

  # User-specified directory overrides any other definition
  if test "x$mlc_cv_user_hint" != xno; then
     if test "x$MLC_INCLUDE_DIR" != x ; then
       AC_MSG_WARN([using $mlc_cv_user_hint instead of $MLC_INCLUDE_DIR])
     fi
     MLC_INCLUDE_DIR="$mlc_cv_user_hint"
  fi

  AC_ARG_VAR([MLC_INCLUDE_DIR], 
	     [location of Metalic (<include dir>, should be autodetected)])
  if test "x$MLC_INCLUDE_DIR" != x ; then
     MLC_CPPFLAGS="-I$MLC_INCLUDE_DIR $MLC_CPPFLAGS"
  fi
  AC_SUBST([MLC_INCLUDE_DIR])
  AC_SUBST([MLC_CPPFLAGS])

  _MLC_CHECK_HEADERS
])

# AC_WITH_MLC

# Invoke configuration code to test for Metalic and set a collection
# of appropriate flags.

AC_DEFUN([AC_WITH_MLC],
[dnl
  AC_REQUIRE([MLC_PATH_HEADERS])

  AC_SUBST([MLC_CPPFLAGS])
  AC_SUBST([MLC_CXXFLAGS])
])

###
### Internal stuff for Intègre
###

m4_define([MTG_CONTACT], [olena-bugs@lrde.epita.fr])
