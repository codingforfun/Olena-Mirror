## Process this file with Automake to produce Makefile.in -*- Makefile -*-

AM_CPPFLAGS = $(OLN_CPPFLAGS) 
AM_CXXFLAGS = $(OLN_CXXFLAGS) $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) $(ZLIB_CXXFLAGS)

INCLUDES = -I$(srcdir)/../../lib -I$(OLN_LOCAL_SRC) -I$(OLN_LOCAL_BUILD)
LDADD = ../../lib/libolntools.a

include $(srcdir)/morpho-rules.make

$(srcdir)/morpho-rules.make: $(srcdir)/generate_morpho.sh
	$(SHELL) $(srcdir)/generate_morpho.sh $(srcdir)

$(srcdir)/Makefile.in: $(srcdir)/morpho-rules.make

MAINTAINERCLEANFILES = $(srcdir)/morpho-rules.make 
EXTRA_DIST = generate_morpho.sh morpho-rules.make 

if HAVE_HELP2MAN
### FIXME
# This is ugly, but necessary since `make dist' can be called
# before the library was compiled.
../../lib/libolntools.a:
	cd ../../lib && $(MAKE) libolntools.a
endif
