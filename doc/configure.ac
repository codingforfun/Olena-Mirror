# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.50])
AC_INIT([olena-doc], [0.3], [olena-bugs@lrde.epita.fr])
AC_CONFIG_SRCDIR([ChangeLog])
AC_CONFIG_AUX_DIR([../tools])

dnl Check for tools needed for formatting the documentation.
AC_PATH_PROG(DVIPS, dvips, $am_aux_dir/missing dvips)
AC_PATH_PROG(TEXI2PDF, texi2pdf, $am_aux_dir/missing texi2pdf)
AC_PATH_PROG(TEXI2HTML, texi2html, $am_aux_dir/missing texi2html)

AM_INIT_AUTOMAKE([olena], [0.3], [:])
AC_CONFIG_FILES([Makefile doc/Makefile])
AC_OUTPUT
