# Process current dir first to built the data.
SUBDIRS = . check io morpho

# SOURCEDATA should never be erased, they are needed to generate
# BUILTDATA.  SOURCEDATA are the only data files worth to keep under
# version control.
SOURCEDATA = data/lena.ppm

# BUILTDATA are generated from SOURCEDATA and can be erased
# by maintainer-clean.
BUILTDATA =					\
  data/lena.pbm					\
  data/lena.pgm					\
  data/lena.ppbm				\
  data/lena.ppgm				\
  data/lena.pppm				\
  data/lena128.pgm

MAINTAINERCLEANFILES = $(BUILTDATA)

# Currently we ship all data files, even generated, in case the user
# doesn't have all the tools required to generated them.  If it turns
# out it makes a too big archive in the future, we might decide to do
# otherwise.
dist_check_DATA = $(SOURCEDATA) $(BUILTDATA)


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

data/lena128.pgm: data/lena.pgm
	convert -scale 25% data/lena.pgm $@
