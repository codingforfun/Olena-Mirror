include ../check/Makefile.flags

EXTRA_DIST = tests runtests image_print.sh

image-print:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(AM_LDFLAGS)" \
	CPPFLAGS="$(AM_CPPFLAGS)" \
	CXXFLAGS="$(AM_CXXFLAGS)" \
	  $(SHELL) $(srcdir)/image_print.sh

tests: 
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(AM_LDFLAGS)" \
	CPPFLAGS="$(AM_CPPFLAGS)" \
	CXXFLAGS="$(AM_CXXFLAGS)" \
	  $(SHELL) $(srcdir)/runtests

check-local: image-print tests

distclean-local:
	rm -rf runs

.PHONY: tests
