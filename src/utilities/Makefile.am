## Process this file with Automake to produce Makefile.in -*-Makefile-*-

SUBDIRS = @MORPHO_UTILITIES@

EXTRA_DIST = generate_morpho.sh

AM_CPPFLAGS = -I$(OLN_INCLUDEDIR)
AM_CXXFLAGS = -Wall -W 

bin_PROGRAMS = oln-mosaic-u8 oln-mosaic-u16

oln_mosaic_u8_SOURCES = mosaic.cc
oln_mosaic_u8_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u8
oln_mosaic_u16_SOURCES = mosaic.cc
oln_mosaic_u16_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u16
