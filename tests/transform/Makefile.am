include $(top_srcdir)/tests/check/Makefile.check

check_PROGRAMS = \
  fft

CLEANFILES = lena_fft_clipped.pgm lena_fft_log.pgm lena_low_pass.pgm

fft_LDFLAGS = -lfftw -lrfftw

fft_SOURCES = fft.cc
