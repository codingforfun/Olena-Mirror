## Process this file through Automake to produce Makefile.in -*- Makefile -*-

# OLN_TOP_SUBDIRS is set when "configure" is run.
SUBDIRS = oln $(OLN_TOP_SUBDIRS)

# "configure.ac" in this directory uses macros in m4 files.
ACLOCAL_AMFLAGS = -I oln/config
CONFIGURE_DEPENDENCIES = oln/config/oln.m4

# This is totally experimental. Comments are welcome.
maintainer-check:
	if cppi --version >/dev/null 2>&1; then \
	  find $(srcdir) -name '*.{cc,hh,hxx}' | \
	    xargs cppi --check; \
	else \
	  echo "$@: GNU cppi is required" >&2; \
	fi

dist_noinst_DATA = README BUILD INSTALL THANKS
INFOTOTEXT = $(MAKEINFO) --enable-encoding --no-headers

README_SRC = $(srcdir)/doc/dev/abstract.texi $(srcdir)/doc/dev/biblio.texi
README: $(README_SRC)
	rm -f $@
	for f in $(README_SRC); do \
	  $(INFOTOTEXT) "$$f" >>$@ ; \
	done

BUILD_SRC = $(srcdir)/doc/dev/build.texi
BUILD: $(BUILD_SRC)
	$(INFOTOTEXT) $(BUILD_SRC) >$@ 

INSTALL_SRC = $(srcdir)/doc/dev/install.texi
INSTALL: $(INSTALL_SRC)
	$(INFOTOTEXT) $(INSTALL_SRC) >$@

THANKS_SRC = $(srcdir)/doc/dev/thanks.texi
THANKS: $(THANKS_SRC)
	$(INFOTOTEXT) $(THANKS_SRC) >$@

