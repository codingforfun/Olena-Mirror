## Process this file through Automake to produce Makefile.in -*- Makefile -*-

# OLN_TOP_SUBDIRS is set when "configure" is run.
SUBDIRS = metalic integre olena $(OLN_TOP_SUBDIRS)

# "configure.ac" in this directory uses macros in m4 files.
ACLOCAL_AMFLAGS = -I config
CONFIGURE_DEPENDENCIES = config/oln.m4

###
### Autoconf macro repository.
###

# oln.m4 is for users, install it with other aclocal data.
m4datadir = $(datadir)/aclocal
dist_m4data_DATA = config/oln.m4

###
### Development files.
###

EXTRA_DIST = config/oln-local.m4 contrib/runtests

###
### Basic doc files generation.
###


# This is totally experimental. Comments are welcome.
maintainer-check:
	if cppi --version >/dev/null 2>&1; then \
	  find $(srcdir) -name '*.{cc,hh,hxx}' | \
	    xargs cppi --check; \
	else \
	  echo "$@: GNU cppi is required" >&2; \
	fi
