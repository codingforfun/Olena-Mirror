## Process this file with Automake to produce Makefile.in -*-Makefile-*-

SUBDIRS = $(OLN_UTILITIES_SUBDIRS)

AM_CPPFLAGS = $(OLN_CPPFLAGS) 
AM_CXXFLAGS = $(OLN_CXXFLAGS) $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) $(ZLIB_CXXFLAGS)

bin_PROGRAMS = oln-mosaic-u8 oln-mosaic-u16

INCLUDES = -I$(srcdir)/../lib -I$(OLN_LOCAL_SRC) -I$(OLN_LOCAL_BUILD)
LDADD = ../lib/libolntools.a

oln_mosaic_u8_SOURCES = mosaic.cc
oln_mosaic_u8_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u8
oln_mosaic_u16_SOURCES = mosaic.cc
oln_mosaic_u16_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u16
