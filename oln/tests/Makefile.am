# Process current dir first to built the data.
# Process the check directory next, because it contains functions
# needed by other directories.
SUBDIRS = . check \
  convert \
  convol \
  io \
  meta \
  morpho \
  sanity \
  transforms \
  types

# SOURCEDATA should never be erased, they are needed to generate
# BUILTDATA.  SOURCEDATA are the only data files worth to keep under
# version control.
SOURCEIMGS = lena.ppm random.pgm se.pbm se9.pbm neighbor.pbm neighbor9.pbm

SOURCEDATA = $(SOURCEIMGS:%=data/%)

# BUILTDATA are generated from SOURCEDATA and can be erased
# by maintainer-clean.
BUILTIMGS =				\
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

BUILTDATA = $(BUILTIMGS:%=data-src/%)

MAINTAINERCLEANFILES = $(BUILTDATA) 

# Currently we ship all data files, even generated, in case the user
# doesn't have all the tools required to generated them.  If it turns
# out it makes a too big archive in the future, we might decide to do
# otherwise.
check_DATA = $(SOURCEIMGS:%=./data/%) $(BUILTIMGS:%=./data/%)
dist_check_DATA = $(BUILTDATA) 
EXTRA_DIST = $(SOURCEIMGS:%=data-src/%)

# Conversion rules.

.png.ppm:
	mkdir -p data-src && pngtopnm $< > $@

.ppm.pgm:
	mkdir -p data-src && ppmtopgm $< > $@

.pgm.pbm:
	mkdir -p data-src && pgmtopbm $< > $@

.ppm.pppm:
	mkdir -p data-src && pnmtoplainpnm $< > $@

.pgm.ppgm:
	mkdir -p data-src && pnmtoplainpnm $< > $@

.pbm.ppbm:
	mkdir -p data-src && pnmtoplainpnm $< > $@

# This is a bit hackish.  There is no way to introduce a derivation
# for `' -> `.gz', so we define the `m' -> `m.gz' rule instead.
# (all our suffixes ends in `m': ppm, pgm, pbm.)
SUFFIXES = m m.gz
mm.gz:
	gzip -c -9 $< > $@

data-src/lena128.pgm: data-src/lena.pgm
	mkdir -p data-src && convert -scale 25% data-src/lena.pgm $@

data/%: $(srcdir)/data-src/%
	mkdir -p data && cp $< $@

CLEANFILES = data/*