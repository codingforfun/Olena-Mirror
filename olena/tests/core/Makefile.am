EXTRA_DIST = tests runtests image_print.sh

check-local:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(LDFLAGS)" \
	CPPFLAGS="$(CPPFLAGS) $(DEFS) -I$(top_builddir)" \
	CXXFLAGS="$(CXXFLAGS) $(CXXFLAGS_OPTIMIZE) $(CXXFLAGS_STRICT_ERRORS)" \
	  $(SHELL) $(srcdir)/image_print.sh
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(LDFLAGS)" \
	CPPFLAGS="$(CPPFLAGS) $(DEFS) -I$(top_builddir)" \
	CXXFLAGS="$(CXXFLAGS) $(CXXFLAGS_OPTIMIZE) $(CXXFLAGS_STRICT_ERRORS)" \
	  $(SHELL) $(srcdir)/runtests

distclean-local:
	rm -rf runs
