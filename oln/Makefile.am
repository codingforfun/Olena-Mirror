## Process this file with Automake to produce Makefile.in -*- Makefile -*-

# OLN_SRC_SUBDIRS is set when "configure" is run.
SUBDIRS = oln img $(OLN_SRC_SUBDIRS)

# oln.m4 is for users, install it with other aclocal data.
m4datadir = $(datadir)/aclocal
dist_m4data_DATA = oln.m4

EXTRA_DIST = ISSUES

# "configure.ac" in this directory uses macros in m4 files.
ACLOCAL_AMFLAGS = -I . -I ..
CONFIGURE_DEPENDENCIES = \
	oln.m4 \
	$(srcdir)/../oln-version.m4 \
	$(srcdir)/../component.m4

# This is totally experimental. Comments are welcome.
maintainer-check:
	if cppi --version >/dev/null 2>&1; then \
	  find $(srcdir) -name '*.cc' -o -name '*.hh' -o -name '*.hxx' | \
	    xargs cppi --check; \
	else \
	  echo "$@: GNU cppi is required" >&2; \
	fi
