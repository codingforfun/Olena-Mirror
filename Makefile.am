## Process this file through Automake to produce Makefile.in -*- Makefile -*-

# OLN_TOP_SUBDIRS is set when "configure" is run.
SUBDIRS = lib metalic oln $(OLN_TOP_SUBDIRS)

# "configure.ac" in this directory uses macros in m4 files.
ACLOCAL_AMFLAGS = -I oln/config
CONFIGURE_DEPENDENCIES = oln/config/oln.m4

dist_noinst_DATA = README BUILD INSTALL THANKS FAQ
MAINTAINERCLEANFILES = README BUILD INSTALL THANKS FAQ

if HAVE_DOC_DEV

INFOTOTEXT = $(MAKEINFO) --enable-encoding --no-headers --no-validate
devdoc = $(srcdir)/doc/dev

README: $(devdoc)/abstract.texi $(devdoc)/biblio.texi
	$(INFOTOTEXT) $(devdoc)/abstract.texi >$@
	$(INFOTOTEXT) $(devdoc)/biblio.texi >>$@

BUILD: $(devdoc)/build.texi
	$(INFOTOTEXT) $(devdoc)/build.texi >$@

INSTALL: $(devdoc)/install.texi
	$(INFOTOTEXT) $(devdoc)/install.texi >$@

THANKS: $(devdoc)/thanks.texi
	$(INFOTOTEXT) $(devdoc)/thanks.texi >$@

FAQ: $(devdoc)/faq.texi
	$(INFOTOTEXT) $(devdoc)/faq.texi >$@

endif

DISTCHECK_CONFIGURE_FLAGS = --disable-manpages-regen

# This is totally experimental. Comments are welcome.
maintainer-check:
	if cppi --version >/dev/null 2>&1; then \
	  find $(srcdir) -name '*.{cc,hh,hxx}' | \
	    xargs cppi --check; \
	else \
	  echo "$@: GNU cppi is required" >&2; \
	fi
