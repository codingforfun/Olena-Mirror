dnl OLN_COLLECTION([subdirs-variable-name],
dnl                [component-list])
dnl OLN_COMPONENT([dirname], [withname], [varname], [description],
dnl               [subdirs-variable-name],
dnl               [config-files],
dnl               [extra-action-if-selected], [dirname-for-subdirs-if-different-than-first-dirname)

AC_DEFUN([OLN_COMPONENT], [dnl
AC_CACHE_CHECK([whether to build $4],
               [$3],
               [$3=no
                if test -d $srcdir/$1; then
                   AC_ARG_WITH([$2],
                               [AC_HELP_STRING([--without-$2], [disable building $4])],
                               [$3=$withval],
                               [$3=yes])
                fi])
if test x$[]$3 != xno; then
   $5_SUBDIRS="$[]$5_SUBDIRS ifelse([$8], [], [$1], [$8])"
   ifelse([$6], [], [], [AC_CONFIG_FILES([$6])])

   $7
fi])

AC_DEFUN([OLN_COLLECTION], [dnl
$1_SUBDIRS=""

$2

ifelse([$3], [], [], [AC_CONFIG_FILES([$3])])
AC_SUBST([$1_SUBDIRS])])
