include $(top_srcdir)/tests/check/Makefile.check

LDADD += $(ZLIB_LDFLAGS)
AM_CFLAGS = $(ZLIB_CFLAGS)

check_PROGRAMS = 2d se_pass se_fail pretty
2d_SOURCES = 2d.cc loadsave.hh
se_pass_SOURCES = se_pass.cc loadsave.hh
se_fail_SOURCES = se_pass.cc loadsave.hh
pretty_SOURCES = pretty.cc loadsave.hh

CLEANFILES =					\
  lena						\
  lena.ppm lena.pgm lena.pbm			\
  lena.pppm lena.ppgm lena.ppbm			\
  lena.ppm.gz lena.pgm.gz lena.pbm.gz		\
  neighbor.pbm neighbor.ppbm			\
  neighbor9.pbm neighbor9.ppbm			\
  se.pbm se.ppbm se9.pbm se9.ppbm		\
  se9.pbm.gz neighbor9.pbm.gz