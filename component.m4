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
   $5_SUBDIRS="$[]$5_SUBDIRS ifelse([$8], [], [$1], [$8])"
   ifelse([$6], [], [], [AC_CONFIG_FILES([$6])])
   $7
fi
])

