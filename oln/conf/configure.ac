# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.57])

AC_INIT([oln-conf], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_SRCDIR([oln-config.shin])

AC_WITH_OLN

if test "x$have_olena" = xyes; then
  AC_MSG_CHECKING([whether Olena works with the selected development environment])
  AC_LANG_PUSH([C++])
  oln_save_CPPFLAGS="$CPPFLAGS"
  oln_save_CXXFLAGS="$CXXFLAGS"
  CPPFLAGS="$CPPFLAGS $OLN_CPPFLAGS"
  CXXFLAGS="$CXXFLAGS $OLN_CXXFLAGS"
  AC_LINK_IFELSE([@%:@include <oln/types/all.hh>
                  int main()
                  {
	              oln::sfloat f = 3.14;
                      oln::int_u8 i = f;
                      return (int)i;
                  }],
                  [AC_MSG_RESULT([yes])], 
                  [AC_MSG_RESULT([no])
                   have_olena=no])
  CPPFLAGS="$oln_save_CPPFLAGS"
  CXXFLAGS="$oln_save_CXXFLAGS"
  AC_LANG_POP([C++])
fi


if test "x$have_olena" != xyes; then
  echo "Olena not found" 1>&2
  exit 1
fi

AC_CXX_FLAGS

AC_CONFIG_FILES([oln-config.sh:oln-config.shin], [chmod +x oln-config.sh])
AC_OUTPUT

