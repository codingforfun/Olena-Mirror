## Process this file with Automake to produce Makefile.in -*- Makefile -*-

## -- FIXME: Improve integration.  ----------------------------------------
##
## The contents of `olena/lrde' should be moved to subdirectory of
## `olena/oln', and the contents of `olena/lrde/ufmt/bin to `olena/tests'
## or to `tools'.

AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -ggdb -O3 -DNDEBUG 
AM_LDFLAGS = -lcfitsio $(ZLIB_LDFLAGS)

noinst_HEADERS =				\
  io.hh						\
  misc.hh					\
  req.hh					\
  wst.hh

# FIXME: Some (or all) of these programs needs a support for the FITS
# image format; add conditional to prevent their compilation when the
# libcfitsio library is absent.

check_PROGRAMS =				\
  gaussian					\
  pfm2pgm					\
  pgm2pfm					\
  pgm2pfm_wo_noise				\
  wst

gaussian_SOURCES = gaussian.cc
pfm2pgm_SOURCES  = pfm2pgm.cc
pgm2pfm_wo_noise_SOURCES = pgm2pfm_wo_noise.cc
pgm2pfm_SOURCES  = pgm2pfm.cc
wst_SOURCES      = wst.cc

# FIXME: Write (runtime) tests!

## ---------------------------------------- FIXME: Improve integration.  --
