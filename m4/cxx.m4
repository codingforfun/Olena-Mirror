###
### Stuff for header files
###

# AC_CXX_TEMPLATE_DEPTH([MINIMUM-DEPTH])

# Check for deep template recursion upto MINIMUM-DEPTH.
# ac_cv_cxx_template_flags holds the final flags which should be used.

# Detect best template flags when :
# - deep template recursion is not available when it is not present
# - the compiler supports it 
# - it provides the right effect when present

AC_DEFUN([AC_CXX_TEMPLATE_DEPTH],
[dnl
  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])

  cxx_tdepth=ifelse([$2], , 50, [$2])
  AC_CACHE_CHECK([for C++ template recursion upto $cxx_tdepth levels],
                 [ac_cv_cxx_template_check],
                 [ac_cv_cxx_template_check=direct
                  cxx_save_CPPFLAGS=$CPPFLAGS
                  cxx_save_CXXFLAGS=$CXXFLAGS
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
                       [ac_cv_cxx_template_check="-ftemplate-depth-$cxx_tdepth"], 
                       [ac_cv_cxx_template_check=unsupported])])
                  CPPFLAGS=$cxx_save_CPPFLAGS
                  CXXFLAGS=$cxx_save_CXXFLAGS])


  AC_LANG_POP([C++])

  ac_cv_cxx_template_flags=""
  case "$ac_cv_cxx_template_check" in
     unsupported )
       AC_MSG_WARN([C++ compiler does not handle template recursion upto $cxx_tdepth. Expect problems.])
       ;;
     direct ) ;;
     * ) 
       ac_cv_cxx_template_flags=$ac_cv_cxx_template_check
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

# AC_CXX_NUMERIC_LIMITS

# Checks for the availability of std::numeric_limits::infinity()
# from C++.

# This tests adds -DOLN_USE_C_LIMITS to OLN_CPPFLAGS if the numeric
# limits are unavailable, in which case HUGE_VAL and HUGE_VALF are
# used instead by Olena.

AC_DEFUN([AC_CXX_NUMERIC_LIMITS],
[dnl
  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])

  ### Check for definitions of infinity values. 

  # In ISO C++98, these are provided by std::numeric_limits, declared
  # in header <limits>. 

  AC_CHECK_HEADERS([limits])

  # Some old (and faulty) <limits> implementations define unusable 
  # std::numeric_limits. Check it.

  AC_CACHE_CHECK([for std::numeric_limits::infinity in <limits>],
                 [ac_cv_cxx_have_limits_infinity], 
	         [if test x$ac_cv_header_limits != xno; then
                     AC_TRY_LINK([@%:@include <limits>],
	        		 [float f1 = 
				     std::numeric_limits<float>::infinity();
		                  double f2 = 
				     std::numeric_limits<double>::infinity();],
                		 [ac_cv_cxx_have_limits_infinity=yes],
                		 [ac_cv_cxx_have_limits_infinity=no])
                  else
                     ac_cv_cxx_have_limits_infinity=no
                  fi])

  AC_LANG_POP([C++])
])

# AC_CXX_MATH_FUNC(VARIABLE, TEST)

# Checks for the availability of a particular math function
# from C++.

# This test attempts to use the function without flags
# at first, then with -D_ISOC99_SOURCE which is known to
# activate C99 declarations in the GNU libc headers.
# If the latter works, the flag is put to variable ac_cv_cxx_FUNCTION_flags.

AC_DEFUN([AC_CXX_MATH_FUNC],
[dnl
  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])
  ac_cv_cxx_$1_flags=""
  AC_CACHE_CHECK([for flags to enable $1() from C++],
                 [ac_cv_cxx_$1_check],
                 [cxx_save_CPPFLAGS=$CPPFLAGS
		  AC_LINK_IFELSE([@%:@include <cmath>
                                  int main() { $3 }],
                                 [ac_cv_$1_check="unneeded"],
                                 [CPPFLAGS="$CPPFLAGS -D_ISOC99_SOURCE=1"
                                  AC_LINK_IFELSE([@%:@include <cmath>
                                                  int main() { $3 }],
                                       [ac_cv_cxx_$1_check="isodef"
					ac_cv_cxx_$1_flags="-D_ISOC99_SOURCE=1"],
                                       [ac_cv_cxx_$1_check="no"
					ac_cv_cxx_$1_flags="no"])])
		  CPPFLAGS=$cxx_save_CPPFLAGS
		 ])
 
  AC_LANG_POP([C++])
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
      _CXXFLAGS_DEBUG="-g"
      _CXXFLAGS_OPTIMIZE="-O3"
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
