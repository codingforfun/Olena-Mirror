## Process this file through Automake to produce Makefile.in -*- Makefile -*-

# OLN_TOP_SUBDIRS is set when "configure" is run.
SUBDIRS = oln $(OLN_TOP_SUBDIRS)

# "configure.ac" in this directory uses macros in m4 files.
ACLOCAL_AMFLAGS = -I .
CONFIGURE_DEPENDENCIES = oln-version.m4 component.m4

EXTRA_DIST = oln-version.m4 component.m4
