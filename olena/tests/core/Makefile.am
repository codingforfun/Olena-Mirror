include ../check/Makefile.runtests
include ../check/Makefile.check

EXTRA_DIST += image_print.sh

check_PROGRAMS = multiple_sources

multiple_sources_SOURCES = multiple_sources_1.cc multiple_sources_2.cc
multiple_sources_LDFLAGS = $(AM_LDFLAGS)
multiple_sources_LDADD = $(LDADD) $(FFTW_LDFLAGS)
multiple_sources_CXXFLAGS = $(AM_CXXFLAGS) $(FFTW_CFLAGS)

check-image-print:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	LDFLAGS="$(LDFLAGS_RUNTESTS)" \
	CPPFLAGS="$(CPPFLAGS_RUNTESTS)" \
	CXXFLAGS="$(CXXFLAGS_RUNTESTS)" \
	  $(SHELL) $(srcdir)/image_print.sh

check-local: check-runtests check-image-print
