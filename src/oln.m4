
AC_DEFUN(AM_PATH_OLN,
      [dnl 
dnl Get the cflags and libraries from the oln-config script
dnl
AC_ARG_WITH(oln-include,[  --with-oln-include=PFX   Prefix where OLN is installed (optional)],
            oln_include_prefix="$withval", oln_include_prefix="")

  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])

  OLN_INCLUDEDIR=""
  ac_save_CXXFLAGS="$CXXFLAGS"
  ac_save_CPPFLAGS="$CPPFLAGS"

  AC_MSG_NOTICE([checking for Olena headers])

  within_olena_source_tree=ifelse([$1], [], [no], [$1])
  if test x$within_olena_source_tree != xno; then
     AC_MSG_NOTICE([Olena seems to lay around...])
     within_olena_source_tree=`( cd $srcdir; cd $within_olena_source_tree; pwd )`
     OLN_INCLUDEDIR="${within_olena_source_tree}"
     CXXFLAGS="-I${within_olena_source_tree}"
     CPPFLAGS="-I${within_olena_source_tree}"
  fi

  if test x$oln_include_prefix != x ; then
     OLN_INCLUDEDIR="$oln_include_prefix"
     CXXFLAGS="-I$oln_include_prefix $CXXFLAGS"
     CPPFLAGS="-I$oln_include_prefix $CPPFLAGS"
  fi

  have_olena=yes
  AC_CHECK_HEADER([oln/basics.hh], [], [have_olena=no])

  if test $have_olena = yes; then
     if test x$OLN_INCLUDEDIR != x; then
        AC_MSG_NOTICE([Olena was found in $OLN_INCLUDEDIR, good.])
     else
        AC_MSG_RESULT([Olena was found, good.])
     fi
  else
     AC_MSG_WARN([*** Olena source headers not found. Expect problems.])
     CXXFLAGS="$ac_save_CXXFLAGS"
     CPPFLAGS="$ac_save_CPPFLAGS"
  fi   

  AC_SUBST([OLN_INCLUDEDIR])
  
  AC_LANG_POP([C++])
])

AC_DEFUN(AC_WITH_OLN,
[
  AC_REQUIRE([AM_PATH_OLN])

  AC_LANG_PUSH([C++])
  AC_CHECK_HEADERS([limits stl_config.h])
  AC_LANG_POP([C++])
])

AC_DEFUN(AC_WITH_FFTW,
[
   AC_ARG_WITH(fftw,
      [AC_HELP_STRING([--with-fft[=DIR]], [using fftw (DIR = prefix for fftw installation)])])
   FFTW_CFLAGS=''
   FFTW_LDFLAGS=''
   if test "x$with_fftw" != xno; then
      if test -n "$with_fftw"; then
        FFTW_CFLAGS="-I${with_fftw}/include"
        FFTW_LDFLAGS="-L${with_fftw}/lib"
      fi
      oln_save_CFLAGS=$CFLAGS
      oln_save_LDFLAGS=$LDFLAGS
      CFLAGS="$CFLAGS $FFTW_CFLAGS"
      LDFLAGS="$LDFLAGS $FFTW_LDFLAGS"
      AC_CHECK_HEADER([fftw.h],
           [AC_CHECK_LIB([fftw], [fftw2d_create_plan],
                [FFTW_LDFLAGS="${FFTW_LDFLAGS} -lfftw -lrfftw"
                 AC_DEFINE([HAVE_FFTW], 1, [Define to 1 if we can use fftw])
                ])
           ])
      CFLAGS=$oln_save_CFLAGS
      LDFLAGS=$oln_save_LDFLAGS
  fi
  AC_SUBST([FFTW_CFLAGS])
  AC_SUBST([FFTW_LDFLAGS])
])


AC_DEFUN(AC_WITH_ZLIB,
[
   AC_ARG_WITH(zlib,
      [AC_HELP_STRING([--with-zlib[=DIR]], [using zlib (DIR = prefix for zlib installation)])])
   ZLIB_CFLAGS=''
   ZLIB_LDFLAGS=''
   if test "x$with_zlib" != xno; then
      if test -n "$with_zlib"; then
        ZLIB_CFLAGS="-I${with_zlib}/include"
        ZLIB_LDFLAGS="-L${with_zlib}/lib"
      fi
      oln_save_CFLAGS=$CFLAGS
      oln_save_LDFLAGS=$LDFLAGS
      CFLAGS="$CFLAGS $ZLIB_CFLAGS"
      LDFLAGS="$LDFLAGS $ZLIB_LDFLAGS"
      AC_CHECK_HEADER([zlib.h],
       [AC_CHECK_LIB([z], [gzopen],
          [ZLIB_LDFLAGS="${ZLIB_LDFLAGS} -lz"
            AC_DEFINE([HAVE_ZLIB], 1, [Define to 1 if we can use zlib])])])
      CFLAGS=$oln_save_CFLAGS
      LDFLAGS=$oln_save_LDFLAGS
   fi
   AC_SUBST([ZLIB_CFLAGS])
   AC_SUBST([ZLIB_LDFLAGS])
])

