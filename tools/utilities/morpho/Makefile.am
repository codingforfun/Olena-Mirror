## Process this file with Automake to produce Makefile.in -*- Makefile -*-

AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) $(ZLIB_CXXFLAGS)
AM_LDFLAGS = $(LDFLAGS)

INCLUDES = -I$(srcdir)/../../lib
LDADD = ../../lib/libolntools.a $(ZLIB_LDFLAGS)

$(srcdir)/morpho-rules.make: $(srcdir)/generate_morpho.sh
	$(SHELL) $(srcdir)/generate_morpho.sh $(srcdir)

$(srcdir)/Makefile.in: $(srcdir)/morpho-rules.make

MAINTAINERCLEANFILES = $(srcdir)/morpho-rules.make
EXTRA_DIST = generate_morpho.sh morpho-rules.make

if REGEN_MANPAGES
### FIXME
# This is ugly, but necessary since `make dist' can be called
# before the library was compiled.
../../lib/libolntools.a:
	cd ../../lib && $(MAKE) libolntools.a
endif

include $(srcdir)/morpho-rules.make
