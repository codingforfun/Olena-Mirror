# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.57])

m4_include(../../oln-version.m4)
AC_INIT([oln-conf], OLN_VERSION, OLN_CONTACT)
AC_CONFIG_SRCDIR([oln-config.shin])

AC_WITH_OLN

if test "x$have_olena" != xyes; then
  echo "Olena not found" 1>&2
  exit 1
fi

AC_CXX_FLAGS

AC_CONFIG_FILES([oln-config.sh:oln-config.shin], [chmod +x oln-config.sh])
AC_OUTPUT

