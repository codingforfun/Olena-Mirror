include ../check/Makefile.flags

EXTRA_DIST = tests runtests

check-local:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(LDFLAGS_RUNTESTS)" \
	CPPFLAGS="$(CPPFLAGS_RUNTESTS)" \
	CXXFLAGS="$(CXXFLAGS_RUNTESTS)" \
	  $(SHELL) $(srcdir)/runtests

distclean-local:
	rm -rf runs
