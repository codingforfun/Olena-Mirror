## Process this file with Automake to produce Makefile.in -*-Makefile-*-

SOURCE_IMGS = \
	face_se.pbm \
	lena.ppm \
	lena256.pgm \
	lena_dark.pgm \
	map.pbm \
	neighbor.pbm \
	neighbor9.pbm \
	object.pbm \
	random.pgm \
	se.pbm \
	se9.pbm

BUILT_IMGS =				\
  lena.pbm				\
  lena.pbm.gz				\
  lena.pgm				\
  lena.pgm.gz				\
  lena.ppbm				\
  lena.ppgm				\
  lena.ppm.gz				\
  lena.pppm				\
  lena128.pgm				\
  se.ppbm				\
  se9.ppbm				\
  se9.pbm.gz				\
  neighbor.ppbm				\
  neighbor9.ppbm			\
  neighbor9.pbm.gz


dist_pkgdata_DATA = $(SOURCE_IMGS) $(BUILT_IMGS)
MAINTAINERCLEANFILES = $(BUILT_IMGS)

# Conversion rules.

.png.ppm:
	pngtopnm $< > $@

.ppm.pgm:
	ppmtopgm $< > $@

.pgm.pbm:
	pgmtopbm $< > $@

.ppm.pppm:
	pnmtoplainpnm $< > $@

.pgm.ppgm:
	pnmtoplainpnm $< > $@

.pbm.ppbm:
	pnmtoplainpnm $< > $@

# This is a bit hackish.  There is no way to introduce a derivation
# for `' -> `.gz', so we define the `m' -> `m.gz' rule instead.
# (all our suffixes ends in `m': ppm, pgm, pbm.)
SUFFIXES = m m.gz
mm.gz:
	gzip -c -9 $< > $@


lena128.pgm: lena.pgm
	convert -scale 25% $< $@

