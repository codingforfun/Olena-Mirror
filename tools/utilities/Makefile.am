## Process this file with Automake to produce Makefile.in -*-Makefile-*-

SUBDIRS = $(OLN_UTILITIES_SUBDIRS)

AM_CPPFLAGS = $(OLN_CPPFLAGS) 
AM_CXXFLAGS = $(OLN_CXXFLAGS) $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) $(ZLIB_CXXFLAGS)

bin_PROGRAMS = oln-mosaic-u8 oln-mosaic-u16
dist_man_MANS = oln-mosaic-u8.1 oln-mosaic-u16.1

INCLUDES = -I$(srcdir)/../lib -I$(OLN_LOCAL_SRC) -I$(OLN_LOCAL_BUILD)
LDADD = ../lib/libolntools.a

oln_mosaic_u8_SOURCES = mosaic.cc
oln_mosaic_u8_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u8
if HAVE_HELP2MAN
oln-mosaic-u8.1: oln-mosaic-u8$(EXEEXT)
	PATH=.:$$PATH $(HELP2MAN) $< >$@
endif

oln_mosaic_u16_SOURCES = mosaic.cc
oln_mosaic_u16_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u16
if HAVE_HELP2MAN
oln-mosaic-u16.1: oln-mosaic-u16$(EXEEXT)
	PATH=.:$$PATH $(HELP2MAN) $< >$@
endif

if HAVE_HELP2MAN
### FIXME
# This is ugly, but necessary since `make dist' can be called
# before the library was compiled.
../lib/libolntools.a:
	cd ../lib && $(MAKE) libolntools.a
endif

