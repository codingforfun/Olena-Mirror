EXTRA_DIST = tests runtests

check-local:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
        TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(LDFLAGS)" \
	CPPFLAGS="$(CPPFLAGS) $(DEFS) -I$(top_builddir) -I$(OLN_LOCAL_SRC) -I$(OLN_LOCAL_BUILD)" \
	CXXFLAGS="$(CXXFLAGS) $(CXXFLAGS_OPTIMIZE) $(CXXFLAGS_STRICT_ERRORS)" \
	  $(SHELL) $(srcdir)/runtests

distclean-local:
	rm -rf runs
