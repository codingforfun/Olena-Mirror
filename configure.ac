# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.54])

# FIXME: define OLN_CONTACT somewhere
AC_INIT([Olena], 0.8, OLN_CONTACT, [olena-bundle])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([1.7 no-define foreign dist-bzip2])

# FIXME: complete !

ac_configure_args="--with-metalic=$srcdir/metalic --with-integre=$srcdir/integre"

AC_CONFIG_SUBDIRS(metalic)
AC_CONFIG_SUBDIRS(integre)
AC_CONFIG_SUBDIRS(olena)

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
