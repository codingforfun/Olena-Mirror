include ../check/Makefile.flags

EXTRA_DIST = tests runtests

check-local:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(AM_LDFLAGS)" \
	CPPFLAGS="$(CPPFLAGS) $(DEFS) -I$(top_builddir)" \
	CXXFLAGS="$(AM_CXXFLAGS)" \
	  $(SHELL) $(srcdir)/runtests

distclean-local:
	rm -rf runs
