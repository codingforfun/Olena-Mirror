include $(top_srcdir)/tests/check/Makefile.check

AM_LDFLAGS = $(ZLIB_LDFLAGS)
AM_CFLAGS = $(ZLIB_CFLAGS)

check_PROGRAMS = 2d
2d_SOURCES = 2d.cc loadsave.hh

CLEANFILES = \
  lena \
  lena.ppm lena.pgm lena.pbm \
  lena.pppm lena.ppgm lena.ppbm \
  lena.ppm.gz lena.pgm.gz lena.pbm.gz
