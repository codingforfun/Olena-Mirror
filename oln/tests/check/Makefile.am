## Process this file through Automake to create Makefile.in -*- Makefile -*-

include Makefile.flags

AM_CPPFLAGS += -DOLN_IMG_DIR=\"$(top_srcdir)/oln/img\"

check_LIBRARIES = libolncheck.a

libolncheck_a_SOURCES =				\
  data.hh					\
  data.cc

EXTRA_DIST = Makefile.flags Makefile.check
