include ../check/Makefile.check

EXTRA_DIST = tests runtests image_print.sh

check_PROGRAMS = multiple_sources

multiple_sources_SOURCES = multiple_sources_1.cc multiple_sources_2.cc
multiple_sources_LDFLAGS = $(AM_LDFLAGS) $(FFTW_LDFLAGS)
multiple_sources_CXXFLAGS = $(AM_CXXFLAGS) $(FFTW_CFLAGS)

image-print:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(AM_LDFLAGS)" \
	CPPFLAGS="$(AM_CPPFLAGS)" \
	CXXFLAGS="$(AM_CXXFLAGS)" \
	  $(SHELL) $(srcdir)/image_print.sh

other-tests: 
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(AM_LDFLAGS)" \
	CPPFLAGS="$(AM_CPPFLAGS)" \
	CXXFLAGS="$(AM_CXXFLAGS)" \
	  $(SHELL) $(srcdir)/runtests

check-local: image-print other-tests

distclean-local:
	rm -rf runs
