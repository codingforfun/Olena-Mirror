include ../check/Makefile.check

EXTRA_DIST = \
	test.sh \
	test-inter-pixel.ref \
	test-cmap.ref

check_PROGRAMS = \
	test-inter-pixel \
	test-cmap

test_cmap_SOURCES = \
	test-cmap.cc

test_inter_pixel_SOURCES = \
	test-inter-pixel.cc

check-local: test-inter-pixel test-cmap
	SRCDIR=$(srcdir) $(SHELL) $(srcdir)/test.sh test-inter-pixel
	SRCDIR=$(srcdir) $(SHELL) $(srcdir)/test.sh test-cmap

CLEANFILES = *.run
