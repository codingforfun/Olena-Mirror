# Process this file through Automake to produce Makefile.in

# OLN_DOC_SUBDIRS is set when "configure" is run.
SUBDIRS = $(OLN_DOC_SUBDIRS)

# Users can use the distribution to build the documentation anew.
EXTRA_DIST = tpl/processing.tpl 

# "configure.ac" in this directory uses macros defined in m4
# files in other directories. 
ACLOCAL_AMFLAGS = -I ../oln -I ..
CONFIGURE_DEPENDENCIES = \
	 $(srcdir)/../oln/oln.m4 \
	 $(srcdir)/../oln-version.m4 \
	 $(srcdir)/../component.m4
