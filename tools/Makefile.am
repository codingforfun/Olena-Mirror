## Process this file with Automake to produce Makefile.in -*- Makefile -*-

# OLN_TOOLS_SUBDIRS is set when "configure" is run.
SUBDIRS = lib $(OLN_TOOLS_SUBDIRS)

# "configure.ac" in this directory uses macros in m4 files.
ACLOCAL_AMFLAGS = -I ../oln -I ..
CONFIGURE_DEPENDENCIES = $(srcdir)/../oln/oln.m4 \
	$(srcdir)/../oln-version.m4 \
	$(srcdir)/../component.m4

