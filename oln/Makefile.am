## Process this file with Automake to produce Makefile.in -*-Makefile-*-

SUBDIRS = oln @OLN_SRC_SUBDIRS@

EXAMPLE_IMGS = \
	img/face_se.pbm \
	img/lena.ppm \
	img/lena256.pgm \
	img/lena_dark.pgm \
	img/map.pbm \
	img/neighbor.pbm \
	img/neighbor9.pbm \
	img/object.pbm \
	img/random.pgm \
	img/se.pbm \
	img/se9.pbm

dist_pkgdata_DATA = $(EXAMPLE_IMGS)

EXTRA_DIST = ISSUES

m4datadir = $(datadir)/aclocal
dist_m4data_DATA = oln.m4

