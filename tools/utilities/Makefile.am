## Process this file with Automake to produce Makefile.in -*-Makefile-*-

SUBDIRS = $(OLN_UTILITIES_SUBDIRS)

AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) $(ZLIB_CXXFLAGS)

bin_PROGRAMS = oln-mosaic-u8 oln-mosaic-u16
dist_man_MANS = oln-mosaic-u8.1 oln-mosaic-u16.1

INCLUDES = -I$(srcdir)/../lib
LDADD = ../lib/libolntools.a

oln_mosaic_u8_SOURCES = mosaic.cc
oln_mosaic_u8_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u8
if REGEN_MANPAGES
$(srcdir)/oln-mosaic-u8.1: oln-mosaic-u8$(EXEEXT)
	PATH=.:$$PATH $(HELP2MAN) --output=$@ $< 
endif

oln_mosaic_u16_SOURCES = mosaic.cc
oln_mosaic_u16_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u16
if REGEN_MANPAGES
$(srcdir)/oln-mosaic-u16.1: oln-mosaic-u16$(EXEEXT)
	PATH=.:$$PATH $(HELP2MAN) --output=$@ $<
endif

if REGEN_MANPAGES
### FIXME
# This is ugly, but necessary since `make dist' can be called
# before the library was compiled.
../lib/libolntools.a:
	cd ../lib && $(MAKE) libolntools.a
endif

MAINTAINERCLEANFILES = $(srcdir)/oln-mosaic-u8.1 $(srcdir)/oln-mosaic-u16.1
