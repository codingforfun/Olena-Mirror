
AM_CPPFLAGS = $(OLN_CPPFLAGS) 
AM_CXXFLAGS = $(OLN_CXXFLAGS) $(ZLIB_CXXFLAGS)

EXTRA_DIST = generate_morpho.sh morpho-rules.make

include $(srcdir)/morpho-rules.make

$(srcdir)/morpho-rules.make: $(srcdir)/generate_morpho.sh
	$(SHELL) $(srcdir)/generate_morpho.sh $(srcdir)

$(srcdir)/Makefile.in: $(srcdir)/morpho-rules.make

