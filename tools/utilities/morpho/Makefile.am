## Process this file with Automake to produce Makefile.in -*- Makefile -*-

AM_CPPFLAGS = $(OLN_CPPFLAGS) 
AM_CXXFLAGS = $(OLN_CXXFLAGS) $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) $(ZLIB_CXXFLAGS)

EXTRA_DIST = generate_morpho.sh morpho-rules.make

INCLUDES = -I$(srcdir)/../../lib -I$(OLN_LOCAL_SRC) -I$(OLN_LOCAL_BUILD)
LDADD = ../../lib/libolntools.a

include $(srcdir)/morpho-rules.make

$(srcdir)/morpho-rules.make: $(srcdir)/generate_morpho.sh
	$(SHELL) $(srcdir)/generate_morpho.sh $(srcdir)

$(srcdir)/Makefile.in: $(srcdir)/morpho-rules.make

