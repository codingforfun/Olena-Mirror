## Process this file with Automake to produce Makefile.in -*-Makefile-*-

SUBDIRS = oln img $(OLN_SRC_SUBDIRS)
EXTRA_DIST = ISSUES

m4datadir = $(datadir)/aclocal
dist_m4data_DATA = oln.m4

ACLOCAL_AMFLAGS = -I . -I ..
CONFIGURE_DEPENDENCIES = $(srcdir)/oln.m4 \
	$(srcdir)/../oln-version.m4 \
	$(srcdir)/../component.m4

maintainer-check:
	if cppi --version >/dev/null 2>&1; then \
	  find $(srcdir) -name '*.cc' -o -name '*.hh' -o -name '*.hxx' | \
	    xargs cppi --check; \
	else \
	  echo "$@: GNU cppi is required" >&2; \
	fi
