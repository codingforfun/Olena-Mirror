# XTC_REPLACE_FUNCS(FUNCTION...)
# ------------------------------
# A replacement for AC_REPLACE_FUNCS that does not call AC_DEFINE when
# the checked function works.
AC_DEFUN([XTC_REPLACE_FUNCS],
[_$0(m4_flatten([$1]))])

m4_define([_XTC_REPLACE_FUNCS],
[AS_LITERAL_IF([$1],
[m4_map_args_w([$1], [_AC_REPLACE_FUNC(], [)
])],
[AC_CHECK_FUNCS([$1],
  [_AH_CHECK_FUNC([$ac_func])],
  [_AC_LIBOBJ([$ac_func])])])])
