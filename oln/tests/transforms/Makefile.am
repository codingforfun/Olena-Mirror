include $(top_srcdir)/lib/check/Makefile.check

AM_LDFLAGS = $(FFTW_LDFLAGS)
AM_CFLAGS = $(FFTW_CFLAGS)

check_PROGRAMS = fft dwt

fft_SOURCES = fft.cc

dwt_SOURCES = dwt.cc

CLEANFILES = fft_trans_clipped.pgm \
	fft_trans_log.pgm \
	fft_low_pass.pgm \
	fft_copy.pgm \
	dwt_trans_non_std.pgm \
	dwt_trans_std.pgm \
	dwt_compressed.pgm \
	dwt_copy.pgm
