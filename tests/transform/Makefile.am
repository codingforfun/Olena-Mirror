include $(top_srcdir)/tests/check/Makefile.check

AM_LDFLAGS = $(FFTW_LDFLAGS)
AM_CFLAGS = $(FFTW_CFLAGS)

check_PROGRAMS = fft
fft_SOURCES = fft.cc

fft_OUTPUT_DIR=output/

CLEANFILES = $(fft_OUTPUT_DIR)output_fft_clipped.pgm \
	$(fft_OUTPUT_DIR)output_fft_log.pgm \
	$(fft_OUTPUT_DIR)output_low_pass.pgm \
	$(fft_OUTPUT_DIR)output_copy.pgm
