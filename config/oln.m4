###
### Stuff for header files
###

# OLN_PATH_LOCAL([RELATIVE-PATH-TO-OLENA-SOURCES])

# Tries to detect Olena sources "near" the current source directory.
# This macro is noticeably used in the Olena distribution itself
# to instruct "configure" to use the bundled Olena sources.

# User projects can use this macro to point their "configure" to a 
# nonstandard Olena sources location, by invoking it _before_ using
# AC_WITH_OLN. See olena/configure.ac or doc/configure.ac in the Olena
# distribution for an example.

AC_DEFUN([OLN_PATH_LOCAL],
[ifelse([$1], [], [oln_cv_local_src=no], [dnl
    AC_CACHE_CHECK([for Olena sources in local distribution],
                   [oln_cv_local_src],
	           [oln_cv_local_src=no
                    if test -r "$srcdir/$1/oln/config/system.hh"; then
                       oln_cv_local_src="$1"
                    fi])
])])

AC_DEFUN([MLC_PATH_LOCAL],
[ifelse([$1], [], [mlc_cv_local_src=no], [dnl
    AC_CACHE_CHECK([for Metalic sources in local distribution],
                   [mlc_cv_local_src],
	           [mlc_cv_local_src=no
                    if test -r "$srcdir/$1/mlc/config/system.hh"; then
                       mlc_cv_local_src="$1"
                    fi])
])])

AC_DEFUN([NTG_PATH_LOCAL],
[ifelse([$1], [], [ntg_cv_local_src=no], [dnl
    AC_CACHE_CHECK([for Integre sources in local distribution],
                   [ntg_cv_local_src],
	           [ntg_cv_local_src=no
                    if test -r "$srcdir/$1/ntg/config/system.hh"; then
                       ntg_cv_local_src="$1"
                    fi])
])])

# OLN_PATH_USER

# Checks for the location of Olena sources specified with the user
# with the flag --with-oln.  Currently the same path is used for
# integre and metalic. This will certainely change when integre and
# metalic will be distributed separately.

AC_DEFUN([OLN_PATH_USER], 
[dnl
  AC_CACHE_CHECK([for Olena in user-specified directory],
		 [oln_cv_user_hint], 
                 [oln_cv_user_hint=no
                  AC_ARG_WITH([oln],
                     [AC_HELP_STRING([--with-oln=DIR], 
                     [Include directory where Olena is installed (optional)])],
                     [if test -r "$withval/oln/config/system.hh"; then
		        oln_cv_user_hint=$withval
		      fi])])
])

# _OLN_CHECK_HEADERS

# Internal macro used by OLN_PATH_HEADERS.
# This macro checks the actual availability of Olena headers after
# the other heuristics have tried setting OLN_INCLUDE_DIR and CPPFLAGS.

AC_DEFUN([_OLN_CHECK_HEADERS], 
[dnl
 AC_REQUIRE([AC_PROG_CXX])
 AC_LANG_PUSH([C++])

 have_olena=yes
 oln_save_CPPFLAGS=$CPPFLAGS
 CPPFLAGS="$OLN_EXTRA_CPPFLAGS $CPPFLAGS"
 # At this point, we can be in a situation where pconf.hh does not
 # exist _yet_. In that particular case, we need a workaround.
 AC_CHECK_HEADER([oln/config/pconf.hh], [], 
          [CPPFLAGS="$CPPFLAGS -DIGNORE_PCONF_HH"])
 AC_CHECK_HEADER([oln/config/system.hh], [], [have_olena=no])
 CPPFLAGS=$oln_save_CPPFLAGS

 AC_LANG_POP([C++])
])

# OLN_PATH_HEADERS

# Find an Olena location from various informations: availability of
# Olena sources around the current source directory, user flags, environment
# variable.

# This macro sets, if needed, OLN_CPPFLAGS before proceeding with
# _OLN_CHECK_HEADERS.

AC_DEFUN([OLN_PATH_HEADERS],
[dnl 
  AC_REQUIRE([OLN_PATH_LOCAL])
  AC_REQUIRE([OLN_PATH_USER])

  # User-specified directory overrides any other definition
  if test "x$oln_cv_user_hint" != xno; then
     if test "x$OLN_INCLUDE_DIR" != x ; then
       AC_MSG_WARN([using $oln_cv_user_hint instead of $OLN_INCLUDE_DIR])
     fi
     OLN_INCLUDE_DIR="$oln_cv_user_hint"
  else
    if test "x$oln_cv_local_src" != xno; then
      if test "x$OLN_INCLUDE_DIR" != x ; then
        AC_MSG_WARN([using $oln_cv_user_src instead of $OLN_INCLUDE_DIR])
      fi
      OLN_INCLUDE_DIR=''

      # This is useful for sanity checks.
      OLN_LOCAL_SRC='$(top_srcdir)/'$oln_cv_local_src
      OLN_LOCAL_BUILD='$(top_builddir)/'$oln_cv_local_src

      #
      OLN_EXTRA_CPPFLAGS="-I$oln_cv_local_src -I$srcdir/$oln_cv_local_src"
      OLN_LOCAL_CPPFLAGS="-I\$(top_srcdir)/$oln_cv_local_src -I\$(top_builddir)/$oln_cv_local_src"

      MLC_LOCAL_CPPFLAGS="-I\$(top_srcdir)/$mlc_cv_local_src -I\$(top_builddir)/$mlc_cv_local_src"
      NTG_LOCAL_CPPFLAGS="-I\$(top_srcdir)/$ntg_cv_local_src -I\$(top_builddir)/$ntg_cv_local_src"

      # CPPFLAGS will need local paths.
      CPPFLAGS="$MLC_LOCAL_CPPFLAGS $NTG_LOCAL_CPPFLAGS $OLN_LOCAL_CPPFLAGS $CPPFLAGS"
      AC_SUBST([OLN_LOCAL_SRC])
      AC_SUBST([OLN_LOCAL_BUILD])
    fi
  fi

  AC_ARG_VAR([OLN_INCLUDE_DIR], 
	     [location of Olena (<include dir>, should be autodetected)])
  if test "x$OLN_INCLUDE_DIR" != x ; then
     CPPFLAGS="-I$OLN_INCLUDE_DIR $CPPFLAGS"
  fi
  AC_SUBST([OLN_INCLUDE_DIR])

  _OLN_CHECK_HEADERS
])

###
### General-use macro repository
###


# OLN_TEMPLATE_DEPTH([MINIMUM-DEPTH])

# Check for deep template recursion upto MINIMUM-DEPTH.

# Automatically adds the flag `-ftemplate-depth' to OLN_CXXFLAGS when :
# - deep template recursion is not available when it is not present
# - the compiler supports it 
# - it provides the right effect when present

AC_DEFUN([AC_CXX_TEMPLATE_DEPTH],
[dnl
  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])

  cxx_tdepth=ifelse([$1], , 50, [$1])
  AC_CACHE_CHECK([for C++ template recursion upto $cxx_tdepth levels],
                 [ac_cv_cxx_template_flags],
                 [ac_cv_cxx_template_flags=direct
                  oln_save_CPPFLAGS=$CPPFLAGS
                  oln_save_CXXFLAGS=$CXXFLAGS
                  CPPFLAGS="$CPPFLAGS -DTDEPTH=$cxx_tdepth"
                  AC_LINK_IFELSE([template<unsigned n> 
				  struct rec { 
			             typedef typename rec<n-1>::ret ret; 
                                  };
                                  template<> struct rec<0> 
                                  { typedef int ret; };
                                  int main(void) 
                                  { rec<TDEPTH>::ret i = 0; return i; }], 
                      [], 
                      [CXXFLAGS="$CXXFLAGS -ftemplate-depth-$cxx_tdepth"
                       AC_LINK_IFELSE([template<unsigned n> 
                                       struct rec { 
                                           typedef typename rec<n-1>::ret ret;
                                       };
                                      template<> struct rec<0> 
                                      { typedef int ret; };
                                      int main(void) 
                                      { rec<TDEPTH>::ret i = 0; return i; }],
                       [ac_cv_cxx_template_flags="-ftemplate-depth-$cxx_tdepth"], 
                       [ac_cv_cxx_template_flags=unsupported])])
                  CPPFLAGS=$oln_save_CPPFLAGS
                  CXXFLAGS=$oln_save_CXXFLAGS])


  AC_LANG_POP([C++])

  case "$ac_cv_cxx_template_flags" in
     unsupported )
       AC_MSG_WARN([C++ compiler does not handle template recursion upto $cxx_tdepth. Expect problems.])
       ;;
     direct ) ;;
     * )
       CXXFLAGS="$CXXFLAGS $ac_cv_cxx_template_flags"
       ;;
  esac

])

## The following macro (AC_CXX_EXCEPTIONS) is courtesy
## Luc Maisonobe, extracted from the Autoconf Macro Repository

# AC_CXX_EXCEPTIONS

# Checks whether the current C++ compiler configuration supports
# exceptions. It can be used to e.g. abort configure if exceptions
# are disabled (-fdisable-exceptions in CXXFLAGS or the like),
# instead of waiting for compilation errors.

AC_DEFUN([AC_CXX_EXCEPTIONS],
[dnl
  AC_CACHE_CHECK([whether the compiler supports exceptions],
                 [ac_cv_cxx_exceptions],
                 [AC_REQUIRE([AC_PROG_CXX])
                  AC_LANG_PUSH([C++])
                  AC_COMPILE_IFELSE([try { throw 1; } 
                                     catch (int i) { return i; }],
			            [ac_cv_cxx_exceptions=yes],
				    [ac_cv_cxx_exceptions=no])
		  AC_LANG_POP([C++])])
  if test "$ac_cv_cxx_exceptions" = yes; then
     AC_DEFINE([HAVE_EXCEPTIONS], 1, 
               [define if the compiler supports exceptions])
  fi
])

# OLN_ENABLE_EXCEPTIONS([yes|no])
# OLN_DISABLE_EXCEPTIONS

# Add -DOLN_EXCEPTIONS to OLN_CXXFLAGS when the flag --enable-oln-exceptions 
# is given to configure and exceptions are available.

AC_DEFUN([OLN_ENABLE_EXCEPTIONS],
[dnl
  AC_REQUIRE([AC_CXX_EXCEPTIONS])
  oln_exn_default=m4_ifselse([$1], , [yes], [$1])

  AC_CACHE_CHECK([whether to use exceptions instead of assertions],
            [oln_cv_use_exceptions],
            [AC_ARG_ENABLE([oln-exceptions],
                           [AC_HELP_STRING([--enable-oln-exceptions], 
                              [enable generation of C++ exceptions instead of aborts on error (default is $oln_exn_default)])],
                           [oln_cv_use_exceptions=$enableval], 
                           [oln_cv_use_exceptions=$oln_exn_default])])

  if test x$oln_cv_use_exceptions != xno; then
     if test x$ac_cv_cxx_exceptions != xno; then
        CPPFLAGS="$CPPFLAGS -DOLN_EXCEPTIONS"
     else
        AC_MSG_WARN([exceptions disabled in C++, cannot use Olena exceptions])
     fi
  fi

])
AC_DEFUN([OLN_DISABLE_EXCEPTIONS], [OLN_ENABLE_EXCEPTIONS([no])])

# OLN_NUMERIC_LIMITS

# Checks for the availability of std::numeric_limits::infinity()
# from C++.

# This tests adds -DOLN_USE_C_LIMITS to OLN_CPPFLAGS if the numeric
# limits are unavailable, in which case HUGE_VAL and HUGE_VALF are
# used instead by Olena.

AC_DEFUN([AC_CXX_NUMERIC_LIMITS],
[dnl
  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])

  ### Olena needs definitions for infinity values for the `float' and
  ### `double' data types. 

  # In ISO C++98, these are provided by std::numeric_limits, declared
  # in header <limits>. 

  AC_CHECK_HEADERS([limits])

  # Some old (and faulty) <limits> implementations define unusable 
  # std::numeric_limits. Check it.

  AC_CACHE_CHECK([for std::numeric_limits::infinity in <limits>],
                 [ac_cv_have_limits_infinity], 
	         [if test x$ac_cv_header_limits != xno; then
                     AC_TRY_LINK([@%:@include <limits>],
	        		 [float f1 = 
				     std::numeric_limits<float>::infinity();
		                  double f2 = 
				     std::numeric_limits<double>::infinity();],
                		 [ac_cv_have_limits_infinity=yes],
                		 [ac_cv_have_limits_infinity=no])
                  else
                     ac_cv_have_limits_infinity=no
                  fi])


  if test x$ac_cv_have_limits_infinity = xno; then
     # Usable std::numeric_limits were *not* found in <limits>.
     CPPFLAGS="-DUSE_C_LIMITS $CPPFLAGS"
  fi

  AC_LANG_POP([C++])
])

# OLN_MATH_FUNC([FUNCTION], [MACRO_NAME], [TEST])

# Checks for the availability of a particular math function
# from C++.

# This test attempts to use the function without flags
# at first, then with -D_ISOC99_SOURCE which is known to
# activate C99 declarations in the GNU libc headers.
# If the latter works, the flag is added to OLN_CPPFLAGS.
# In the default case, it adds -DOLN_NEED_XXX to
# OLN_CPPFLAGS, hoping that oln/config/math.hh will provide
# an implementation.

AC_DEFUN([AC_CXX_CHECK_MATH],
[dnl
  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])
  AC_CACHE_CHECK([for flags to enable $1() from C++],
                 [oln_cv_$1_flags],
                 [oln_save_CPPFLAGS=$CPPFLAGS
                  AC_LINK_IFELSE([@%:@include <cmath>
                                  int main() { $3 }],
                                 [oln_cv_$1_flags=unneeded],
                                 [CPPFLAGS="$CPPFLAGS -D_ISOC99_SOURCE=1"
                                  AC_LINK_IFELSE([@%:@include <cmath>
                                                  int main() { $3 }],
                                       [oln_cv_$1_flags=isodef],
                                       [oln_cv_$1_flags=redef])])
                  CPPFLAGS=$oln_save_CPPFLAGS])
  if test "x$[]oln_cv_$1_flags" = xredef; then
     CPPFLAGS="$CPPFLAGS -DNEED_$2"
  elif test "x$[]oln_cv_$1_flags" = xisodef; then
     CPPFLAGS="$CPPFLAGS -D_ISOC99_SOURCE=1"
  fi
  AC_LANG_POP([C++])
])  

AC_DEFUN([AC_CXX_FLOAT_MATH],
[dnl
  AC_CXX_CHECK_MATH([sqrtf], [SQRTF], [float f = sqrtf(0.1f);])
  AC_CXX_CHECK_MATH([floorf], [FLOORF], [float f = floorf(0.1f);])
  AC_CXX_CHECK_MATH([round], [ROUND], [double f = round(0.1);])
  AC_CXX_CHECK_MATH([roundf], [ROUNDF], [float f = roundf(0.1f);])
])

# OLN_WARN_CXXFLAGS

# Check that the variable CXXFLAGS does not contain debug or
# optimization flags

AC_DEFUN([OLN_WARN_CXXFLAGS],
[dnl
  oln_cxxflags_clean=yes

  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])dnl force initialization of default CXXFLAGS
  AC_MSG_CHECKING([for debbuging options in CXXFLAGS ($CXXFLAGS)])
  if echo "$CXXFLAGS" | grep -- "-g" >/dev/null 2>&1; then
     AC_MSG_RESULT([yes])
     AC_MSG_WARN([CXXFLAGS seems to contain debugging options, which is known to conflict with Olena optimizations.])
     AC_MSG_NOTICE([please cleanup CXXFLAGS and use CXXFLAGS_DEBUG instead for your own options.])
     oln_cxxflags_clean=no
  else
     AC_MSG_RESULT([no, good])
  fi
  AC_MSG_CHECKING([for optimization options in CXXFLAGS ($CXXFLAGS)])
  if echo "$CXXFLAGS" | grep -- "-O" >/dev/null 2>&1; then
     AC_MSG_RESULT([yes])
     AC_MSG_WARN([CXXFLAGS seems to contain optimization options, while Olena provides its own optimization flags.])
     AC_MSG_NOTICE([please cleanup CXXFLAGS and use CXXFLAGS_OPTIMIZE instead for your own options.])
     oln_cxxflags_clean=no
  else
     AC_MSG_RESULT([no, good])
  fi
  AC_LANG_POP([C++])
])

# AC_WITH_OLN

# Invoke configuration code to test for Olena and set a collection
# of appropriate flags.

AC_DEFUN([AC_WITH_OLN],
[dnl
  AC_REQUIRE([AC_CXX_TEMPLATE_DEPTH])
  AC_REQUIRE([AC_CXX_NUMERIC_LIMITS])
  AC_REQUIRE([AC_CXX_FLOAT_MATH])
  AC_REQUIRE([OLN_PATH_HEADERS])
  AC_REQUIRE([OLN_WARN_CXXFLAGS])
])

# AC_CXX_FLAGS

# Attempts to recognize specific compilers to set, if availables, extra
# flags for debugging, optimization and strict conformance to language
# specifications.

# This macro checks for the following compilers :
#   - GNU C++ (g++)
#   - Sun WorkShop C++ (Sun/CC)
#   - Intel C++ (icc)
#   - Comeau C++ (como)
# and sets the following autoconf variables:
#   CXXFLAGS_DEBUG
#   CXXFLAGS_STRICT
#   CXXFLAGS_STRICT_ERRORS
#   CXXFLAGS_OPTIMIZE

AC_DEFUN([AC_CXX_FLAGS],
[dnl
   AC_REQUIRE([AC_PROG_CXX])
   AC_LANG_PUSH([C++])
   AC_CACHE_CHECK([for C++ compiler-specific extra flags],
                  [ac_cv_cxx_style],
                  [ac_cv_cxx_style=unknown
                   if test "x$ac_compiler_gnu" != xno; then
		      if $CXX --version | grep '^2\.' >/dev/null ; then
			ac_cv_cxx_style=weakGNU
                      else
                        ac_cv_cxx_style=GNU
                      fi
                   elif $CXX -V 2>&1 | grep -i "WorkShop">/dev/null 2>&1; then 
		      ac_cv_cxx_style=Sun
                   elif $CXX -V 2>&1 | grep -i "Intel(R) C++">/dev/null 2>&1;
                   then
                      ac_cv_cxx_style=Intel
                   else
                      echo "int main() {}" >conftest.cc
                      if $CXX --version conftest.cc 2>&1 \
		         | grep -i "Comeau C/C++" >/dev/null 2>&1; then       
                         ac_cv_cxx_style=Comeau
		      fi
                      rm -f conftest.*
                   fi])
   AC_LANG_POP([C++])

   case "$ac_cv_cxx_style" in
     GNU)
      _CXXFLAGS_DEBUG="-g"
      _CXXFLAGS_OPTIMIZE="-O3 -finline-limit-1500"
      _CXXFLAGS_STRICT="-W -Wall -pedantic"
      _CXXFLAGS_STRICT_ERRORS="-W -Wall -pedantic -Werror"
      ;;
     weakGNU)
      _CXXFLAGS_DEBUG="-g"
      _CXXFLAGS_OPTIMIZE="-O2 -felide-constructors -funroll-loops"
      _CXXFLAGS_STRICT="-W -Wall -pedantic"
      _CXXFLAGS_STRICT_ERRORS="-W -Wall -pedantic -Werror"
      ;;
     Sun)
      _CXXFLAGS_DEBUG="-g"
      _CXXFLAGS_OPTIMIZE="-fast"
      _CXXFLAGS_STRICT="-v"
      _CXXFLAGS_STRICT_ERRORS="-v -xwe"
      ;;
     Comeau)
      _CXXFLAGS_DEBUG="-g"
      _CXXFLAGS_STRICT="-r"
      _CXXFLAGS_STRICT_ERRORS="-r"
      ;;
     Intel)
      _CXXFLAGS_OPTIMIZE="-O3"
      _CXXFLAGS_DEBUG="-g"
      _CXXFLAGS_STRICT="-w1 -Wall"
      _CXXFLAGS_STRICT_ERRORS="-w1 -Wall"
      ;;
   esac

   if test "x$CXXFLAGS_OPTIMIZE" = "x"; then
	CXXFLAGS_OPTIMIZE=$_CXXFLAGS_OPTIMIZE
   fi
   if test "x$CXXFLAGS_DEBUG" = "x"; then
	CXXFLAGS_DEBUG=$_CXXFLAGS_DEBUG
   fi
   if test "x$CXXFLAGS_STRICT" = "x"; then
	CXXFLAGS_STRICT=$_CXXFLAGS_STRICT
   fi
   if test "x$CXXFLAGS_STRICT_ERRORS" = "x"; then
	CXXFLAGS_STRICT_ERRORS=$_CXXFLAGS_STRICT_ERRORS
   fi

   AC_SUBST([CXXFLAGS_DEBUG])
   AC_SUBST([CXXFLAGS_OPTIMIZE])
   AC_SUBST([CXXFLAGS_STRICT])
   AC_SUBST([CXXFLAGS_STRICT_ERRORS])
])

# AC_WITH_CXX_FFTW

# Checks for availability of fftw from C++ programs.

# This macro sets FFTW_CXXFLAGS and FFTW_LDFLAGS if the library is
# found and its functions available from C++.

AC_DEFUN([AC_WITH_CXX_FFTW],
[dnl
 AC_REQUIRE([AC_PROG_CXX])
 AC_LANG_PUSH([C++])

 AC_ARG_WITH([fftw],
      [AC_HELP_STRING([--with-fftw@<:@=DIR@:>@], 
                      [using fftw (DIR = prefix for fftw installation)])])
 FFTW_CXXFLAGS=''
 FFTW_LDFLAGS=''
 if test "x$with_fftw" != xno; then
   if test -n "$with_fftw"; then
     FFTW_CXXFLAGS="-I${with_fftw}/include"
     FFTW_LDFLAGS="-L${with_fftw}/lib"
   fi
   oln_save_CXXFLAGS=$CXXFLAGS
   oln_save_LDFLAGS=$LDFLAGS
   CXXFLAGS="$CXXFLAGS $FFTW_CXXFLAGS"
   LDFLAGS="$LDFLAGS $FFTW_LDFLAGS"
   oln_have_fftw=no
   AC_CHECK_HEADER([fftw.h],
                 [AC_CHECK_LIB([fftw], 
                               [fftw2d_create_plan],
                               [oln_have_fftw=yes
                                FFTW_LDFLAGS="$FFTW_LDFLAGS -lfftw -lrfftw"
                                AC_DEFINE([HAVE_FFTW], 1, 
                                          [Define to 1 if we can use fftw])])])
   CXXFLAGS=$oln_save_CXXFLAGS
   LDFLAGS=$oln_save_LDFLAGS
 fi
 AC_SUBST([FFTW_CXXFLAGS])
 AC_SUBST([FFTW_LDFLAGS])

 AC_LANG_POP([C++])
])

# AC_WITH_CXX_ZLIB

# Checks for availability of Zlib from C++ programs.

# This macro sets ZLIB_CXXFLAGS and ZLIB_LDFLAGS if the library is
# found and its functions available from C++.

AC_DEFUN([AC_WITH_CXX_ZLIB],
[dnl
 AC_REQUIRE([AC_PROG_CXX])
 AC_LANG_PUSH([C++])

 AC_ARG_WITH([zlib],
             [AC_HELP_STRING([--with-zlib@<:@=DIR@:>@], 
                    [using zlib (DIR = prefix for zlib installation)])])
 ZLIB_CXXFLAGS=''
 ZLIB_LDFLAGS=''
 if test "x$with_zlib" != xno; then
   if test -n "$with_zlib"; then
     ZLIB_CXXFLAGS="-I${with_zlib}/include"
     ZLIB_LDFLAGS="-L${with_zlib}/lib"
   fi
   oln_save_CXXFLAGS=$CXXFLAGS
   oln_save_LDFLAGS=$LDFLAGS
   CXXFLAGS="$CXXFLAGS $ZLIB_CXXFLAGS"
   LDFLAGS="$LDFLAGS $ZLIB_LDFLAGS"
   oln_have_zlib=no
   AC_CHECK_HEADER([zlib.h],
                   [AC_CHECK_LIB([z], 
                               [gzopen],
                               [oln_have_zlib=yes
                                ZLIB_LDFLAGS="$ZLIB_LDFLAGS -lz"
                                AC_DEFINE([HAVE_ZLIB], 1, 
                                          [Define to 1 if we can use zlib])])])
   CXXFLAGS=$oln_save_CXXFLAGS
   LDFLAGS=$oln_save_LDFLAGS
 fi
 AC_SUBST([ZLIB_CXXFLAGS])
 AC_SUBST([ZLIB_LDFLAGS])

 AC_LANG_POP([C++])
])

###
### Internal stuff for Olena
###
# $Format: "m4_define([OLN_VERSION], [0.$ProjectMajorVersion$])"$
m4_define([OLN_VERSION], [0.2003paradigm])
m4_define([OLN_VERSION], [0.8])
m4_define([OLN_CONTACT], [olena-bugs@lrde.epita.fr])

# OLN_COLLECTION([subdirs-variable-name],
#                [component-list])

AC_DEFUN([OLN_COLLECTION], [dnl
$1_SUBDIRS=""
AC_SUBST([$1_SUBDIRS])
$2
])

# OLN_COMPONENT([DIRNAME], [WITHNAME], [VARNAME], [DESCRIPTION],
#               [SUBDIRS-VARIABLE-NAME],
#               [CONFIG-FILES],
#               [EXTRA-ACTION-IF-SELECTED], [DIRNAME-FOR-SUBDIRS-IF-DIFFERENT-THAN-FIRST-DIRNAME])

# This macro factorizes a pattern that occurs frequently in Olena
# configuration files : the concept of optional directory associated
# with a component elidable from the distribution.

# A component is defined by the following characteristics :

#   * DIRNAME: the elidable directory, relative to the current
#   configuration directory,

#   * WITHNAME: the name of the flag the user can use to exclude the
#   component (--without-WITHNAME),

#   * VARNAME: the name of the cached Autoconf variable representing
#   the inclusion status of this component (usually oln_cv_DIRNAME),

#   * DESCRIPTION: the name of the component, as mentioned in help
#   messages from the "configure" script,

#   * SUBDIRS-VARIABLE-NAME: the name of the variable that contains
#   (or not) mention to DIRNAME and is AC_SUBST'ituted in Makefiles
#   for use by $SUBDIRS,

#   * CONFIG-FILES (optional): the name of files added to the list of
#   generated files when the component is included,

#   * EXTRA-ACTION-IF-SELECTED (optional): extra test code executed
#   only when the component is included,

#   * DIRNAME-FOR-SUBDIRS-IF-DIFFERENT-THAN-FIRST-DIRNAME (optional):
#   if set, the name of the subdirectory inserted in
#   $SUBDIRS-VARIABLE-NAME if different than DIRNAME (example in
#   tools/configure.ac)

AC_DEFUN([OLN_COMPONENT], [dnl
AC_CACHE_CHECK([whether to build $4],
               [$3],
               [$3=no
                if test -r "$srcdir/$1/Makefile.in"; then
                   AC_ARG_WITH([$2],
                               [AC_HELP_STRING([--without-$2], [disable building $4])],
                               [$3=$withval],
                               [$3=yes])
                fi])
if test x$[]$3 != xno; then
   $7
   if test x$[]$3 != xno; then
     ifelse([$6], [], [], [AC_CONFIG_FILES([$6])])
     $5_SUBDIRS="$[]$5_SUBDIRS ifelse([$8], [], [$1], [$8])"
   fi
fi
])

