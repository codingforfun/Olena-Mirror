## Process this file through Automake to create Makefile.in -*- Makefile -*-

include ../check/Makefile.check

check_PROGRAMS = 2d bin se_pass se_fail pretty
2d_SOURCES = 2d.cc loadsave.hh
se_pass_SOURCES = se_pass.cc loadsave.hh
se_fail_SOURCES = se_fail.cc loadsave.hh
pretty_SOURCES = pretty.cc loadsave.hh
bin_SOURCES = bin.cc

CLEANFILES =					\
  black-test.pbm white-test.pbm			\
  lena						\
  lena.ppm lena.pgm lena.pbm			\
  lena.pppm lena.ppgm lena.ppbm			\
  lena.ppm.gz lena.pgm.gz lena.pbm.gz		\
  lena16b.pgm lena16b.ppgm			\
  neighbor.pbm neighbor.ppbm			\
  neighbor9.pbm neighbor9.ppbm			\
  se.pbm se.ppbm se9.pbm se9.ppbm		\
  se9.pbm.gz neighbor9.pbm.gz
