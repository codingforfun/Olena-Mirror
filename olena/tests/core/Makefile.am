EXTRA_DIST = tests runtests image_print.sh

image-print:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(LDFLAGS)" \
	CPPFLAGS="$(CPPFLAGS) $(DEFS) -I$(top_builddir)" \
	CXXFLAGS="$(CXXFLAGS) $(CXXFLAGS_OPTIMIZE) $(CXXFLAGS_STRICT_ERRORS)" \
	  $(SHELL) $(srcdir)/image_print.sh

tests: 
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(LDFLAGS)" \
	CPPFLAGS="$(CPPFLAGS) $(DEFS) -I$(top_builddir)" \
	CXXFLAGS="$(CXXFLAGS) $(CXXFLAGS_OPTIMIZE) $(CXXFLAGS_STRICT_ERRORS)" \
	  $(SHELL) $(srcdir)/runtests

check-local: image-print tests

distclean-local:
	rm -rf runs

.PHONY: tests
