include $(top_srcdir)/check/Makefile.check

AM_LDFLAGS = $(FFTW_LDFLAGS)
AM_CFLAGS = $(FFTW_CFLAGS)

check_PROGRAMS = fft dwt

fft_SOURCES = fft.cc
fft_OUTPUT_DIR = output/

dwt_SOURCES = dwt.cc
dwt_OUTPUT_DIR = output/

CLEANFILES = $(fft_OUTPUT_DIR)output_fft_trans_clipped.pgm \
	$(fft_OUTPUT_DIR)output_fft_trans_log.pgm \
	$(fft_OUTPUT_DIR)output_fft_low_pass.pgm \
	$(fft_OUTPUT_DIR)output_fft_copy.pgm \
	$(dwt_OUTPUT_DIR)output_dwt_trans_non_std.pgm \
	$(dwt_OUTPUT_DIR)output_dwt_trans_std.pgm \
	$(dwt_OUTPUT_DIR)output_dwt_compressed.pgm \
	$(dwt_OUTPUT_DIR)output_dwt_copy.pgm
