## Process this file with Automake to produce Makefile.in -*-Makefile-*-

SOURCE_IMGS =			\
  black.pbm			\
  white.pbm			\
  face_se.pbm			\
  lena.ppm			\
  lena1d.ppm			\
  lena256.pgm			\
  lena_dark.pgm			\
  map.pbm			\
  neighbor.pbm			\
  neighbor9.pbm			\
  object.pbm			\
  random.pgm			\
  se.pbm			\
  se9.pbm			\
  test-cmap.pgm

BUILT_IMGS =			\
  lena.pbm			\
  lena.pbm.gz			\
  lena.pgm			\
  lena.pgm.gz			\
  lena.ppbm			\
  lena.ppgm			\
  lena.ppm.gz			\
  lena.pppm			\
  lena128.pgm			\
  lena16b.pgm			\
  lena16b.ppgm			\
  lena1d.pbm			\
  lena1d.pbm.gz			\
  lena1d.pgm			\
  lena1d.pgm.gz			\
  lena1d.ppbm			\
  lena1d.ppgm			\
  lena1d.ppm.gz			\
  lena1d.pppm			\
  lena1d128.pgm			\
  lena1d16b.pgm			\
  lena1d16b.ppgm		\
  se.ppbm			\
  se9.ppbm			\
  se9.pbm.gz			\
  neighbor.ppbm			\
  neighbor9.ppbm		\
  neighbor9.pbm.gz

SRC_BUILT_IMGS =			\
  $(srcdir)/lena.pbm			\
  $(srcdir)/lena.pbm.gz			\
  $(srcdir)/lena.pgm			\
  $(srcdir)/lena.pgm.gz			\
  $(srcdir)/lena.ppbm			\
  $(srcdir)/lena.ppgm			\
  $(srcdir)/lena.ppm.gz			\
  $(srcdir)/lena.pppm			\
  $(srcdir)/lena128.pgm			\
  $(srcdir)/lena16b.pgm			\
  $(srcdir)/lena16b.ppgm		\
  $(srcdir)/lena1d.pbm			\
  $(srcdir)/lena1d.pbm.gz		\
  $(srcdir)/lena1d.pgm			\
  $(srcdir)/lena1d.pgm.gz		\
  $(srcdir)/lena1d.ppbm			\
  $(srcdir)/lena1d.ppgm			\
  $(srcdir)/lena1d.ppm.gz		\
  $(srcdir)/lena1d.pppm			\
  $(srcdir)/lena1d128.pgm		\
  $(srcdir)/lena1d16b.pgm		\
  $(srcdir)/lena1d16b.ppgm		\
  $(srcdir)/se.ppbm			\
  $(srcdir)/se9.ppbm			\
  $(srcdir)/se9.pbm.gz			\
  $(srcdir)/neighbor.ppbm		\
  $(srcdir)/neighbor9.ppbm		\
  $(srcdir)/neighbor9.pbm.gz

dist_pkgdata_DATA = $(SOURCE_IMGS) $(SRC_BUILT_IMGS)
MAINTAINERCLEANFILES = $(SRC_BUILT_IMGS)


# Conversion rules.

.png.ppm:
	$(PNGTOPNM) $< > $@

.ppm.pgm:
	$(PPMTOPGM) $< > $@

.pgm.pbm:
	$(PGMTOPBM) $< > $@

.ppm.pppm:
	$(PNMTOPLAINPNM) $< > $@

.pgm.ppgm:
	$(PNMTOPLAINPNM) $< > $@

.pbm.ppbm:
	$(PNMTOPLAINPNM) $< > $@

# This is a bit hackish.  There is no way to introduce a derivation
# for `' -> `.gz', so we define the `m' -> `m.gz' rule instead.
# (all our suffixes ends in `m': ppm, pgm, pbm.)
SUFFIXES = m m.gz
mm.gz:
	$(GZIP) -c -9 $< > $@

$(srcdir)/lena128.pgm: lena.pgm
	$(IMAGEMAGICK_CONVERT) -scale 25% `test -f '$<' || echo '$(srcdir)/'`$< $@

$(srcdir)/lena16b.pgm: lena.pgm
	$(PNMDEPTH) 65535 `test -f '$<' || echo '$(srcdir)/'`$< > $@

$(srcdir)/lena1d128.pgm: lena1d.pgm
	$(IMAGEMAGICK_CONVERT) -scale 25% `test -f '$<' || echo '$(srcdir)/'`$< $@

$(srcdir)/lena1d16b.pgm: lena1d.pgm
	$(PNMDEPTH) 65535 `test -f '$<' || echo '$(srcdir)/'`$< > $@
