### FIXME: this is a copy paste of olena related things from old oln.m4.

AC_DEFUN([OLN_CXX_FLOAT_MATH],
[dnl
  OLN_MATH_FUNC([sqrtf], [float f = sqrtf(0.1f);])
  OLN_MATH_FUNC([floorf], [float f = floorf(0.1f);])
  OLN_MATH_FUNC([round], [double f = round(0.1);])
  OLN_MATH_FUNC([roundf], [float f = roundf(0.1f);])
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

m4_define([OLN_CONTACT], [olena-bugs@lrde.epita.fr])

# FIXME: insert other macros here.
