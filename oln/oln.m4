AC_DEFUN(OLN_PATH_LOCAL, 
[dnl
  advertised_oln_dir=ifelse([$1], [], [no], [$1])
  AC_CACHE_CHECK([whether Olena resides around us],
                 [oln_cv_local_src],
		 [oln_cv_local_src=no
                  if test "x$advertised_oln_dir" != xno; then
                     oln_dir=`pwd`
                     advertised_oln_hdir=`cd "$srcdir" && cd "$advertised_oln_dir" && pwd`
                     cd "$oln_dir"
                     if test -r "$advertised_oln_hdir/oln/config/pconf.hh"; then
                        oln_cv_local_src="$advertised_oln_hdir"
                     fi
                  fi])
  if test "x$oln_cv_local_src" != xno; then
     AC_CACHE_CHECK([for auto-generated pconf.inc],
                    [oln_cv_build_src],
                    [oln_cv_build_src=no
                     if test -r "$oln_cv_local_src/oln/config/pconf.inc"; then
                        oln_cv_build_src="$oln_cv_local_src"
                     else
                        oln_dir=`pwd`
                        advertised_oln_idir=`cd "$advertised_oln_dir" && pwd`
                        cd "$oln_dir"
                        if test -r "$advertised_oln_idir/oln/config/pconf.inc"; then
                           oln_cv_build_src="$advertised_oln_idir"
                        fi
                     fi])
  fi
])

AC_DEFUN(OLN_PATH_USER, 
[dnl
  AC_CACHE_CHECK([for Olena in user-specified directory],
		 [oln_cv_user_hint], 
                 [oln_cv_user_hint=no
                  AC_ARG_WITH([oln],
                              [AC_HELP_STRING([--with-oln=DIR], 
                                              [Include directory where Olena is installed (optional)])],
                              [if test -r "$withval/oln/config/pconf.hh"; then
				  oln_cv_user_hint="$withval"
			       fi])])
])

AC_DEFUN(_OLN_CHECK, 
[dnl
  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])

  have_olena=yes
  oln_save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$OLN_CPPFLAGS $CPPFLAGS"
  AC_CHECK_HEADER([oln/config/pconf.hh], [], [have_olena=no])
  CPPFLAGS="$oln_save_CPPFLAGS"

  if test $have_olena = yes; then
     if test x$OLN_INCLUDE_DIR != x; then
        AC_MSG_NOTICE([using include path $OLN_INCLUDE_DIR to find Olena.])
     else

	# Here Olena was found in the standard search path.
	# FIXME: to forcibly compute OLN_INCLUDE_DIR, we rely on it being defined
	# in the installed headers. This leads to a bug if the headers are moved
        # after being installed or installed with a DESTDIR different
        # than what was computed during a run of `configure'.

        AC_MSG_CHECKING([for location of Olena headers])

        OLN_INCLUDE_DIR=no
        AC_TRY_RUN([@%:@include <oln/config/pconf.hh>
		    @%:@include <fstream>
		    int main(void) { std::ofstream f("conftest.oln"); f << OLN_INCLUDE_DIR; return 0; }], 
		   [OLN_INCLUDE_DIR=`cat conftest.oln`], 
		   [], [])

        if test x$OLN_INCLUDE_DIR != xno; then
             if test -r $OLN_INCLUDE_DIR/oln/config/pconf.hh; then
                AC_MSG_RESULT([$OLN_INCLUDE_DIR])
             else
	        AC_MSG_RESULT([duh ?])
	        AC_MSG_WARN([Olena advertises $OLN_INCLUDE_DIR but $OLN_INCLUDE_DIR/oln/config/pconf.hh not found])
	     fi
        else 
            OLN_INCLUDE_DIR=''
            AC_MSG_RESULT([unknown. Expect problems.])
        fi
     fi
  else
     AC_MSG_WARN([Olena source headers not found. Expect problems.])
  fi   

  AC_SUBST([OLN_INCLUDE_DIR])

  AC_LANG_POP([C++])
])

AC_DEFUN(OLN_PATH_HEADERS,
[dnl 
  AC_REQUIRE([OLN_PATH_LOCAL])
  AC_REQUIRE([OLN_PATH_USER])

  if test "x$oln_cv_local_src" != xno; then
     OLN_INCLUDE_DIR="$oln_cv_local_src"
     if test "x$oln_cv_build_src" != xno -a "x$oln_cv_local_src" != "x$oln_cv_build_src"; then
        OLN_EXTRA_CPPFLAGS="-I$oln_cv_build_src"
     fi
  fi

  # User-specified directory overrides any previous definition
  if test "x$oln_cv_user_hint" != xno; then
     if test "x$OLN_INCLUDE_DIR" != x ; then
       AC_MSG_WARN([using $oln_cv_user_hint instead of $OLN_INCLUDE_DIR])
     fi
     OLN_INCLUDE_DIR="$oln_cv_user_hint"
  fi

  AC_ARG_VAR([OLN_INCLUDE_DIR], [location of Olena (<include dir>, should be autodetected)])
  if test "x$OLN_INCLUDE_DIR" != x ; then
     OLN_CPPFLAGS="-I$OLN_INCLUDE_DIR $OLN_EXTRA_CPPFLAGS $OLN_CPPFLAGS"
  fi
  AC_SUBST([OLN_CPPFLAGS])

  _OLN_CHECK
])

AC_DEFUN(OLN_PATH_IMGS,
[dnl
  AC_REQUIRE([OLN_PATH_LOCAL])

  olndatadir=`eval echo $datadir/oln`
  AC_ARG_VAR([OLN_IMG_DIR], [location of Olena sample images (should be autodetected)])

  AC_CACHE_CHECK([for Olena sample images],
                 [oln_cv_img_dir],
                 [oln_cv_img_dir=no
                  AC_ARG_WITH([oln-imgs],
	                      [AC_HELP_STRING([--with-oln-imgs=DIR],
			                      [location of Olena sample images (defaults to $olndatadir)])],
                              [oln_img_dir="$withval"],
                              [oln_img_dir=""])
                  for d in "$OLN_IMG_DIR" "$oln_cv_local_src/img" "$oln_img_dir" "$olndatadir"; do
                    if test -d "$d"; then
                       if test -r "$d/lena.ppm"; then
                          oln_cv_img_dir="$d"
                          break
                       fi
                    fi
                  done])
  if test "x$oln_cv_img_dir" != xno ; then
    OLN_IMG_DIR="$oln_cv_img_dir"

    ### Check for location of autogenerated images.
    # FIXME: this check is incomplete since the images are not available at the
    # time configure is run.

    AC_CACHE_CHECK([for Olena autogenerated sample images],
                   [oln_cv_img_aux_dir],
                   [oln_cv_img_aux_dir=no
                    AC_ARG_WITH([oln-aux-imgs],
                                [AC_HELP_STRING([--with-oln-aux-imgs=DIR],
                                                [location of derivate Olena images (defaults to $olndatadir)])],
                                [oln_img_aux_dir="$withval"],
                                [oln_img_aux_dir=""])
                    for d in "$oln_cv_build_src/img" "$oln_img_aux_dir" "$oln_cv_img_dir" "$olndatadir"; do
                        if test -d "$d"; then
                           oln_cv_img_aux_dir="$d"
                           break 
                        fi
                    done])
     if test "x$oln_cv_img_aux_dir" != xno; then
        OLN_IMG_AUX_DIR="$oln_cv_img_aux_dir"
     else
        AC_MSG_WARN([Olena derivate images not found. Expect problems.])
     fi
  else
    AC_MSG_WARN([Olena images not found. Expect problems.])
  fi
  AC_SUBST([OLN_IMG_DIR])  
  AC_SUBST([OLN_IMG_AUX_DIR])  
])

AC_DEFUN(OLN_TEMPLATE_DEPTH,
[dnl
  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])

  cxx_tdepth=ifelse([$1], , 50, [$1])
  AC_CACHE_CHECK([for automatic C++ template depth adjustment by the compiler, upto $cxx_tdepth],
                 [oln_cv_template_flags],
                 [oln_cv_template_flags=direct
                  oln_save_CPPFLAGS="$CPPFLAGS"
                  oln_save_CXXFLAGS="$CXXFLAGS"
                  CPPFLAGS="$CPPFLAGS $OLN_CPPFLAGS -DTDEPTH=$cxx_tdepth"
                  CXXFLAGS="$CXXFLAGS $OLN_CXXFLAGS"
                  AC_LINK_IFELSE([template<unsigned n> struct rec { typedef typename rec<n-1>::ret ret; };
                                  template<> struct rec<0> { typedef int ret; };
                                  int main(void) { rec<TDEPTH>::ret i = 0; return i; }], 
                                 [], 
                                 [CXXFLAGS="$CXXFLAGS -ftemplate-depth-$cxx_tdepth"
                                  AC_LINK_IFELSE([template<unsigned n> struct rec { typedef typename rec<n-1>::ret ret; };
                                                  template<> struct rec<0> { typedef int ret; };
                                                  int main(void) { rec<TDEPTH>::ret i = 0; return i; }],
                                                 [oln_cv_template_flags="-ftemplate-depth-$cxx_tdepth"], 
                                                 [oln_cv_template_flags=unsupported])])
                  CPPFLAGS="$oln_save_CPPFLAGS"
                  CXXFLAGS="$oln_save_CXXFLAGS"])


  AC_LANG_POP([C++])

  case "$oln_cv_template_flags" in
     unsupported|direct ) ;;
     * )
       OLN_CXXFLAGS="$OLN_CXXFLAGS $oln_cv_template_flags"
       ;;
  esac

  AC_SUBST([OLN_CXXFLAGS])
])

AC_DEFUN(OLN_ENABLE_EXCEPTIONS,
[dnl
  oln_exn_default=m4_ifselse([$1], , [yes], [$1])

  AC_CACHE_CHECK([whether to use exceptions instead of assertions],
            [oln_cv_use_exceptions],
            [AC_ARG_ENABLE([oln-exceptions],
                           [AC_HELP_STRING([--enable-oln-exceptions], 
                              [enable generation of C++ exceptions instead of aborts on error (default is $oln_exn_default)])],
                           [oln_cv_use_exceptions=$enableval], 
                           [oln_cv_use_exceptions=$oln_exn_default])])

  if test x$oln_cv_use_exceptions != xno; then
     OLN_CPPFLAGS="$OLN_CPPFLAGS -DOLN_EXCEPTIONS"
  fi

  AC_SUBST([OLN_CPPFLAGS])
])
AC_DEFUN(OLN_DISABLE_EXCEPTIONS, [OLN_ENABLE_EXCEPTIONS([no])])

AC_DEFUN(OLN_NUMERIC_LIMITS,
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
	        		 [float f1 = std::numeric_limits<float>::infinity();
		                  double f2 = std::numeric_limits<double>::infinity();],
                		 [ac_cv_have_limits_infinity=yes],
                		 [ac_cv_have_limits_infinity=no])
                  else
                     ac_cv_have_limits_infinity=no
                  fi])


  if test x$ac_cv_have_limits_infinity != xno; then
  
  # Usable std::numeric_limits were found in <limits>.
  # Use them to define constants OLN_*_INFINITY.

     AC_DEFINE(OLN_FLOAT_INFINITY, [(std::numeric_limits<float>::infinity())],
	       [the infinity value for floats])
     AC_DEFINE(OLN_DOUBLE_INFINITY, [(std::numeric_limits<double>::infinity())],
               [the infinity value for doubles])
  else

  # Usable std::numeric_limits were not found in <limits>.
  # Try to find a replacement using HUGE_VAL and (if it exists) HUGE_VALF.

     AC_CACHE_CHECK([for HUGE_VAL],
                    [ac_cv_have_huge_val], 
		    [AC_TRY_LINK([@%:@include <cmath>],
                     		 [double f1 = HUGE_VAL;], 
                     		 [ac_cv_have_huge_val=yes], 
                                 [ac_cv_have_huge_val=no])])

     if test x$ac_cv_have_huge_val != xno; then
         AC_DEFINE(OLN_DOUBLE_INFINITY, [HUGE_VAL], [the infinity value for doubles])
         AC_CACHE_CHECK([for HUGE_VALF],
                        [ac_cv_have_huge_valf], 
			[AC_TRY_LINK([@%:@include <cmath>],
                        	     [double f1 = HUGE_VALF;], 
                        	     [ac_cv_have_huge_valf=yes], 
                                     [ac_cv_have_huge_valf=no])])

          if test x$ac_cv_have_huge_valf != xno; then
              AC_DEFINE(OLN_FLOAT_INFINITY, [HUGE_VALF], [the infinity value for floats])
          else
              AC_DEFINE(OLN_FLOAT_INFINITY, [((float)HUGE_VAL)], [the infinity value for floats])
          fi
     else
        AC_MSG_WARN([don't know how to define infinity on this host. Expect problems.])
     fi
  fi

  AC_LANG_POP([C++])

])

AC_DEFUN(OLN_C99_MATH,
[dnl
  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])

  ### Check for various ISO C99 float functions.
  ### Olena noticeably uses roundf(3).

  AC_CACHE_CHECK([for flag to activate C99 float functions from C++],
                 [oln_cv_c99_flags],
                 [oln_save_CPPFLAGS="$CPPFLAGS"
 		  oln_save_CXXFLAGS="$CXXFLAGS"
		  CPPFLAGS="$OLN_CPPFLAGS $CPPFLAGS"
		  CXXFLAGS="$OLN_CXXFLAGS $CXXFLAGS"

                  ### The checks are done using AC_TRY_COMPILE instead of AC_TRY_FUNC,
                  ### because the latter uses invalid C++ syntax.
                  AC_TRY_COMPILE([@%:@include <cmath>],
                                 [float f1 = roundf(0f); float f2 = nearbyintf(0f);],  
                                 [oln_cv_c99_flags=unneeded],
                                 [# When using a ISO-compliant C++98 compiler with a C99 standard library,
		  		  # usually the C99 functions are not declared in C++ scope. 
		  		  # 
                  		  # However, with GNU systems, the _ISOC99_SOURCE macro conditional
		  		  # forces C99 declarations when set to 1. 
		  		  #
                                  CPPFLAGS="$CPPFLAGS -D_ISOC99_SOURCE=1"
                                  AC_TRY_COMPILE([@%:@include <cmath>],
                                                 [float f1 = roundf(0.); float f2 = nearbyintf(0.);],  
                                                 [oln_cv_c99_flags="-D_ISOC99_SOURCE=1"],
                                                 [oln_cv_c99_flags=unavailable])])
                  CPPFLAGS="$oln_save_CPPFLAGS"
                  CXXFLAGS="$oln_save_CXXFLAGS"])


  AC_LANG_POP([C++])

  case "$oln_cv_c99_flags" in 
     unavailable | unneeded )
        ;;
     * )
        OLN_CPPFLAGS="$OLN_CPPFLAGS $oln_cv_c99_flags"
        ;;
  esac

  AC_SUBST([OLN_CPPFLAGS])
])

AC_DEFUN(AC_WITH_OLN,
[dnl
  AC_REQUIRE([OLN_PATH_HEADERS])
  AC_REQUIRE([OLN_PATH_IMGS])
  AC_REQUIRE([OLN_TEMPLATE_DEPTH])
  AC_REQUIRE([OLN_NUMERIC_LIMITS])
  AC_REQUIRE([OLN_C99_MATH])

  AC_LANG_PUSH([C++])
  AC_CHECK_HEADERS([stl_config.h])
  AC_LANG_POP([C++])
])


AC_DEFUN(AC_WITH_CXX_FFTW,
[dnl
   AC_REQUIRE([AC_PROG_CXX])
   AC_LANG_PUSH([C++])

   AC_ARG_WITH([fftw],
      [AC_HELP_STRING([--with-fftw@<:@=DIR@:>@], [using fftw (DIR = prefix for fftw installation)])])
   FFTW_CXXFLAGS=''
   FFTW_LDFLAGS=''
   if test "x$with_fftw" != xno; then
      if test -n "$with_fftw"; then
        FFTW_CXXFLAGS="-I${with_fftw}/include"
        FFTW_LDFLAGS="-L${with_fftw}/lib"
      fi
      oln_save_CXXFLAGS="$CXXFLAGS"
      oln_save_LDFLAGS="$LDFLAGS"
      CXXFLAGS="$CXXFLAGS $FFTW_CXXFLAGS"
      LDFLAGS="$LDFLAGS $FFTW_LDFLAGS"
      oln_have_fftw=no
      AC_CHECK_HEADER([fftw.h],
                      [AC_CHECK_LIB([fftw], 
                                    [fftw2d_create_plan],
                                    [oln_have_fftw=yes
                                     FFTW_LDFLAGS="$FFTW_LDFLAGS -lfftw -lrfftw"
                                     AC_DEFINE([HAVE_FFTW], 1, [Define to 1 if we can use fftw])])])
      CXXFLAGS="$oln_save_CXXFLAGS"
      LDFLAGS="$oln_save_LDFLAGS"
  fi

  AC_SUBST([FFTW_CXXFLAGS])
  AC_SUBST([FFTW_LDFLAGS])

  AC_LANG_POP([C++])
])


AC_DEFUN(AC_WITH_CXX_ZLIB,
[dnl
   AC_REQUIRE([AC_PROG_CXX])
   AC_LANG_PUSH([C++])

   AC_ARG_WITH([zlib],
      [AC_HELP_STRING([--with-zlib@<:@=DIR@:>@], [using zlib (DIR = prefix for zlib installation)])])
   ZLIB_CXXFLAGS=''
   ZLIB_LDFLAGS=''
   if test "x$with_zlib" != xno; then
      if test -n "$with_zlib"; then
        ZLIB_CXXFLAGS="-I${with_zlib}/include"
        ZLIB_LDFLAGS="-L${with_zlib}/lib"
      fi
      oln_save_CXXFLAGS="$CXXFLAGS"
      oln_save_LDFLAGS="$LDFLAGS"
      CXXFLAGS="$CXXFLAGS $ZLIB_CXXFLAGS"
      LDFLAGS="$LDFLAGS $ZLIB_LDFLAGS"
      oln_have_zlib=no
      AC_CHECK_HEADER([zlib.h],
                      [AC_CHECK_LIB([z], 
                                    [gzopen],
                                    [oln_have_zlib=yes
                                     ZLIB_LDFLAGS="$ZLIB_LDFLAGS -lz"
                                     AC_DEFINE([HAVE_ZLIB], 1, [Define to 1 if we can use zlib])])])
      CXXFLAGS="$oln_save_CXXFLAGS"
      LDFLAGS="$oln_save_LDFLAGS"
   fi
   AC_SUBST([ZLIB_CXXFLAGS])
   AC_SUBST([ZLIB_LDFLAGS])

   AC_LANG_POP([C++])
])

AC_DEFUN([AC_WITH_CXX_WARNINGS], [dnl
   # FIXME: This is pretty poor at the moment...
   # FIXME: this is highly autoconf-internals dependent (undocumented variable used)
   AC_REQUIRE([AC_PROG_CXX])
   AC_MSG_CHECKING([whether we can enable g++-specific warning options to the compiler])
   if test "x$ac_cv_cxx_compiler_gnu" != xno; then
      AC_MSG_RESULT([yes])
      CXXFLAGS="$CXXFLAGS -W -Wall -pedantic"
   else
      AC_MSG_RESULT([no])
   fi
])

