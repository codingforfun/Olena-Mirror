## Process this file with Automake to produce Makefile.in -*-Makefile-*-

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)
AM_CXXFLAGS = -Wall -W -DNDEBUG -ftemplate-depth-50 -ansi -pedantic

bin_PROGRAMS = oln-mosaic-u8 oln-mosaic-u16

oln_mosaic_u8_SOURCES = mosaic.cc
oln_mosaic_u8_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u8
oln_mosaic_u16_SOURCES = mosaic.cc
oln_mosaic_u16_CXXFLAGS = $(AM_CXXFLAGS) -DTYPE=int_u16
