
AC_DEFUN(AM_PATH_OLN,
      [dnl 
dnl Get the cflags and libraries from the oln-config script
dnl
AC_ARG_WITH(oln-include,[  --with-oln-include=PFX   Prefix where OLN is installed (optional)],
            oln_include_prefix="$withval", oln_include_prefix="")

  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])

  OLN_INCLUDEDIR=""
  oln_save_CXXFLAGS="$CXXFLAGS"
  oln_save_CPPFLAGS="$CPPFLAGS"

  AC_MSG_NOTICE([checking for Olena headers])

  within_olena_source_tree=ifelse([$1], [], [no], [$1])
  if test x$within_olena_source_tree != xno; then
     AC_MSG_CHECKING([for Olena around us ($within_olena_source_tree)])
     within_olena_source_tree=`( cd $srcdir; cd $within_olena_source_tree; pwd )`
     if test -r $within_olena_source_tree/oln/config/pconf.hh; then
        AC_MSG_RESULT([yes, present])
        OLN_SOURCE_DIR="${within_olena_source_tree}/oln"
        OLN_INCLUDEDIR="${within_olena_source_tree}"
        CXXFLAGS="-I${within_olena_source_tree} $CXXFLAGS"
        CPPFLAGS="-I${within_olena_source_tree} $CPPFLAGS"
     else
        AC_MSG_RESULT([no, searching again])
     fi
  fi

  if test x$oln_include_prefix != x ; then
     AC_MSG_CHECKING([for Olena in $oln_include_prefix])
     if test -r $oln_include_prefix/oln/config/pconf.hh; then
        AC_MSG_RESULT([yes, present])
        if test x$OLN_INCLUDEDIR != x ; then
           AC_MSG_WARN([Olena present in $OLN_INCLUDEDIR, but using version in $oln_include_prefix])
        fi
        OLN_SOURCE_DIR="$oln_include_prefix/oln"
        OLN_INCLUDEDIR="$oln_include_prefix"
        CXXFLAGS="-I$oln_include_prefix $CXXFLAGS"
        CPPFLAGS="-I$oln_include_prefix $CPPFLAGS"
     else
        AC_MSG_RESULT([no])
     fi
  fi

  have_olena=yes
  AC_CHECK_HEADER([oln/config/pconf.hh], [], [have_olena=no])

  if test $have_olena = yes; then
     if test x$OLN_INCLUDEDIR != x; then
        AC_MSG_NOTICE([using include path $OLN_INCLUDEDIR to find Olena.])
     fi
     if test -z "$OLN_SOURCE_DIR"; then
        AC_MSG_CHECKING([for location of Olena headers])
        OLN_SOURCE_DIR=no
        AC_TRY_RUN([
#include <oln/config/pconf.hh>
#include <fstream>
int main(void) { std::ofstream f("conftest.oln"); f << OLN_SOURCE_DIR; return 0; }
                   ], [OLN_SOURCE_DIR=`cat conftest.oln`], [], [])
        if test x$OLN_SOURCE_DIR != xno; then
             if test -r $OLN_SOURCE_DIR/config/pconf.hh; then
                AC_MSG_RESULT([$OLN_SOURCE_DIR])
             else
	        AC_MSG_RESULT([duh ?])
	        AC_MSG_WARN([Olena advertises $OLN_SOURCE_DIR but $OLN_SOURCE_DIR/config/pconf.hh not found])
	     fi
        else 
            OLN_SOURCE_DIR=''
            AC_MSG_RESULT([unknown])
        fi
     fi
  else
     AC_MSG_WARN([*** Olena source headers not found. Expect problems.])
  fi   
  CXXFLAGS="$oln_save_CXXFLAGS"
  CPPFLAGS="$oln_save_CPPFLAGS"

  AC_SUBST([OLN_SOURCE_DIR])
  AC_SUBST([OLN_INCLUDEDIR])
  
  AC_LANG_POP([C++])
])

AC_DEFUN(AC_TEMPLATE_DEPTH,
[
  AC_REQUIRE([AC_PROG_CXX])
  AC_LANG_PUSH([C++])

  cxx_tdepth=ifelse([$1], , 50, [$1])

  AC_MSG_CHECKING([for automatic C++ template depth adjustment by the compiler, upto $cxx_tdepth])
 
  oln_save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS -DTDEPTH=$cxx_tdepth"
  AC_TRY_RUN([
   template<unsigned n> struct rec { typedef typename rec<n-1>::ret ret; };
   template<> struct rec<0> { typedef int ret; };
   int main(void) { rec<TDEPTH>::ret i = 0; return i; }
  ], [AC_MSG_RESULT([yes])], [
     oln_save_CXXFLAGS="$CXXFLAGS"
     CXXFLAGS="$CXXFLAGS -ftemplate-depth-$cxx_tdepth"
     AC_TRY_RUN([
        template<unsigned n> struct rec { typedef typename rec<n-1>::ret ret; };
        template<> struct rec<0> { typedef int ret; };
        int main(void) { rec<TDEPTH>::ret i = 0; return i; }
       ], [AC_MSG_RESULT([yes, with -ftemplate-depth-$cxx_tdepth])], [
          AC_MSG_RESULT([no])
	  AC_MSG_WARN([deep template recursions ($cxx_tdepth) not supported. Sorry.])
          CXXFLAGS="$oln_save_CXXFLAGS"
	])
	])
  CPPFLAGS="$oln_save_CPPFLAGS"

  AC_LANG_POP([C++])
])

AC_DEFUN(OLN_ENABLE_EXCEPTIONS,
[
  oln_exn_default=ifselse([$1], , [yes], [$1])

  AC_ARG_ENABLE(oln-exceptions,
       [AC_HELP_STRING([--enable-oln-exceptions], [enable generation of C++ exceptions instead of aborting on error (default is $oln_exn_default)])],
	[oln_exn_enable=$enableval], [oln_exn_enable=$oln_exn_default])
  if test x$oln_exn_enable != xno; then
     CPPFLAGS="$CPPFLAGS -DOLN_EXCEPTIONS"
  fi
])
AC_DEFUN(OLN_DISABLE_EXCEPTIONS, [OLN_ENABLE_EXCEPTIONS([no])])


AC_DEFUN(AC_WITH_OLN,
[
  AC_REQUIRE([AM_PATH_OLN])
  AC_REQUIRE([AC_TEMPLATE_DEPTH])

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
      oln_save_CFLAGS="$CFLAGS"
      oln_save_LDFLAGS="$LDFLAGS"
      CFLAGS="$CFLAGS $FFTW_CFLAGS"
      LDFLAGS="$LDFLAGS $FFTW_LDFLAGS"
      AC_CHECK_HEADER([fftw.h],
           [AC_CHECK_LIB([fftw], [fftw2d_create_plan],
                [FFTW_LDFLAGS="${FFTW_LDFLAGS} -lfftw -lrfftw"
                 AC_DEFINE([HAVE_FFTW], 1, [Define to 1 if we can use fftw])
                ])
           ])
      CFLAGS="$oln_save_CFLAGS"
      LDFLAGS="$oln_save_LDFLAGS"
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
      oln_save_CFLAGS="$CFLAGS"
      oln_save_LDFLAGS="$LDFLAGS"
      CFLAGS="$CFLAGS $ZLIB_CFLAGS"
      LDFLAGS="$LDFLAGS $ZLIB_LDFLAGS"
      AC_CHECK_HEADER([zlib.h],
       [AC_CHECK_LIB([z], [gzopen],
          [ZLIB_LDFLAGS="${ZLIB_LDFLAGS} -lz"
            AC_DEFINE([HAVE_ZLIB], 1, [Define to 1 if we can use zlib])])])
      CFLAGS="$oln_save_CFLAGS"
      LDFLAGS="$oln_save_LDFLAGS"
   fi
   AC_SUBST([ZLIB_CFLAGS])
   AC_SUBST([ZLIB_LDFLAGS])
])

